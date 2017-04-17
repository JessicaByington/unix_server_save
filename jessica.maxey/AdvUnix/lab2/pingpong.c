/**************************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       pingpong.c
 *  Date Created:   4/14/16 
 * ***********************************************************************/
/**************************************************************************
 *  Lab/Assignment: Lab 2 - Processes & Signals
 *
 *  Overview: 
 *      This program will use a parent and child process and bounce signals
 *      back and forth for a specified number of times waiting 2 seconds
 *      between each.
 *
 *  Input:
 *      The only input is a #define in order to change the amount of times
 *      it is to bounce signals back and forth.
 *
 *  Output:
 *      Which signal is in use and the process id the is using it will be
 *      printed to the screen. When it is done executing, it will print
 *      that the parent/child process has finised and what status the child
 *      exited with.
 *
 * ***********************************************************************/
#define _POSIX_SOURCE
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

#define NUMBER_OF_TIMES 2 

void parent_signal(pid_t signo);
void child_signal(pid_t signo);
void sig_usr(int signo);

/**************************************************************************
 *  Purpose: To set up the signals that will be used, create the new child
 *      process, and send the respective processes to their signal 
 *      processing functions.
 *
 *  Precondition:
 *      N/A
 *
 *  Postcondition:
 *      Signals will have exited from their functions and the parent will
 *      wait for the child to exit, and then check it's exit status
 *
 * ***********************************************************************/
int main (int argc, char ** argv)
{
    int status;

    //create the signals so that they are duplcated in the child 
    //process as well
    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        perror("Can't catch SIGUSR1\n");
        exit(1);
    }

    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        perror("Can't catch SIGUSR2\n");
        exit(1);
    }
    
    //fork the process
    pid_t pid = fork();

//    sleep(2);       

    //check the pid to make sure it forked correctly
    if (pid < 0)
    {
        if(fprintf(stderr, "Failed to fork") < 0)
            exit(1);
    }
    else if(pid == 0)
    {
        //call the fucntion to process the signals sent from child
        child_signal(getppid());
    }
    else
    {
        //call the function to process the signals sent from parent
        parent_signal(pid);

        //wait for the child to exit
        if(wait(&status) == -1)
        {
            //wait failed for child
            perror("Wait failed\n");
        }
        else
        {
            //gets the exit status from the child if it exited correctly
            if(WIFEXITED(status))
            {
                fprintf(stdout, "Child with ID: %u died with status: %d\n", pid, WEXITSTATUS(status));
            }
            //gets the termination signal sent
            else if(WIFSIGNALED(status))
            {
                fprintf(stderr, "Child with ID: %u terminated because it didn't catch signal number %d\n", pid, WTERMSIG(status));
            }
        }

        fprintf(stdout, "Parent Finished\n");
    }



    return 0;
}

/**********************************************************************************
 *  Purpose:
 *      To send user signals out to the child process, and pause to receive the
 *      signal back from child.
 *
 *  Precondition:
 *      Child process is running as well, number of times to loop has been defined.
 *
 *  Postcondition:
 *      The loop will execute for the number of times specified printing out to the
 *      console the signal it sent and the pid who sent it, and then exit back to 
 *      main.
 *
 * *******************************************************************************/
void parent_signal(pid_t pid)
{
    int count = 0;

    fprintf(stdout, "Parent Started (PID = %u, Child ID = %u)\n", getpid(), pid);

    sleep(2);

    while(count < NUMBER_OF_TIMES)
    {
        //create new signal after orignal has been killed
        if(signal(SIGUSR1, sig_usr) == SIG_ERR)
        {
            perror("Can't catch SIGUSR1\n");
            exit(1);
        }

        if(signal(SIGUSR2, sig_usr) == SIG_ERR)
        {
            perror("Can't catch SIGUSR2\n");
            exit(1);
        }

        //send out sigusr1 to child
        fprintf(stdout, "Sending SIGUSR1 to process: %u\n", pid);
        kill(pid, SIGUSR1);
        //wait to recieve signal from child
        pause();
        sleep(2);
        
        //send out sigusr2 to child
        fprintf(stdout, "Sending SIGUSR2 to process: %u\n", pid);
        kill(pid, SIGUSR2);
        //wait to recieve signal from child
        pause();
        sleep(2);
        
        count++;
    }    
    
}

/**********************************************************************************
 *  Purpose:
 *      To send user signals out to the parent process, and pause to receive the
 *      signal back from parent.
 *
 *  Precondition:
 *      Child process is running as well, number of times to loop has been defined.
 *
 *  Postcondition:
 *      The loop will execute for the number of times specified printing out to the
 *      console the signal it sent and the pid who sent it, and then exit back to 
 *      main.
 *
 * *******************************************************************************/
void child_signal(pid_t pid)
{
    int count = 0;

    fprintf(stdout, "Child Started (PID = %u, PPID = %u)\n", getpid(), getppid());  

    
    while(count < NUMBER_OF_TIMES)
    {
        //wait from parent to send signal
        pause();
        sleep(2);    
    
        //creates new signal after the orignal has been killed
        if(signal(SIGUSR1, sig_usr) == SIG_ERR)
        {
            perror("Can't catch SIGUSR1\n");
            exit(1);
        }
    
        if(signal(SIGUSR2, sig_usr) == SIG_ERR)
        {
            perror("Can't catch SIGUSR2\n");
            exit(1);
        }
    
        //send out sigusr1 signal to parent
        fprintf(stdout, "Sending SIGUSR1 to process: %u\n", pid);
        kill(pid, SIGUSR1);
        //wait to recieve from parent
        pause();
        sleep(2);
     
        //send out sigusr2 signal to parent
        fprintf(stdout, "Sending SIGUSR2 to process: %u\n", pid);
        kill(pid, SIGUSR2);

        count++;
    }

    fprintf(stdout, "Child Finished\n");
}

/**********************************************************************************
 *  Purpose:
 *      To catch user signals recieved from the child or parent process.
 *
 *  Precondition:
 *      Child and parent processes are running and sending out user signals
 *
 *  Postcondition:
 *      Will print out what signal was recieved and from what pid it was sent from.
 *
 * *******************************************************************************/
void sig_usr(int signo)
{
    if(signo == SIGUSR1)
    {
        fprintf(stdout, "received SIGUSR1 in process: %u\n", getpid());
    }
    else if(signo == SIGUSR2)
    {
        fprintf(stdout, "received SIGUSR2 in process: %u\n", getpid());
    }
    else
        fprintf(stderr, "Error: received signal %d\n", signo);
}
