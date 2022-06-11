#include "../lib/queue.h"
#include <stdlib.h>
#include <stdio.h>

int queue_add(void* new_obejct, queue_object* queue){
    // TODO
    queue_object* new_object = (queue_object*) malloc(sizeof(queue_object));
    new_object->next = queue->next;
    new_object->object = new_obejct;
    queue->next = new_object;
    return 0;
}

void* queue_poll(queue_object* queue){
    // TODO
    queue_object* current_object = (queue_object*) malloc(sizeof(queue_object));
    queue_object* previous_object = (queue_object*) malloc(sizeof(queue_object));
    if (queue == NULL || queue->next == NULL) {
        return NULL;
    }
    current_object = queue->next;
    while (current_object->next != NULL) {
        previous_object = current_object;
        current_object = current_object->next;
    }
    previous_object->next = NULL;
    return current_object->object;
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
    queue_object* item = queue->next;
    if (item->next != NULL) {
        free_queue(item->next);
        free(item);
    }
    free(queue);
}

void* queue_peek(queue_object* queue){
    // TODO
    queue_object* current_object = (queue_object*) malloc(sizeof(queue_object));
    if (queue == NULL || queue->next == NULL) {
        return NULL;
    }
    current_object = queue->next;
    while(current_object->next != NULL) {
        current_object = current_object->next;
    }
    return current_object->object;
}
