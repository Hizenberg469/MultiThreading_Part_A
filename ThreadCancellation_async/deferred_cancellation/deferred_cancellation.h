#ifndef __MASTER_SLAVE1_H
#define __MASTER_SLAVE1_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>


void*
write_into_file(void* arg);

#endif /* __MASTER_SLAVE1_H*/