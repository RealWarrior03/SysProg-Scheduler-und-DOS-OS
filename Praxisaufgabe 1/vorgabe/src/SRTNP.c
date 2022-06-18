#include "../lib/SRTNP.h"

static queue_object* SRTNP_queue;
//You can add more global variables here

int addWithTime(process* new_process, queue_object* queue) {
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object)); //create a new queue object with a given process
    new_object->next = NULL;
    new_object->object = new_process;

    queue_object* current = queue->next; //create current and previous for looping through the queue
    queue_object* previous = queue;

    while (current != NULL) { //if current exists
        if (((process*) current->object)->time_left <= ((process*) new_object->object)->time_left) { //if the remaining time of the new object is greater than the current one
            previous = current; //continue to the next object
            current = current->next;
        } else if (current->next == NULL) { //if the end of the queue is reached
            current->next = new_object; //add the new object behind
            return 0;
        } else { //if the new objects remaining time is smaller than the current one
            previous->next = new_object; //add the new object in between current and previous
            new_object->next = current;
            return 0;
        }
    }

    queue->next = new_object; //if the queue is empty, just add the new object

    return 0;
}


process* SRTNP_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll(SRTNP_queue); //poll a new process if there is no running process or its finished running
    }
    if (running_process!=NULL){ //if a process is running
        running_process->time_left--; //reduce its remaining time
    }
    return running_process;
}

int SRTNP_startup(){
    // TODO
    SRTNP_queue=new_queue(); //create a queue
    if (SRTNP_queue==NULL){
        return 1; //if it wasn't created properly, return 1
    }
    return 0;
}

process* SRTNP_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if(arriving_process!=NULL){ //if there is a new process arriving
        queue_add(arriving_process, SRTNP_queue); //add it to the queue
    }
    return running_process;
}

void SRTNP_finish(){
    // TODO
    free_queue(SRTNP_queue);
}
