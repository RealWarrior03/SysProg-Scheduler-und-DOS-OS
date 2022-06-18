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
    LCFS_queue=new_queue(); //create a new queue
    if (LCFS_queue==NULL){
        return 1; //in case it didn't work, return 1
    }
    return 0; //otherwise 0
}

process* LCFS_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){ //if there is no running process or the process is done
        running_process=queue_poll(LCFS_queue); //the next process is polled
    }
    if (running_process!=NULL){ //if the running process exists and isn't finished
        running_process->time_left--; //the remaining time is decreased
    }

    return running_process;
}

process* LCFS_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if(arriving_process!=NULL){ //if the arriving process exists
        addToBeginning(arriving_process, LCFS_queue); //it gets added to the queue
    }
    return running_process;
}

void LCFS_finish(){
    // TODO
    free_queue(LCFS_queue);
}
/*
 * Created by Henry Krieger
 */
