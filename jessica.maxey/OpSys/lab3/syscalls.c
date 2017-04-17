/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        syscall.c
 * Date Created:    4/18/16
 * Modificatons:    
 * *****************************************************************/

#include "usr_lib.h"

// interrupt is not needed for this lab, but is needed to compile with -boot
int interrupt() {}


/**************************************************************
 *  Purpose: To execute TRAP insructions
 *  
 *  Precondition: TRAP needs to have been called, op, string 
 *  and value need to have been given values in syscall
 *
 *  Postcondition: Trap will have executed
 * **********************************************************/
int my_systrap(int op, char * string, int value)
{
    int LP;
    int BP;
    
    BP = 0;
    LP = 0;

    io_blk_t io_blk;
    io_blk.op = op;

    BP = asm2("PUSHREG", BP_REG);
    LP = asm2("PUSHREG", LP_REG);

    string += BP;
    value += BP;

    if(op == PRINT_OP)
    {

        if(string < 0 || string > LP)
        {
            return -1;
        } 
        
        asm("OUTS", string);

        return 0;
    }
    else if(op == GETI_CALL)
    {
        io_blk.addr = value;

        if(value < 0 || value > LP)
        {
            return -1;
        } 

        asm("INP", &io_blk);
    
        while(!(io_blk.op < 0))
        {}

        return 0;
    }
    else if(op == GETL_CALL)
    {
        io_blk.addr = string;

        if(string < 0 || string > LP)
        {
            return -1;
        } 
        
        asm("INP", &io_blk);
    
        while(!(io_blk.op < 0))
        {}

        return 0;
    }

}
/**************************************************************
 *  Purpose: Calls my_systrap to do the actual work
 *  
 *  Precondition: Trap has been called, args have been set
 *  and passed into function
 *
 *  Postcondition: will exit using RTI
 * **********************************************************/
int systrap(int op, char * string, int value)
{
    my_systrap(op, string, value);
    asm("RTI");
}


/**************************************************************
 *  Purpose: Start up the program
 *  
 *  Precondition: program needs to be running
 *
 *  Postcondition: program will have started running
 * **********************************************************/
int startup__()
{
    io_blk_t io_blk;

    int bp;
    int high_mem;

    // Set the BP leaving enough room for our stack
    bp = asm2("PUSHREG", SP_REG);
    bp += 64;
    asm2("POPREG", BP_REG, bp);

    // Load user.slb into memory
    io_blk.op = EXEC_CALL;
    io_blk.addr = "user.slb";
    io_blk.status = 0;
    asm("INP", &io_blk);
    while((io_blk.op & IO_COMPLETE) == 0)
    {
    }

    // Set the LP leaving 1000 bytes of stack space
    high_mem = bp + io_blk.status + 1000;
    asm2("POPREG", LP_REG, high_mem);

    // Set SP and FP
    // NOTE: FP must be set LAST!
    high_mem = io_blk.status + 4;
    asm2("POPREG", SP_REG, high_mem);
    asm2("POPREG", FP_REG, high_mem);

    // Execute user.slb
    asm2("JMPUSER", 8); 
    asm("HALT");
}

