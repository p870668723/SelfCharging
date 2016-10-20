#ifndef MYOS_H
#define MYOS_H
#include "ucos_ii.h"

#define START_PRIO           4

extern  OS_STK TaskStk[OS_MAX_TASKS][OS_TASK_STK_SIZE];
extern void systick_init(void );
#endif
