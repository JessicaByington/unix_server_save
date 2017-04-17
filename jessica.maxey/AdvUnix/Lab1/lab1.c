/**********************************************************
 *  Author:         Jessica Maxey
 *  Filename:       lab1.c
 *  Date Created:   4/5/16
 *  Modifications:  4/6/16 - Changed so that it creates a
 *      new file instead of opening a exsisting one.
 *                  4/7/16 - Changed the way permissions 
 *      are done when creating a new file.
 * *******************************************************/
/*********************************************************
 *  Lab/Assignment: Lab 1 - POSIX Compliant Code
 *
 *  Overview: 
 *      This program will use system calls to copy one file
 *      to another, only using POSIX compliant code.
 *
 *  Input:
 *      The input will be 2 arguments, the first is the
 *      source file to be copied, and the second is the
 *      name of the destination file.
 *
 *  Output: 
 *      The program will have created a new file based on
 *      the name previously provided, and will have copied
 *      all information from the source to this new file.
 *      There will be a string printed to the screen saying
 *      that it is being copyed, and another message saying
 *      when it is done.
 * ******************************************************/
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 255

/**********************************************************
 *  Purpose: This function gets the arguments, then copies
 *      the content from the source file to the dest file.
 *
 *  Precondition:
 *      A source file must have already been created, and 
 *      arguments for both the source file and dest file
 *      must be passed in to the program.
 *
 *  Postcondition:
 *      A dest file will have been created, and content 
 *      will have been copied.
 *
 * *******************************************************/
int main(int argc,  char ** argv)
{
    int returnVal = 0; 
    char * source_file;
    char * dest_file;
    char buffer[BUFF_SIZE];
    ssize_t bytes_in, bytes_out;

    if(argc != 3)
    {
        write(STDERR_FILENO,  "Error: Incorrect number of arguments\n",  37); 
        write(STDERR_FILENO,  "Usage: Lab1 <src file> <dst file>\n", 34); 
        exit(-1); 
    }
    else
    {
        // TODO
        // Make Appropriate assignments if correct number of parameters
        source_file = argv[1];
        dest_file = argv[2];
    }

    //TODO
    // Open the source and destination files, be sure to apply appropriate permissions
    // BE SURE TO CHECK FOR ERRORS WHEN OPENING FILES
    // If an error occurs, report the error and terminate
    // the program

    //opening source file as read only, and in append mode
    int fd_source = open(source_file, O_RDONLY | O_APPEND);

    //checks for error opening source file
    if(fd_source < 0)
    {
        write(2, "Error opening the source file\n", 30);
        exit(1);
    }
    
    //create stat struct to get permissions out of the source file
    struct stat fileStat;

    //gets data out of the file descripter for the source file
    //checks for a error while reading the stats 
    if(fstat(fd_source, &fileStat) < 0)
    {
        write(2, "Error reading stats for source file\n", 36);
        exit(1);
    }
    
    //opening and creating the dest file as write only, and creating with 
    //the same permissions as the source file.
    int fd_dest = open(dest_file, O_WRONLY | O_CREAT, fileStat.st_mode); 
   
    //checks for errer while opening/creating the dest file. 
    if(fd_dest < 0)
    {
        write(2, "Error opening the destination file\n", 35);
        exit(1);
    }


    // TODO
    // Determine the permission of the source file
    // Again, be sure to check for errors!
    
    
    // TODO
    // Output a status message here, something like
    // "Copying AAA to BBB"
    
    //printing out that it is copying the files, and
    //prints out the name of the source file and dest
    //files that were provied by the user.
    write(1, "Copying ", 8);
    write(1, source_file, strlen(source_file));
    write(1, " to ", 4);
    write(1, dest_file, strlen(dest_file)); 
    write(1, "\n", 1);

    // TODO
    // Do the work to actually copy the file here
    // Again, be sure to check for errors!
    
    //reads the source file, fills up buffer until equals the size
    //of buffer
    while((bytes_in = read(fd_source, &buffer, BUFF_SIZE)) > 0)
    {
        //writes the content of buffer to the dest file, then repeat.
        bytes_out = write(fd_dest, &buffer, (ssize_t)bytes_in);

        //checks to see if there was a error when writing to file
        if(bytes_out != bytes_in)
        {
            write(2, "Error while writing to file\n", 28);
            exit(1); 
        }
    }


    // TODO
    // Close the source and destination files here
    // As always, check for any errors that may be generated

    //closing source file, if it failed, print out error message
    if (close(fd_source) != 0)
    {
        write(2, "Error on close\n", 14);
        exit(1);
    }

    //closing destination file, if it failed, print out error message
    if (close(fd_dest) != 0)
    {
        write(2, "Error on close\n", 14);
        exit(1);
    }

    //Coping is complete, printing done to console.
    write(0, "DONE\n", 5); 

    return(returnVal); 
}
