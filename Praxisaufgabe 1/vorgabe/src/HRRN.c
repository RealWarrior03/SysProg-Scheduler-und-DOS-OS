#include "../lib/HRRN.h"

static queue_object* HRRN_queue;
//You can add more global variables and structs here
static int counter;

int calculate_WaitingTime(process* process) { //calculate the waiting time of a process
    return (counter - process->start_time);
}

float calculate_ResponseRatio(process* process) { //calculate the response ratio of a process
    int waiting_time = calculate_WaitingTime(process);
    return (waiting_time + process->time_left)/process->time_left;
}

void addResponseRatio(process* process, queue_object* queue) {
    queue_object* new_process = (queue_object*) malloc(sizeof(queue_object)); //create a new object
    new_process->next = NULL;
    new_process->object = process;

    if (queue == NULL || process == NULL) { //if the queue or the process are NULL
        return; //leave the function
    }

    queue_object* current = queue; //set current for looping through the queue
    while(current->next != NULL) { //as long as current isn't the last object
        if (calculate_ResponseRatio(process) < calculate_ResponseRatio(current->next->object)) { //if the response ratio of the new process is smaller than the one after current
            current = current->next; //move to the next object
        } else {
            queue_add(process, current); //otherwise add it to the queue, behind the current object
            return;
        }
    }
    queue_add(process, queue); //if the queue is empty, add it as the first element
}

process* HRRN_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){ //if there is no running process or it has finished running
        running_process=queue_poll(HRRN_queue); //poll a new one
    }
    if (running_process!=NULL){ //if a running process exists
        running_process->time_left--; //reduce its remaining time
    }

    counter++; //increase the time
    return running_process;
}

int HRRN_startup(){
    // TODO
    counter = 0; //set the time to 0
    HRRN_queue=new_queue(); //create a new queue
    if (HRRN_queue==NULL){
        return 1; //if the queue wasn't create properly, return 1
    }
    return 0;
}

process* HRRN_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if (running_process == NULL) { //if there is no running process
        return arriving_process; //return the new one
    }
    if(arriving_process!=NULL){ //if the new one exists
        arriving_process->start_time = counter; //set its starting time
        addResponseRatio(arriving_process, HRRN_queue); //and add it to the queue depending on its response ratio
    }
    return running_process;
}

void HRRN_finish(){
    // TODO
    free(HRRN_queue);
}
