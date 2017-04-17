/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        postfilter.c
 * Date Created:    3/31/16
 * Modificatons:    
 * *****************************************************************/
/*******************************************************************
 *
 * Lab/Assignment: Lab 1 - Processes and Pipes
 *
 *  Overview: 
 *      Behaves similar to prefilter, except that it filters out 
 *      characters after the specified character instead of characters
 *      before the specified character. 
 *
 *  Input:
 *      The specified character, and the string of characters that is
 *      to be filtered
 *
 *      Ex:
 *      this is x a test
 *      1234x5678
 *      abcde
 *
 *  Output:
 *      The output will be the filtered string based on the location
 *      of the specified character in the input. It will print out 
 *      everything before the character, including that character.
 *      If there is no specified character in the string input, then 
 *      it will print the whole string. 
 *      
 *      Ex:
 *      this is x
 *      1234x
 *      abcde
 *
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/******************************************************************
 *  Purpose: This function reads input, and filters that input
 *      based on the arg passed in.
 *
 *  Precondition:
 *      Argv[1] must contain a character, there must be input 
 *      inputed into the program
 *
 *  Postcondition:
 *      Input will be printed out to the console window having
 *      been filtered.
 * ***************************************************************/

int main (int argc, const char * argv[])
{
    char buffer[255];
    
    if (argc != 2)
    {
        if(fprintf(stderr, "Invaild number of arguments, exiting\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf\n");
            exit(1);
        }
        exit(1);
    }
    
    //get the delimiter from command line
    const char delimiter = argv[1][0];

    //read in from stdin
    while( fgets(buffer, 255, stdin) != NULL)
    {
        char *str;

        str = strrchr(buffer, delimiter);

        if (str != NULL)
        {
            char new_str[str - buffer + 1];

            strncpy(new_str, buffer, str - buffer + 1);        
        
            strcat(new_str, "\n");

        
            if(fprintf(stderr, "%d post %c: %s", getpid(), delimiter, buffer) < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
           
            if(fprintf(stderr, "%d rest: %s", getpid(), new_str) < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
            printf("%s", new_str);
        }
        else
        {
            if(fprintf(stderr, "%d post %c: %s", getpid(), delimiter, buffer) < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
    
            if(fprintf(stderr, "%d rest: %s", getpid(), buffer) < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
    
            printf("%s", buffer);
        }

        if(fflush(stdout) == EOF)
        {
            if(fprintf(stderr, "Failed to fflush\n") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
        }
        
        if(fflush(stderr) == EOF)
        {
            if(fprintf(stderr, "Failed to fflush\n") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
        }
    }

    return 0;
}
