#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>

#include "list.h"
#include "sched.h"
#include "ready_queue.h"

// use a 1M stack
#define STACK_SIZE  (1024*1024)
#define NORMAL 0
#define EXIT 1
#define DETACHED 2
#define NON_DETACHED 3
#define EXIT_NONDETACHED 4
#define EXIT_DETACHED 5
#define JOIN_NONEXIT 6

//******************************************
// Definition of Thread Control Block
typedef struct thread_s
{
    int status;                         // Keep track of status of thread
    int detached_status;                // Keep track of detached status of thead
    unsigned long waiting_thread;       // To attached another thread to it
    void * result;                     // Store the result
    unsigned long sp;                   // Stack pointer for thread
    unsigned long fp;                   // Frame pointer for thread
    long *stack;                        // ptr to block of memory used for stack
    void *(*start_func)(void *arg);     // function to run in thread
    void *arg;                          // arg to be passed to start_func
} thread_t;

// Pointer to currently running thread
thread_t *Current_Thread;
thread_t *Prev_Thread;
queue_t queue;
list_t * list;

/**********************************************************************
 * Purpose:  This function can be used for adding debug statements to your code.
 * Debug statements can then be turned on/off with the Do_Debug flag.
 *
 * Precondition:
 *     None
 *
 * Postcondition:
 *      text printed to stderr if Do_Debug is true
 ************************************************************************/
static int Do_Debug = 0;
static void debug_print(const char *fmt, ...)
{
    if (!Do_Debug) return;

    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);
    va_end(args);
}

/**********************************************************************
 * Purpose:  This function can be used for adding debug statements to your code.
 * Debug statements can then be turned on/off with the Do_Debug flag.
 *
 * Precondition:
 *     None
 *
 * Postcondition:
 *      text printed to stderr if Do_Debug is true
 ************************************************************************/
void mythread_init()
{
    queue = Q_Init();
    list = list_init();

    // Create TCB for whoever is initing me
    Current_Thread = (thread_t *)malloc(sizeof(thread_t));

    assert(Current_Thread != NULL);

    memset(Current_Thread, 0, sizeof(thread_t));

    Current_Thread->sp = 0;
    Current_Thread->fp = 0;
    Current_Thread->stack = NULL;
    Current_Thread->start_func = NULL;
    Current_Thread->arg = NULL;
    Current_Thread->status = NORMAL;
    Current_Thread->detached_status = NON_DETACHED;
    Current_Thread->waiting_thread = 0;
    Current_Thread->result = NULL;
}

// //*************************************
// Function that gets called on thread startup. It calls the main thread
// function and then stops the thread.
/**********************************************************************
 * Purpose: This function gets called on thread startup. It calles the
 * user supplied function and then exits the thread.
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *      Current_Thread points to a newly created TCB
 *
 * Postcondition:
 *      User's thread has completed
 ************************************************************************/
static void start_thread_func()
{
    void *result;
    result = Current_Thread->start_func(Current_Thread->arg);

    mythread_exit(result);
}

/**********************************************************************
 * Purpose: This function will create a new user thread
 *      func is the function to run in the thread
 *      arg is the argument to be passed to the function
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      The user's thread has been created
 *      return: the thread_id of the newly created thread is returned
 ************************************************************************/
unsigned long mythread_create( void *(*func)(void *arg), void *arg)
{
    long *mem;
    thread_t *thread;

    // allocate the thread control block
    thread = (thread_t *)malloc(sizeof(thread_t));
    assert(thread != NULL);

    memset(thread, 0, sizeof(thread_t));

    // Allocate space for the stack
    thread->stack = (long *)malloc(STACK_SIZE*sizeof(long));

    // Remember the user's thread function and argument
    thread->start_func = func;
    thread->arg = arg;

    // Initialize the stack. Pretend we made a call to yield 
    // from the beginning of start_thread_func
    //
    // NOTE: The following code, even though written in C is machine specific
    mem = thread->stack;

    mem[STACK_SIZE-1] = (long)start_thread_func;        // return addr
    mem[STACK_SIZE-2] = (long)&mem[STACK_SIZE-2];       // frame ptr
    thread->fp = (long)&mem[STACK_SIZE-2];              // save FP
    thread->sp = (long)&mem[STACK_SIZE-2];              // save SP

    // a call to yield will now run this thread
    /*debug_print("Thread is running\n");
    unsigned long reg;

    Current_Thread = thread;

    reg = thread->sp;
    __asm__ volatile("movq %0, %%rsp" : : "m" (reg) :);

    reg = thread->fp;
    __asm__ volatile("movq %0, %%rbp" : : "m" (reg) :);*/

    Q_Enqueue(queue, thread);

    list_push(list, thread);

    return (unsigned long)thread;   // should return thread_id
}

