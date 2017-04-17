/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        usr_lib.h
 * Date Created:    4/21/16
 * Modificatons:    
 * *****************************************************************/
#include "syscalls.h"

/**************************************************************
 *  Purpose: this is called by user programs to initiate a TRAP
 *  
 *  Precondition: user program should have called this function
 *  passing in either a string or a int (int can be a address)
 *  TRAP function should be set up to receve a call to it.
 *
 *  Postcondition: TRAP should have been initiated  
 * **********************************************************/
int syscall(int op, char * string, int value)
{

    int LP;
    LP = 0;
    io_blk_t io_blk;
    io_blk.op = op;

    LP = asm2("PUSHREG", LP_REG);

    if(op == PRINT_OP)
    {

        if(string < 0 || string > LP)
        {
            return -1;
        } 
        
        asm("TRAP");

        return 0;
    }
    else if(op == GETI_CALL)
    {
        io_blk.addr = value;

        if(value < 0 || value > LP)
        {
            return -1;
        } 

        asm("TRAP");
    
        return 0;
    }
    else if(op == GETL_CALL)
    {
        io_blk.addr = string;

        if(string < 0 || string > LP)
        {
            return -1;
        } 
        
        asm("TRAP");
    
        return 0;
    }

    return -1;
}


/**************************************************************
 *  Purpose: To allow the a system print in privileged mode 
 *  
 *  Precondition: message to be sent
 *
 *  Postcondition: message is printed out
 * **********************************************************/
int sys_prints(char * msg)
{
    asm("OUTS", msg);
    return 0;
}

/**************************************************************
 *  Purpose: to print a string using a syscall
 *  
 *  Precondition: syscall should be set up, message should be 
 *  passed to function
 *
 *  Postcondition: message will be printed out
 * **********************************************************/
int prints(char *string)
{
    //prints string to stdout
    int return_val;
    return_val = 0;

    return_val = syscall(PRINT_OP, string, 0);

    return return_val;
}

/**************************************************************
 *  Purpose: to print a int using a syscall
 *  
 *  Precondition: syscall should be set up, message should be 
 *  passed to function
 *
 *  Postcondition: message will be printed out
 * **********************************************************/
int printi(int value)
{
    //prints int value to stdout
    int return_val;
    char str_val [15];
    return_val = 0;

    itostr(value, str_val);
    return_val = prints(str_val);

    return 0;
}

/**************************************************************
 *  Purpose: Retrieve a int value from stdin
 *  
 *  Precondition: value should be availble to retrieve from stdin
 *
 *  Postcondition: value will have be retrieved
 * **********************************************************/
int geti()
{
    //gets int value from stdin
    int return_val;
    int stdin_val;
    return_val = 0;
    stdin_val = 0;

    return_val = syscall(GETI_CALL, "", &stdin_val);

    return stdin_val;
}

/**************************************************************
 *  Purpose: Retrieve a string value from stdin
 *  
 *  Precondition: value should be availble to retrieve from stdin
 *
 *  Postcondition: value will have be retrieved
 * **********************************************************/
int gets(char *buff)
{
    //acts like fgets(buff, 256, stdin)
    syscall(GETL_CALL, buff, 0);

    return 0;
}

/**************************************************************
 *  Purpose: to execute a system halt call in privileged mode
 *  
 *  Precondition: halt needs to have been called, and in 
 *  privileged mode
 *
 *  Postcondition: system will have been put into a halted state.
 * **********************************************************/
int halt()
{
    syscall(HALT_CALL, 0);
}
