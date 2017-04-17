/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        prefilter.c
 * Date Created:    3/31/16
 * Modificatons:    
 * *****************************************************************/
/*******************************************************************
 *
 * Lab/Assignment: Lab 1 - Processes and Pipes
 *
 * Overview: 
 *  This program will read in a line of text from stdin and for each 
 *  line, prints to stdout all characters including and following a 
 *  specified character.
 *
 *  Input:
 *      The input will be a specified character passed in as a
 *      command line arg, and the input string will be typed in
 *      after the program starts which may or may not include 
 *      the prefilter character. 
 *
 *      Ex:
 *      prefilter x
 *      this is x a test
 *      1234x5678
 *      abcde
 *
 *  Output:
 *      The output will be the broken up string based on the location 
 *      of the dilimeter character, it will print everything after the
 *      character including the character its self. If there was no 
 *      character present in the string, it should print nothing.
 *
 *      Ex:
 *      x a test
 *      x5678
 *
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*******************************************************************
 *  Purpose: This function reads input, and filters that input based
 *      on the arg passed in.
 *
 *  Precondition:
 *      Argv[1] must contain a character, there must be input inputed
 *      into the program
 *
 *  Postcondition:
 *      Input will be printed out to the console window having been
 *      filtered.
 * ****************************************************************/
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

        str = strchr(buffer, delimiter);

        if(str != NULL)
        {
            if(fprintf(stderr, "%d pre %c: %s", getpid(), delimiter, buffer) < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }

            if(fprintf(stderr, "%d rest: %s", getpid(), str) < 0)
            {
                fprintf(stderr, "Failed doing a fprintf\n");
                exit(1);
            }
            printf("%s", str);
        }
        else
        {
            if(fprintf(stderr, "%d pre %c: %s\n", getpid(), delimiter, "") < 0)
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
