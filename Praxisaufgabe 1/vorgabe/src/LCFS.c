#include "../lib/LCFS.h"

static queue_object* LCFS_queue;
//You can add more global variables here

int addToBeginning(void* new_process, queue_object* queue) {
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object));
    new_object->next = NULL;
    new_object->object = new_process;

    if (queue->next == NULL) {
        queue->next = new_object;
    } else {
        new_object->next = queue->next;
        queue->next = new_object;
    }

    return 0;
}

int LCFS_startup(){
    // TODO
    LCFS_queue=new_queue();
    if (LCFS_queue==NULL){
        return 1;
    }
    return 0;
}

process* LCFS_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll(LCFS_queue);
    }
    if (running_process!=NULL){
        running_process->time_left--;
    }

    return running_process;
}

process* LCFS_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if(arriving_process!=NULL){
        addToBeginning(arriving_process, LCFS_queue);
    }
    return running_process;
}

void LCFS_finish(){
    // TODO
    free(LCFS_queue);
}
