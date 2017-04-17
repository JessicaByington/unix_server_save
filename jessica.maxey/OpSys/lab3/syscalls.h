/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        syscall.h
 * Date Created:    4/21/16
 * Modificatons:    
 * *****************************************************************/
#include <machine_def.h>
#include <string.h>
#define PRINTS_CALL 1
#define EXIT_CALL   2
#define HALT_CALL   3
#define PRINT_OP    4
#define GETL_CALL   6
#define GETS_CALL   5
#define GETI_CALL   7
#define EXEC_CALL   8
#define IO_QUEUED   0x10000000
#define IO_PENDING  0x20000000
#define IO_ERROR    0x40000000
#define IO_COMPLETE 0x80000000

/*****************************************************************
 *  Struct: io_blk_t
 *
 *  Purpose: This struct stores information that is assigned to it
 *  based on what functionalitiy is needed (using a string or a int)
 *
 *  The op will store which operation it is to do
 *  The addr will store the string or int values
 *  The status is not used this lab.
 ****************************************************************/
typedef struct 
{
    int op;
    int addr;
    int status;
} io_blk_t;
