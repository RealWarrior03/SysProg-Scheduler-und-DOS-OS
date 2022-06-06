#include "../lib/MLF.h"
#include <stdio.h>

static queue_object** MLF_queues;
//You can add more global variables here

process* MLF_tick (process* running_process){
    // TODO
    return NULL;
}

/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int MLF_startup(){
    // TODO
    return 1;
}

process* MLF_new_arrival(process* arriving_process, process* running_process){
    // TODO
    return NULL;
}

/**
 * is called once after the all processes were handled. In case you want to cleanup something
 */
void MLF_finish(){
    // TODO
}
