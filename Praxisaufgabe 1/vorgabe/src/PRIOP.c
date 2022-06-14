#include "../lib/PRIOP.h"
#include <stdio.h>

static queue_object* PRIOP_queue;
//You can add more global variables here

int addWithPriority(void* new_process, queue_object* queue) {
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object));
    new_object->next = NULL;
    new_object->object = new_process;

    queue_object* current = queue->next;
    queue_object* previous = queue;

    while (current != NULL) {
        if (((process*) current->object)->priority >= ((process*) new_object->object)->priority) {
            if (current->next == NULL) {
                previous->next = new_object;
                new_object->next = current;
                return 0;
            }
            previous = current;
            current = current->next;
        } else {
            previous->next = new_object;
            new_object->next = current;
            return 0;
        }
    }

    queue->next = new_object;

    return 0;
}

process* PRIOP_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll(PRIOP_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    }
    return running_process;
}

int PRIOP_startup(){
    // TODO
    PRIOP_queue=new_queue();
    if (PRIOP_queue==NULL){
        return 1;
    }
    return 0;
}

process* PRIOP_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if(arriving_process != NULL) {
        if (running_process != NULL) {
            if (arriving_process->priority > running_process->priority) {
                addWithPriority(running_process, PRIOP_queue);
                return arriving_process;
            } else {
                addWithPriority(arriving_process, PRIOP_queue);
                return running_process;
            }
        }
        return arriving_process;
    }
    return running_process;
}

void PRIOP_finish(){
    // TODO
    free(PRIOP_queue);
}
