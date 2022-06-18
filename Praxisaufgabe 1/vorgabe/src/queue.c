#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void* new_obejct, queue_object* queue){
    // TODO
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object)); //creating new queue_object
    new_object->next = NULL;
    new_object->object = new_obejct;

    while (queue->next != NULL) { //while the end of the queue isn't reached, continue
        queue = queue->next;
    }
    queue->next = new_object; //after the last object is found, the new object is set as the next one

    return 0;
}

void* queue_poll(queue_object* queue){
    // TODO
    if (queue->next == NULL) { //if the queue is empty, NULL is returned
        return NULL;
    }
    queue_object* polling = queue->next; //get the first item
    void* object = polling->object; //store the process
    queue->next = polling->next; //remove the first item from the queue
    free(polling); //free the first item

    return object; //return the object of the first item
}

queue_object* new_queue(){
    // TODO
    queue_object* queue = (queue_object*) malloc(sizeof(queue_object)); //create the queue
    queue->next = NULL; //set everything to NULL
    queue->object = NULL;
    return queue; //return the created queue header
}


void free_queue(queue_object* queue){
    // TODO
    queue_object* item = queue; //item is the header of the queue
    if (item->next != NULL) { //as long as there is another object after the current one
        free_queue(item->next); //we call the function again from that object
    }
    free(item); //afterwards we free the named object
}

void* queue_peek(queue_object* queue){
    // TODO
    if (queue->next == NULL) { //if the queue is empty, we return NULL
        return NULL;
    }
    return queue->next->object; //otherwise we return the process of the first object in the queue
}
