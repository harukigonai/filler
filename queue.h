#ifndef __QUEUE_H__
#define __QUEUE_H__

struct Node {
    struct Node *prv;
    struct Node *nxt;
    void *data;
};

struct Queue {
    struct Node *head;
    struct Node *tail;
};

void init_queue(struct Queue *queue_ptr);
void enqueue(struct Queue *queue_ptr, void *new_elem_ptr);
void *dequeue(struct Queue *queue_ptr);
int queue_size(struct Queue *queue_ptr);
int queue_is_empty(struct Queue *queue_ptr);
void delete_queue(struct Queue *queue_ptr);

#endif