/**********************************************************************
 * Purpose: This function is called by threads in order to yield the CPU
 * NOTE: This code is CPU and compiler option specific
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      none
 ************************************************************************/
void mythread_yield()
{
    unsigned long reg;

    debug_print("Thread is yielding\n");
    // save FP, SP and queue current thread in ready queue
    __asm__ volatile("movq %%rbp, %0" : "=m" (reg) : :);
    Current_Thread->fp = reg;
    __asm__ volatile("movq %%rsp, %0" : "=m" (reg) : :);
    Current_Thread->sp = reg;

    if(Current_Thread->status == NORMAL)
    {
        Q_Enqueue(queue, Current_Thread);
        // Current_Thread = pick_a_thread,any_thread();

        debug_print("Thread is running normal status\n");
    }
   
    Prev_Thread = Current_Thread;
    if((Current_Thread = Q_Dequeue(queue)) != NULL)
    {
        // restore the SP, FP
        reg = Current_Thread->sp;
        __asm__ volatile("movq %0, %%rsp" : : "m" (reg) :);

        reg = Current_Thread->fp;
        __asm__ volatile("movq %0, %%rbp" : : "m" (reg) :);


        if(Prev_Thread->status == EXIT && Prev_Thread->detached_status == DETACHED)
        {
            DeleteElement(list, Prev_Thread);
            free(Prev_Thread->stack);
            free(Prev_Thread);
        }
    }

    // return to next thread
    return;
}

/**********************************************************************
 * Purpose: This function will terminate the thread that called it
 *      result: the return v    alue of the thread
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      The user's thread has been terminated.
 *      If the thread is detached, the memory for the thread will be cleaned up
 *      If the thread is not detached, result is available for a caller to
 *          mythread_join.
 ************************************************************************/
void mythread_exit(void *result)
{
    debug_print("Thread is exiting\n");

    // clean up thread data structures
    Current_Thread->status = EXIT;

    Current_Thread->result = result;

    if(Current_Thread->waiting_thread != 0)
    {
        //just do the
        //the enqueue stuff here
        Q_Enqueue(queue, (thread_t*)Current_Thread->waiting_thread);
    }

    //Current_Thread = NULL;
    mythread_yield();
}

/**********************************************************************
 * Purpose: This function will wait for the specified thread to terminate.
 *      thread_id: the identifier of the thread to wait for
 *      result: location to store the thread's result.
 *              if result==NULL, the thread's result will be ignored
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *      thread_id is the ID of a thread that is not detached and has not already
 *          joined.
 *
 * Postcondition:
 *      The user's thread has finished
 *      The result of the user's thread is in result (if result != NULL)
 ************************************************************************/
void mythread_join(unsigned long thread_id, void **result)
{
    thread_t * thread = (thread_t *)thread_id;

    if(SearchElement(list, thread) == 0)
    {
        fprintf(stderr, "Invaild thread ID\n");
        return;
    }

    if (thread->status != EXIT)
    {
        thread->waiting_thread = (unsigned long)Current_Thread;
        Current_Thread->status = JOIN_NONEXIT;
        mythread_yield();
    }

    if(result != NULL)
       *result = thread->result;
    else
        debug_print("Result == NULL\n");

    DeleteElement(list, thread);
    free(thread->stack);
    free(thread);
}

/**********************************************************************
 * Purpose: This function will mark a thread a detached meaning that the
 *      thread's memory will be cleaned up immediately upon thread_exit()
 *
 *      thread_id: the ID of the thread to be detached
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *      thread_id has not already been detached and has not already finished
 *
 * Postcneondition:
 *      The user's thread has been marked as detached
 ************************************************************************/
void mythread_detach(unsigned long thread_id)
{
    //do the casting. 
    thread_t * detach_thread = (thread_t*)thread_id;

    if(SearchElement(list, detach_thread) == 1)
        detach_thread->detached_status = DETACHED;
    else
        fprintf(stderr, "Not a vaild thread ID\n");
}

/**********************************************************************
 * Purpose: This function shut down the user thread system and clean-up any
 *      memory used by the threading system
 *
 * Precondition:
 *      mythread_init has been called and mythread_cleanup has NOT.
 *
 * Postcondition:
 *      Memory has been reclaimed.
 *      the behavior of any calls to mythread_ functions after this call 
 *      is undefined
 ************************************************************************/
void mythread_cleanup()
{
    printf("cleaning up\n");

    if(Current_Thread->stack != NULL)
    {
        //something went wrong
        debug_print("Something went wrong in clean up\n");
        exit(-1);
    }
    else
    {
        DeleteElement(list, Current_Thread);
        free(Current_Thread);
        Q_Close(queue);
        Q_Destroy(queue);
    }
}

