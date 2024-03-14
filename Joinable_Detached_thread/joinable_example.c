/*
 * =====================================================================================
 *
 *       Filename:  joinable_example.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/03/24 04:17:21 PM IST
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
#include <errno.h>   /* For using Global variable errno */

void *
thread_fn_callback(void *arg) {

    int th_id = *(int *)arg;

    free(arg);

    int rc = 0;

    while(rc != th_id ){

        printf("Thread %d doing some work\n", th_id);
        sleep(1);
        rc++;
    }

    int *result = calloc(1,sizeof(int));

    *result = th_id * th_id;

    return (void *)result;
}

void
thread_create(pthread_t *pthread_handle, int th_id){
    
    pthread_attr_t attr;
    
    int *_th_id = calloc(1, sizeof(int));
    *_th_id = th_id;

    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr,
            PTHREAD_CREATE_JOINABLE
            /* PTHREAD_CREATE_DETACHED */);

    pthread_create(pthread_handle,
                    &attr,
                    thread_fn_callback,
                    (void *)_th_id);


}

/* These thread would find the square of the
 * argument passed onto it */
pthread_t pthread2;
pthread_t pthread3;

int 
main( int argc, char **argv ){

    void *thread_result2 , *thread_result3;
    thread_create(&pthread2, 10);
    thread_create(&pthread3, 2);
    
    printf("main fn blocked on pthread join for thread with "
        "th_id = 2\n");

    pthread_join(pthread2, &thread_result2);
    
    if( thread_result2 ){

        printf("Return result from thread 2 = %d\n",
                *(int *)thread_result2);
        free(thread_result2);
        thread_result2 = NULL;
    }

     printf("main fn blocked on pthread join for thread with "
        "th_id = 3\n");

    pthread_join(pthread3, &thread_result3);
    
    if( thread_result3 ){

        printf("Return result from thread 3 = %d\n",
                *(int *)thread_result3);
        free(thread_result3);
        thread_result3 = NULL;
    }



    return 0;
}
