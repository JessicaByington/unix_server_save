/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        process.c
 * Date Created:    4/1/16
 * Modificatons:    
 * *****************************************************************/
/*******************************************************************
 *
 * Lab/Assignment: Lab 1 - Processes and Pipes
 *
 *  Overview: 
 *      This program will use prefilter and postfilter to process the
 *      output of a specified command.
 *
 *  Input:
 *      Variable number of arguments, first arg will be the character
 *      input for prefilter, the second will be the arg input for 
 *      postfilter, the rest will specify the command whose output
 *      is to be filtered.
 *
 *      ex: process x y ls -l
 *
 *  Output:
 *      Will be the outputs from prefilter and postfilter based on 
 *      the chars passed in and the command given (ex. ls).
 *
 *******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

/***********************************************************************
 *  Purpose:
 *      Take in command line args, a fork into different processes to
 *      filter input
 *
 *  Precondition:
 *      Args need to be passed into program, some input needs to be 
 *      present
 *
 *  Postcondition:
 *      the input will be filtered by different processes and displayed
 *      to the console window
 * ********************************************************************/
int main (int argc, const char * argv[])
{
    const char * pre_char = argv[1];
    const char * post_char = argv[2];
    char * args[argc - 2 + 1];
    int status;
    int mypipe[4];
    int count = 4;

    char * bin = "/bin/";
    char cmd[strlen(argv[3])];

    strcat(cmd, bin);
    strcat(cmd, argv[3]);
    
    args[0] = cmd;

    int i = 1; 
    while (count != argc)
    {
        args[i] = (char*)argv[count];
        count++;
        i++;
    }

    args[i] = NULL;


    if (pipe(mypipe) == -1)
    {
        if(fprintf(stderr, "Pipe failed\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
        exit(1);
    }

    if (pipe(mypipe + 2) == -1)
    {
        if(fprintf(stderr, "Pipe + 2 failed\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
        exit(1);
    }

    pid_t pid0 = fork();

    if(pid0 < 0)
    {
        if(fprintf(stderr, "Failed to fork") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
        exit(1);
    }
    else if(pid0 == 0)
    {
        if(dup2(mypipe[1], 1) == -1)
        {
            if(fprintf(stderr, "Failed to write to end of pipe") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
            exit(1);
        }
            
        if (execv(cmd, args) == -1)
        {
             if(fprintf(stderr, "Failed to exec fork 0") < 0)
             {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
             }
             exit(1);
        }
    }
    if(close(mypipe[1]) != 0)
    {
        if(fprintf(stderr, "Failed to close pipe[1]\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
    }

    pid_t pid1 = fork();
            
    if(pid1 < 0)
    {
        if(fprintf(stderr, "Failed to fork") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }

        exit(1);
    }
    else if(pid1 == 0)
    {
        if(dup2(mypipe[0], 0) == -1)
        {
            if(fprintf(stderr, "Failed to read from end of pipe") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
            exit(1);
        }
               
        if(dup2(mypipe[3], 1) == -1)
        {
            if(fprintf(stderr, "Failed to write to end of pipe") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
            exit(1);
        }
                
        if (execl("./prefilter", "./prefilter", pre_char, NULL) == -1)
        {
            if(fprintf(stderr, "Failed to exec") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
            exit(1);
        }     
    }
    if(close(mypipe[0]) != 0)
    {
        if(fprintf(stderr, "Failed to close pipe[0]\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
    }

    if(close(mypipe[3]) != 0)
    {
        if(fprintf(stderr, "Failed to close pipe[3]\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
    }

    pid_t pid2 = fork();

    if(pid2 < 0)
    {
        if(fprintf(stderr, "Failed to fork") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
        exit(1);
    }
    else if(pid2 == 0)
    {
        if(dup2(mypipe[2], 0) == -1)
        {
            if(fprintf(stderr, "Failed to read from end of pipe") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
            exit(1);
        }
        
        if (execl("./postfilter", "./postfilter", post_char, NULL) == -1)
        {
            if(fprintf(stderr, "Failed to exec") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
            exit(1);                    
        }
    }
    if(close(mypipe[2]) != 0)
    {
        if(fprintf(stderr, "Failed to close pipe[2]\n") < 0)
        {
            fprintf(stderr, "Failed doing a fprintf");
            exit(1);
        }
    }
    
    for(int i = 0; i < 3; i++)
    {
        if(wait(&status) == -1)
            if(fprintf(stderr, "Failed to wait for child process") < 0)
            {
                fprintf(stderr, "Failed doing a fprintf");
                exit(1);
            }
    }

    return 0;
}
