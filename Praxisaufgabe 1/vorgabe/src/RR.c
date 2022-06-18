#include "../lib/RR.h"

static queue_object* RR_queue;
//You can add more global variables
static int rrQuantum;
static int ticker;

process* RR_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){ //if there is no running process or it finished running
        running_process=queue_poll(RR_queue); //poll a new one
        ticker = rrQuantum; //and reset the time to the allowed quantum
    }
    if (ticker == 0) { //if the time of a process is over
        queue_add(running_process, RR_queue); //add it back to the queue
        running_process = queue_poll(RR_queue); //get the next process that can run
        ticker = rrQuantum; //and reset the time
    }
    if (running_process!=NULL){ //if there is a running process
        running_process->time_left--; //reduce its remaining time
        ticker--; //and reduce the timer its still allowed to run
    }

    return running_process;
}

int RR_startup(int quantum){
    // TODO
    RR_queue=new_queue(); //create the queue
    rrQuantum = quantum; //save the quantum
    if (RR_queue==NULL){
        return 1; //if the queue wasn't created properly, return 1
    }
    return 0;
}


process* RR_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if (running_process == NULL) { //if there is no running process
        return arriving_process; //return the new arriving one
    }
    if(arriving_process!=NULL){ //if there is an arriving process
        queue_add(arriving_process, RR_queue); //add it to the queue
    }
    return running_process;
}


void RR_finish(){
    // TODO
    free_queue(RR_queue);
}
