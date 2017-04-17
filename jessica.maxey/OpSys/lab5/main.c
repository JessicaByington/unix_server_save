/*************************************************************
 * Author:        Philip Howard
 * Filename:      prod_cons.h
 * Date Created:  5/4/2016
 * Modifications:
 * Edited by:     Jessica Maxey 
 **************************************************************
 *
 * Lab/Assignment: Lab 5 - Producer Consumer
 * 
 * Overview:
 *    This program is a producer-consumer application. 
 *    The producers read text files and send the text, one line at a time,
 *    to the consumers. The consumers print the test along with a 
 *    thread-id to identify which thread printed each line.
 *
 * Input:
 *    Command line arguments are used to specify the number of consumers and
 *    the filenames of the files to be read by producers. One producer will 
 *    be started for each filename.
 *
 * Output:
 *    The lines of text from the collected input files.
 *    Note: If there are multiple consumers, there is no guarantee as to the 
 *    order of the output lines.
 *
 * NOTE: this is PSEUDO-CODE, and it will have to be turned into real code
 * to complete this lab.
 ************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "prod_cons.h"

typedef struct arg_struct
{
    queue_t arg1;
    char *  arg2;
}arg_struct;

void * producer_caller(void * args)
{
    queue_t queue = ((arg_struct *)args)->arg1;
    char * filename = ((arg_struct *)args)->arg2;

    producer(queue, filename);

    free(args);
    return NULL;
}

int main(int argc, char **argv)
{
    printf("Jessica Maxey\n");
   
    printf("________________$$$$$$$\n\
____________$$$$$$$$$$$$$$\n\
_________$$$$$$$$$$$$$$$$$$\n\
________$$$$$$$$$$$$$$$$$$$$\n\
_______$$$$$$$$$$$$$$$☻$$$$$____$$$$$$\n\
_______$$$$$$$$☻$$$$$$$$$$$$___$$$__$$$\n\
_______$$$$$$$$$$$$$$$$$$$$__$$$$____$$\n\
________$$$$$$$$$$$$$$$$$$$$$$$$\n\
_________$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\
_$$$$$$_____$$$$$$$$$$$$$$$$$$________$$\n\
$$___$$$$$$$$$$$$$$$$$$$$$$$_________$$\n\
_$$$____$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\
_____$$$$$$$$__$$$_$$$$$$__________$$\n\
___$$$$_______$$$__$$$_$$$______$$$$\n\
__$$$_______$$$$___$$$__$$$\n\
___$$$$$___$$$_____$$$___$$$$___ $$\n\
____$$$____$$$$$$__$$$____$$$$$$$$\n\
____________$$$_____$$$$$___$$$$\n\n");

    int n_consumers;
    int ii;
    queue_t queue;
    pthread_t * consumer_thread;
    pthread_t * producer_thread;

    if (argc < 3)
    {
        fprintf(stderr, "Must specify the number of threads "
                        "and at least one file\n");
        return 1;
    }

    n_consumers = atoi(argv[1]);
    if (n_consumers < 1)
    {
        fprintf(stderr, "Must specify the number of threads "
                        "and at least one file\n");
        return 1;
    }

    queue = Q_Init();

    consumer_thread = malloc(n_consumers * sizeof(pthread_t));
    producer_thread = malloc((argc - 2) * sizeof(pthread_t));


    for (ii = 0; ii < argc - 2; ii++)
    {
        struct arg_struct * args = malloc(sizeof(arg_struct));
        
        args->arg1 = queue;
        args->arg2 = argv[ii + 2];
        
        if(pthread_create(&producer_thread[ii], NULL, producer_caller, (void*)args) != 0)
        {
            fprintf(stderr, "Failed to create producer thread\n");
            return -1;
        }
    }

    for (ii = 0; ii < n_consumers; ii++)
    {
        if(pthread_create(&consumer_thread[ii], NULL, consumer, queue) != 0)
        {
            fprintf(stderr, "Failed to create consumer thread\n");
            return -1;
        }
    }

    for (ii = 0; ii < argc-2; ii++)
    {
        if(pthread_join(producer_thread[ii], NULL) != 0)
        {
            fprintf(stderr, "Failed to join producer_thread\n");
            return -1;
        }
    }

    if(Q_Close(queue) != 0)
    {
        fprintf(stderr, "Failure caused while closing queue\n");
        return -1;    
    }

    for (ii = 0; ii < n_consumers; ii++)
    {
        if(pthread_join(consumer_thread[ii], NULL) != 0)
        {
            fprintf(stderr, "Failed to join consumer_thread\n");
            return -1;
        }
    }

    if(Q_Destroy(queue) != 0)
    {
        fprintf(stderr, "Failure cause while destroying queue\n");
        return -1;
    }
    

    free(consumer_thread);
    free(producer_thread);

    return 0;
}
