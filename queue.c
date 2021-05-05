#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_queue(struct Queue *queue_ptr)
{
    queue_ptr->head = malloc(sizeof(struct Node));
    queue_ptr->tail = malloc(sizeof(struct Node));
    queue_ptr->head->prv = NULL;
    queue_ptr->head->nxt = queue_ptr->tail;
    queue_ptr->tail->prv = queue_ptr->head;
    queue_ptr->tail->nxt = NULL;
}

void enqueue(struct Queue *queue_ptr, void *new_elem_ptr)
{
    struct Node *node_ptr = malloc(sizeof(struct Node));
    node_ptr->data = new_elem_ptr;
    
    struct Node *tmp = queue_ptr->head->nxt;
    queue_ptr->head->nxt = node_ptr;
    node_ptr->prv = queue_ptr->head;
    node_ptr->nxt = tmp;
    tmp->prv = node_ptr;
}
    
void *dequeue(struct Queue *queue_ptr)
{
    void *output = NULL;
    if(queue_size(queue_ptr) == 0) 
        printf("Error: Empty queue cannot be dequeued.\n");
    else {
        output = queue_ptr->tail->prv->data;
        struct Node *tmp = queue_ptr->tail->prv->prv;
        free(queue_ptr->tail->prv);
        queue_ptr->tail->prv = tmp;
        tmp->nxt = queue_ptr->tail;
    }
    return output;
}

int queue_size(struct Queue *queue_ptr)
{
    int i = 0;
    struct Node *tmp = queue_ptr->head->nxt;
    for (; tmp->nxt != NULL; i++)
        tmp = tmp->nxt;
    return i;
}

int queue_is_empty(struct Queue *queue_ptr) 
{
    return queue_size(queue_ptr) == 0;
}

void delete_queue(struct Queue *queue_ptr)
{
    free(queue_ptr->head);
    free(queue_ptr->tail);
}
