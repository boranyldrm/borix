//
// Created by boran on 02.10.2016.
//

#include "process.h"
#include "../mm/alloc.h"


Sched_Q *q_create () {
    Sched_Q *queue =  malloc(sizeof(Sched_Q));
    queue ->size = 0;
    return queue;
}


void insertProcess (Sched_Q *queue, Process *process) {
    if (queue->size >= MAX_PROCESS)
        return;

    // place the process at the end
    queue->sched_proc[queue->size] = process;

    uint8 place = queue->size;
    uint8 parent = (uint8) ((place - 1) / 2);

    while (place >= 0 &&
           queue->sched_proc[place]->p_priority > queue->sched_proc[parent]->p_priority) {
        Process *tmp = queue->sched_proc[parent];
        queue->sched_proc[parent] = queue->sched_proc[place];
        queue->sched_proc[place] = tmp;

        place = parent;
        parent = (uint8) ((place - 1) / 2);
    }
    queue->size++;
}

void peekProcess(Sched_Q *queue) {
    if (queue->size <= 0)
        return;

    p_root = queue->sched_proc[0];    // peek the max prority process
    queue->sched_proc[0] = queue->sched_proc[--queue->size];
    schedule(queue, 0);
}

void schedule(Sched_Q *queue, uint8 root) {
    uint8 left = (uint8) (2 * root + 1);   // index of root's left child
    if (left < queue->size) {
        // proc is not leaf so that it has a left left
        uint8 right = (uint8) (left + 1);  // root's right child

        if (right < queue->size &&
                queue->sched_proc[right]->p_priority > queue->sched_proc[left]->p_priority)
            left = right;

        // if root's priority is less than large child, swap processes
        if (queue->sched_proc[root]->p_priority < queue->sched_proc[left]->p_priority) {
            Process *tmp = queue->sched_proc[root];
            queue->sched_proc[root] = queue->sched_proc[left];
            queue->sched_proc[left] = tmp;

            // transform the new subtree
            schedule(queue, left);
        }
    }
}

void q_delete (Sched_Q *queue) {
    free(queue);
}