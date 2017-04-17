#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "myfs.h"

static int fs_fd = -1;
static int FS_Num_Blocks = 1000;            // in blocks
static int FS_Num_Inodes = 100;             // Max number of files

#define FS_BLOCKS_PER_INDIRECT 266
#define FS_INODES_PER_BLOCK  (FS_BLOCK_SIZE/sizeof(inode_t))

// the Super Block is kept in memory while FS is open
static super_block_t Super_Block;

// Cache of the last block of inodes read
static inode_t FS_Inode_Cache[FS_INODES_PER_BLOCK + 1];

// block number of the block in the Inode_Cache
static int FS_Inode_Cache_Block = -1;

// How many blocks are in the inode area?
#define FS_NUM_INODE_BLOCKS     (FS_Num_Inodes / FS_INODES_PER_BLOCK + 1)

// Index of first Data Block
#define FS_FIRST_DATA_BLOCK     (FS_NUM_INODE_BLOCKS + 2)

// Number of items in the Super Block's free list
// NOTE: This doesn't include slot zero, which is point to next free list
#define FS_FREE_LIST_SIZE       (sizeof(Super_Block.free_list)/  \
                                sizeof(Super_Block.free_list[0]) - 1)

// Number of items in the Super Block's free inode list
#define FS_FREE_INODE_SIZE      (sizeof(Super_Block.free_inode_list)/  \
                                sizeof(Super_Block.free_inode_list[0]))

//*************************************
int FS_Close()
{
    int status;

    status = FS_Write(&Super_Block, 1);
    if (status != FS_BLOCK_SIZE) return status;

    return close(fs_fd);
}

//*************************************
int FS_Open(const char *filename)
{
    int status;

    // indicate no inodes in the cache
    FS_Inode_Cache_Block = -1;

    // file is opened with O_SYNC so all activity gets flushed to disk
    fs_fd = open(filename, O_RDWR | O_SYNC);

    if (fs_fd < 0) return -1;

    // Read the Super Block so we have it in memory
    status = FS_Read(&Super_Block, 1);
    if (status != FS_BLOCK_SIZE) return status;

    return 0;
}

