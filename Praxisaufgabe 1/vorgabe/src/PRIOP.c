#include "../lib/PRIOP.h"
#include <stdio.h>

static queue_object* PRIOP_queue;
//You can add more global variables here

void addWithPriority(void* new_process, queue_object* queue) {
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object)); //create the new object
    new_object->next = NULL;
    new_object->object = new_process;

    queue_object* current = queue->next; //set current and previous object for looping through the queue
    queue_object* previous = queue;

    while (current != NULL) { //as long as the end isnt reached
        if (((process*) current->object)->priority >= ((process*) new_object->object)->priority) { //if the priority of the new process is smaller than the one of the current object
            if (current->next == NULL) { //if the last object is reached
                current->next = new_object; //add the new one behind
                return;
            }
            previous = current; //move to the next object
            current = current->next;
        } else { //if the priority of the new object is greater than the one of the current object
            previous->next = new_object;
            new_object->next = current;
            return; //add it before that object and return
        }
    }

    queue->next = new_object; //add the new object at the beginning, because the queue is empty
}

process* PRIOP_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){ //if the running process doesn't exist or is done
        running_process=queue_poll(PRIOP_queue); //get a new one
    }
    if (running_process!=NULL){ //if the running process exists
        running_process->time_left--; //reduce its remaining time
    }
    return running_process;
}

int PRIOP_startup(){
    // TODO
    PRIOP_queue=new_queue(); //create a new queue
    if (PRIOP_queue==NULL){
        return 1; //if it wasn't created properly, return 1
    }
    return 0; //otherwise 0
}

process* PRIOP_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if(arriving_process != NULL) { //if the new process exists
        if (running_process != NULL) { //and a running process exists
            if (arriving_process->priority > running_process->priority) { //if the priority of the new one is higher
                addWithPriority(running_process, PRIOP_queue); //add the running one to the queue
                return arriving_process; //and return the new one
            } else { //if the priority isn't higher
                addWithPriority(arriving_process, PRIOP_queue); //add the new process to the queue
                return running_process; //and return the running one
            }
        }
        return arriving_process; //if there is no running process, just return the arriving one
    }
    return running_process; //if there is no new process, return the running one
}

void PRIOP_finish(){
    // TODO
    free_queue(PRIOP_queue);
}
