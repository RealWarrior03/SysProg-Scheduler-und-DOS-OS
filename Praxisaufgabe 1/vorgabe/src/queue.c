#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void* new_obejct, queue_object* queue){
    // TODO
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object));
    new_object->next = NULL;
    new_object->object = new_obejct;

    while (queue->next != NULL) {
        queue = queue->next;
    }
    queue->next = new_object;

    return 0;
}

void* queue_poll(queue_object* queue){
    // TODO
    if (queue->next == NULL) {
        return NULL;
    }
    queue_object* polling = (queue_object*) malloc(sizeof(queue_object));
    polling = queue->next;
    void* object = polling->object;
    queue->next = polling->next;
    free(polling);

    return object;
}

queue_object* new_queue(){
    // TODO
    queue_object* queue = (queue_object*) malloc(sizeof(queue_object));
    queue->next = NULL;
    queue->object = NULL;
    return queue;
}


void free_queue(queue_object* queue){
    // TODO
    queue_object* item = queue;
    if (item->next != NULL) {
        free_queue(item->next);
        free(item);
    }
}

void* queue_peek(queue_object* queue){
    // TODO
    if (queue->next == NULL) {
        return NULL;
    }
    return queue->next->object;
}
