/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        user.c
 * Date Created:    4/18/16
 * Modificatons:    
 * *****************************************************************/
/*******************************************************************
 * Lab/Assignment: Lab 3b - Syscalls (Stackl)
 *
 *  Overview: 
 *     Learning how to use Stackl to interface with the OS using 
 *     TRAPs and syscalls.      
 *
 *  Input:
 *      The user will be able to input a string or a int into the 
 *      console, and be able to "echo" that value back if the print
 *      function is used.
 *
 *  Output:
 *      Either the program will output something that was hardcoded
 *      or something that the user typed in. 
 *      
 *
 *******************************************************************/
#include "usr_lib.h"

int main();

/**************************************************************
 *  Purpose: To test the functionality of the program, see if 
 *  it is able to get input and output things to the console
 *  
 *  Precondition: n/a
 *
 *  Postcondition: data will be recieved from user, and outputed
 *  as requested.
 * **********************************************************/
int main()
{
    char input[100];
    int num;
    num = 0;
    //sys_prints("Attempting to trap\n");
    //prints("Printed from trap\n");
    //printi(10);
    //prints("\n");

    prints("Hello world\n");
    gets(input);
    prints(input);

    
    printi(1);
    prints("\n");
    num = geti();
    printi(num);
    prints("\n");

    return 0;
}
