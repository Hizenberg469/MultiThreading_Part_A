/*
 * =====================================================================================
 *
 *       Filename:  hello_world_thread.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/03/24 08:38:10 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /* For working with POSIX threads */
#include <unistd.h>  /* For pause() and sleep() */


/*Note: pthread_exit(0) API can be used to terminate a thread
 * , be it child thread or parent thread */


/* A thread callback fn must have following prototypes
 * void *(*thread_fn)(void *)
 * */
static void *
thread_fn_callback(void *arg){
    char *input = (char *)arg;
    while(1){
        printf("Input is : %s\n",input);
        sleep(1);
    }
}

/* Note that this function is not
 * main thread, rather it just a function
 * which create a thread and return to main()
 * function, which is actually the main thread of this
 * process */
void
thread1_create(){

    /* opaque object, don't bother about its internal
     * members */
    pthread_t pthread; /* Thread handler: This is like a variable which is used
                        * handle the thread processed */

    /* Take some argument to be passed to the thread fn,
     * Look after that you always pass the persistant memory
     * as an argument to the thread, do not pass caller's
     * local variables Or stack Memory */
    static char *thread_input1 = "I am thread 1, child thread of main thread";
    
    /* Return 0 on success, otherwise returns error-code, all
     * pthread functions return -ve error-code on failure, they
     * do not set global 'errno' variable */
    int rc = pthread_create(&pthread,
                            NULL,
                            thread_fn_callback,
                            (void *)thread_input1);

    if( rc != 0 ){
        printf("Error occured, thread could not be created, errno = %d\n",rc);
        exit(0);
    }
}

int
main(int argc , char **argv){
    
   thread1_create();
   printf("The execution of main function( or main thread ) is paused\n");
   //pause();
   /* Main thread, if complete its execution and return normally
    * by returning 0 then all the child thread of main thread terminates
    * but, if we use pthread_exit(0) to terminate main thread then the 
    * child threads can continue to exit without any obstruction. */

   pthread_exit(0);
   return 0;
}
