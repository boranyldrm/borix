//
// Created by boran on 02.10.2016.
//

#ifndef BORIX_PROCES_H
#define BORIX_PROCES_H

#include "../include/types.h"

// mximum number of process
#define MAX_PROCESS 50

// struct for status of the process
typedef struct P_STATUS {
    uint8 running;
    uint8 wait;
    uint8 end;
} P_Status;


typedef struct PROCESS {
    uint8 pid;  // 0-255 enough for separate processes id
    uint32 p_time;
    P_Status p_stat;
    uint8 p_priority;   // 0-255 enough for separate processes priority
    struct PROCESS *p_next;
} Process;


typedef struct SCHEDULER_QUEUE {
    Process *sched_proc[MAX_PROCESS];
    uint8 size;  //  0 < length < 50
} Sched_Q;


Process *p_root;


Sched_Q *q_create ();
void insertProcess (Sched_Q *queue, Process *process);
void peekProcess(Sched_Q *queue);
void schedule (Sched_Q *queue, uint8 root);
void q_delete (Sched_Q *queue);

#endif //BORIX_PROCES_H