//*************************************
// NOTE: This will destructively format the file system
int FS_Create(const char *filename, u_int32_t fs_size)
{
    // buffer we can use to initialize all blocks on the virtual drive
    char block[FS_BLOCK_SIZE];
    int ii;
    int status;

    // create the file
    // Truncate the file if it already exists
    // NOTE: we do NOT use O_SYNC so that initialation goes fast
    //       we will reopen after init, so the live FS has O_SYNC
    fs_fd = open(filename, O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
    if (fs_fd < 0) return 1;

    FS_Num_Blocks = fs_size;

    // Fill the FS with zeros
    memset(block, 0, FS_BLOCK_SIZE);
    for (ii=0; ii<FS_Num_Blocks; ii++)
    {
        status = write(fs_fd, block, FS_BLOCK_SIZE);
        if (status != FS_BLOCK_SIZE) return -1;
    }

    // Initialize the Super Block
    memset(&Super_Block, 0, sizeof(Super_Block));

    // Free all data blocks
    for (ii=FS_Num_Blocks-1; ii>=FS_FIRST_DATA_BLOCK; ii--)
    {
        FS_Free_Block(ii);
    }

    // Free all inodes
    inode_t inode;
    memset(&inode, 0, sizeof(inode));
    for (ii=0; ii<FS_Num_Inodes; ii++)
    {
        inode.free = 0;             // pretend like inode is in use then
                                    // call FS_Free_Inode to free it
        inode.inode_number = ii;
        FS_Free_Inode(&inode);
    }

    // Close so everything is flushed to disk
    status = FS_Close();
    if (status) return status;

    // Open the live FS
    status = FS_Open(filename);
    if (status) return status;

    return 0;
}

//*************************************
int FS_Read(void *buff, u_int32_t block)
{
    int status;
    
    status = lseek(fs_fd, block*FS_BLOCK_SIZE, SEEK_SET);
    if (status != block*FS_BLOCK_SIZE) return -1;

    status = read(fs_fd, buff, FS_BLOCK_SIZE);
    if (status == FS_BLOCK_SIZE) return status;

    return -1;
}

//*************************************
int FS_Write(void *buff, u_int32_t block)
{
    int status;
    
    status = lseek(fs_fd, block*FS_BLOCK_SIZE, SEEK_SET);
    if (status != block*FS_BLOCK_SIZE) return -1;

    status = write(fs_fd, buff, FS_BLOCK_SIZE);
    if (status == FS_BLOCK_SIZE) return status;

    return -1;
}

//*************************************
int FS_Read_Inode(inode_t *inode, u_int32_t index)
{
    int inode_block;
    int status;
    int offset;

    // what block is the inode stored in?
    // Add 2 to account for boot block and super block
    inode_block = index / FS_INODES_PER_BLOCK + 2;

    // offset of the inode within the block
    offset = index % FS_INODES_PER_BLOCK;

    // if the desired block is NOT already in our cache, read it
    if (inode_block != FS_Inode_Cache_Block)
    {
        status = FS_Read(&FS_Inode_Cache, inode_block);
        if (status != FS_BLOCK_SIZE)
        {
            FS_Inode_Cache_Block = -1;
            return status;
        }
        FS_Inode_Cache_Block = inode_block;
    }

    // copy the requested inode from the cache into user's buffer
    memcpy(inode, &FS_Inode_Cache[offset], sizeof(inode_t));

    return 0;
}
//*************************************
int FS_Write_Inode(inode_t *inode)
{
    int inode_block;
    u_int32_t index;
    int offset;
    int status;

    // get inode number
    index = inode->inode_number;

    // figure out what block the inode lives in
    inode_block = index / FS_INODES_PER_BLOCK + 2;

    // offset of inode within the block
    offset = index % FS_INODES_PER_BLOCK;

    // make sure we have the inode in our cache
    if (inode_block != FS_Inode_Cache_Block)
    {
        status = FS_Read(&FS_Inode_Cache, inode_block);
        if (status != FS_BLOCK_SIZE)
        {
            FS_Inode_Cache_Block = -1;
            return status;
        }
        FS_Inode_Cache_Block = inode_block;
    }

    // copy the inode into the cache
    memcpy(&FS_Inode_Cache[offset], inode, sizeof(inode_t));

    // write back the cache
    status = FS_Write(&FS_Inode_Cache, inode_block);
    if (status != FS_BLOCK_SIZE) return status;

    return 0;
}
//*************************************
int FS_Read_File_Block(inode_t *inode, void *buff, u_int32_t block)
{
    int status = 0;

    u_int32_t temp_block = Translate_Block(inode, block, 0);

    if(temp_block <= 0) return -1;

    //do the checks
    if(temp_block == 0)
    {
        //if zero do the memset
        memset(buff, 0, FS_BLOCK_SIZE);
    } 
    else
    {
        //else read
        status = FS_Read(buff, temp_block);
        if (status != FS_BLOCK_SIZE) return -1;
    }
   
    return FS_BLOCK_SIZE;
}

//*************************************
int FS_Write_File_Block(inode_t *inode, void *buff, u_int32_t block)
{
    int status = 0;
    
    //Go get the real block location
    u_int32_t temp_block = Translate_Block(inode, block, 1);

    if (temp_block < 0) return -1;
    
    //then do the write using the block and the disk map
    status = FS_Write(buff, temp_block);
    if (status != FS_BLOCK_SIZE) return status;
    
    //use size things
    if(inode->size < ((block + 1) * FS_BLOCK_SIZE))
        inode->size = ((block + 1) *  FS_BLOCK_SIZE);
    
    return FS_BLOCK_SIZE;
}

u_int32_t Translate_Block(inode_t *inode, u_int32_t block, int do_write)
{
    int status = 0;

    if (block < 10) 
    {
        // direct block
        if (inode->disk_map[block] == 0 && do_write)
        {
            inode->disk_map[block] = FS_Alloc_Block();
            
            status = FS_Write_Inode(inode);
            if (status != 0) return status;
        }

        return inode->disk_map[block];
    }
    else if(block > 9 && block < FS_BLOCKS_PER_INDIRECT)
    {
        indirect_block_t indirect_block;

        // indirect block
        //do the checking with the disk map
        if(inode->disk_map[10] == 0)
        {
            if(do_write)
            {
                //if 0, then do that alloc, and set the block to the thing that was alloced
                inode->disk_map[10] = FS_Alloc_Block();
                if(inode->disk_map[10] == -1) return -1;

                memset(&indirect_block, 0, sizeof(indirect_block));
               
                status = FS_Write(&indirect_block, inode->disk_map[10]);
                if (status != FS_BLOCK_SIZE) return status;
            } 
            else
            {
                return 0;
            }
        }
        else
        {
            status = FS_Read(&indirect_block, inode->disk_map[10]);
            if (status != FS_BLOCK_SIZE) return status;
        }
            
        if (indirect_block.block[block - 10] == 0 && do_write) 
        {
            indirect_block.block[block - 10] = FS_Alloc_Block();
            if(indirect_block.block[block - 10] == -1) return -1;
                
            status = FS_Write(&indirect_block, inode->disk_map[10]);
            if (status != FS_BLOCK_SIZE) return status;
        }

        return indirect_block.block[block - 10];
    }
    else
    {
        return -1;
    }

}

//*************************************
int FS_Alloc_Inode(inode_t *inode)
{
    int status = 0;

    //if not empty
    if(Super_Block.num_free_inodes == 0)
    {
        //call function to go through list.
        Search_Inode();

        if(Super_Block.num_free_inodes == 0)
            return -1;
    }

    //do all this stuff regardless
    Super_Block.num_free_inodes--;
    
    //simply take one from the free list
    int inode_number = Super_Block.free_inode_list[Super_Block.num_free_inodes];

    status = FS_Write(&Super_Block, 1);
    if (status != FS_BLOCK_SIZE) return status;
    
    //call function;
    status = FS_Read_Inode(inode, inode_number);    
    if (status != 0) return status;

    //and mark it as busy
    inode->free = 0;
    
    status = FS_Write_Inode(inode);
    if (status != 0) return status;
    
    //return 0 on success
    return 0;
}

void Search_Inode()
{
    int start_block = FS_Inode_Cache_Block;
    int status = 0;
    int j;
    
    do
    {
        for(j = 0; j < FS_INODES_PER_BLOCK; j++)
        {
            //print out here fsinodecache.inodenumber
            //printf("Checking cache inode at %d\n", FS_Inode_Cache[j]);
            if(FS_Inode_Cache[j].free)
            {
               // printf("Free at inode number: %d\n", FS_Inode_Cache[j].inode_number);
                //might be a problem with the ++ or --, do print outs.
                Super_Block.free_inode_list[Super_Block.num_free_inodes++] = 
                    FS_Inode_Cache[j].inode_number;


                //Make sure that it doesn't not over fill the inode
                if(j == (FS_FREE_INODE_SIZE - 1)) return;
        
            }
        } 

        if (Super_Block.num_free_inodes != 0) return;

        if(FS_Inode_Cache_Block != FS_NUM_INODE_BLOCKS + 2)
        {
            FS_Inode_Cache_Block++;
        }
        else
            FS_Inode_Cache_Block = 2;


        status = FS_Read(FS_Inode_Cache, FS_Inode_Cache_Block);
        //printout here
        //printf("Reading Inode from %d from %d \n", 
          //      FS_Inode_Cache_Block, FS_Inode_Cache);
       
        if (status != FS_BLOCK_SIZE) return;

    } while(FS_Inode_Cache_Block != start_block);
}

//*************************************
int FS_Free_Inode(inode_t *inode)
{
    int status;

    if (Super_Block.num_free_inodes < FS_FREE_INODE_SIZE)
    {
        Super_Block.free_inode_list[ Super_Block.num_free_inodes] = 
            inode->inode_number;
        Super_Block.num_free_inodes++;

        status = FS_Write(&Super_Block, 1);
        if (status != FS_BLOCK_SIZE) return status;
    }

    inode->free = 1;

    status = FS_Write_Inode(inode);
    if (status != 0) return status;
    
    return status;
}
//*************************************
int FS_Alloc_Block()
{
    super_block_t temp_super_block;
    int free_block;
    int status;

    //is the free list not empty
    if(Super_Block.num_free_blocks != 0)
        //then return the top free block
        return Super_Block.free_list[Super_Block.num_free_blocks--];
    //if the free list is empty
    else
    {
        free_block = Super_Block.free_list[0];

        //read the block at [0] in free list
        //block = Super_Block.free_list[0];
        status = FS_Read(&temp_super_block, Super_Block.free_list[0]);

        /*printf("Reading SB from %d from %d to %d\n", 
                Super_Block.free_list[0], temp_super_block.free_list[1], 
                temp_super_block.free_list[FS_FREE_LIST_SIZE-1]);*/
        if (status != FS_BLOCK_SIZE) return status;

        //copy the free list from this block into the Super Block
        //Super_Block.free_list = block;
        memcpy(Super_Block.free_list, temp_super_block.free_list, sizeof(Super_Block.free_list));

        //might need - 1???
        Super_Block.num_free_blocks = FS_FREE_LIST_SIZE;

        //return block just read as the free block
        return free_block;
    }
    
    return -1;
}
//*************************************
int FS_Free_Block(u_int32_t block)
{
    int status;
    
    if(Super_Block.num_free_blocks != FS_FREE_LIST_SIZE)
    {
        //add to list
        Super_Block.num_free_blocks++;
        Super_Block.free_list[Super_Block.num_free_blocks] = block;
    }
    else
    {
        /*printf("Writing SB to %d from %d to %d\n", 
                block, Super_Block.free_list[1], 
                Super_Block.free_list[FS_FREE_LIST_SIZE-1]);*/
        status = FS_Write(&Super_Block, block);
        if (status != FS_BLOCK_SIZE) return status;
        
        Super_Block.free_list[0] = block;
        //set number free to 0 or 1
        Super_Block.num_free_blocks = 0;
    }

    status = FS_Write(&Super_Block, 1);
    if (status != FS_BLOCK_SIZE) return status;
   
    return status;
    //return -1;
}
