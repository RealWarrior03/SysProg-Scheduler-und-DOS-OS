#include "../lib/MLF.h"
#include <stdio.h>

static queue_object** MLF_queues;
//You can add more global variables here
static queue_object* first_level_queue;
static queue_object* second_level_queue;
static queue_object* third_level_queue;
static queue_object* fourth_level_queue;
static int queueOfProcess;
static int counter;

process* pollNewMLFProcess() {
    if (first_level_queue != NULL && first_level_queue->next != NULL) { //if there are objects in the first level
        queueOfProcess = 1; //mark the queue level that the new process came from
        return queue_poll(first_level_queue); //and return the first object from that queue
    } else if (second_level_queue != NULL && second_level_queue->next != NULL) { //if the first queue is empty but the second level isn't
        queueOfProcess = 2; //mark the queue level that the new process came from
        return queue_poll(second_level_queue); //and return the first object from queue level 2
    } else if (third_level_queue != NULL && third_level_queue->next != NULL) { //if level 1 and level 2 are empty, but level 3 isn't
        queueOfProcess = 3; //mark the queue level of the new process
        return queue_poll(third_level_queue); //and return the first object from that queue
    } else if (fourth_level_queue != NULL && fourth_level_queue->next != NULL) { //if all three queues are empty, but the last level isn't
        queueOfProcess = 4; //mark the queue level of the new process
        return queue_poll(fourth_level_queue); //and return the first object from the fourth level queue
    }
    return NULL; //if all queues are empty, return NULL
}

process* MLF_tick (process* running_process){
    // TODO
    if (running_process == NULL || running_process->time_left == 0) { //if the running process is finished or doesn't exist
        running_process = pollNewMLFProcess(); //get a new process
    } else {
        if (queueOfProcess == 1 && counter % 1 == 0) { //if the process came from the first level and its time of 3^0 is over
            queue_add(running_process, second_level_queue); //add it to the second level
            counter = 0; //reset the timer
            running_process = pollNewMLFProcess(); //and get a new process
        } else if (queueOfProcess == 2 && counter % 3 == 0) { //if the process came from the second level and its time of 3^1 is over
            queue_add(running_process, third_level_queue); //add it to the third level
            counter = 0; //reset the timer
            running_process = pollNewMLFProcess(); //and get a new process
        } else if (queueOfProcess == 3 && counter % 9 == 0) { //if the process came from the second level and its time of 3^2 is over
            queue_add(running_process, fourth_level_queue); //add it the the fourth level
            counter = 0; //reset the timer
            running_process = pollNewMLFProcess(); //and get a new process
        }
    }
    counter++; //increase the counter
    running_process->time_left--; //decrease the remaining time
    return running_process; //and return the running process
}

/**
 * Do everything you have to at startup in this function. It is called once.
 * @result 0 if everything was fine. Any other number if there was an error.
 */
int MLF_startup(){
    // TODO
    first_level_queue = new_queue(); //create the four levels
    second_level_queue = new_queue();
    third_level_queue = new_queue();
    fourth_level_queue = new_queue();
    if (first_level_queue==NULL || second_level_queue == NULL || third_level_queue == NULL || fourth_level_queue == NULL){
        return 1; //if one of the levels wasn't created properly, return 1
    }
    return 0;
}

process* MLF_new_arrival(process* arriving_process, process* running_process){
    // TODO
    if (arriving_process != NULL) { //if the arriving process exists
        queue_add(arriving_process, first_level_queue); //add it to the first level
    }
    return running_process;
}

/**
 * is called once after the all processes were handled. In case you want to cleanup something
 */
void MLF_finish(){
    // TODO
    free_queue(first_level_queue);
    free_queue(second_level_queue);
    free_queue(third_level_queue);
    free_queue(fourth_level_queue);
}
