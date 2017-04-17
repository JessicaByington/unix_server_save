#pragma once
//********************************************
// Thread-safe queue implementation
//
// Author: Phil Howard
// Email:  phil.howard@oit.edu
//

// Opaque pointer to a queue
typedef void *queue_t;

queue_t queue_create();
void queue_insert(queue_t q, void *data);
void *queue_remove(queue_t q);
void queue_mark_closed(queue_t q);
int queue_is_open(queue_t q);
void queue_destroy(queue_t q);
