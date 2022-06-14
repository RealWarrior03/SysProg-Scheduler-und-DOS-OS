#include "../lib/RR.h"

static queue_object* RR_queue;
//You can add more global variables
static int rrQuantum;
static int ticker;

process* RR_tick (process* running_process){
    // TODO
    if (running_process==NULL || running_process->time_left==0){
        running_process=queue_poll(RR_queue);
        ticker = rrQuantum;
    }
    if (ticker == 0) {
        queue_add(running_process, RR_queue);
        running_process = queue_poll(RR_queue);
        ticker = rrQuantum;
    }
    if (running_process!=NULL){
        running_process->time_left--;
        ticker--;
    }

    return running_process;
}

int RR_startup(int quantum){
    // TODO
    RR_queue=new_queue();
    rrQuantum = quantum;
    if (RR_queue==NULL){
        return 1;
    }
    return 0;
}


process* RR_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if (running_process == NULL) {
        return arriving_process;
    }
    if(arriving_process!=NULL){
        queue_add(arriving_process, RR_queue);
    }
    return running_process;
}


void RR_finish(){
    // TODO
    free(RR_queue);
}
