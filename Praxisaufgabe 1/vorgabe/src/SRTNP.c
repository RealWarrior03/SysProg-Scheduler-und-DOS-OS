#include "../lib/SRTNP.h"

static queue_object* SRTNP_queue;
//You can add more global variables here

int addWithTime(process* new_process, queue_object* queue) {
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object));
    new_object->next = NULL;
    new_object->object = new_process;

    queue_object* current = queue->next;
    queue_object* previous = queue;

    while (current != NULL) {
        if (((process*) current->object)->time_left <= ((process*) new_object->object)->time_left) {
            previous = current;
            current = current->next;
        } else if (current->next == NULL) {
            current->next = new_object;
            return 0;
        } else {
            previous->next = new_object;
            new_object->next = current;
            return 0;
        }
    }

    queue->next = new_object;

    return 0;
}


process* SRTNP_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll(SRTNP_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    }
    return running_process;
}

int SRTNP_startup(){
    // TODO
    SRTNP_queue=new_queue();
    if (SRTNP_queue==NULL){
        return 1;
    }
    return 0;
}

process* SRTNP_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if(arriving_process!=NULL){
        queue_add(arriving_process, SRTNP_queue);
    }
    return running_process;
}

void SRTNP_finish(){
    // TODO
    free(SRTNP_queue);
}
