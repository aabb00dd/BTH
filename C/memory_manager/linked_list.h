#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "memory_manager.h"
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

// Define the Node structure
typedef struct Node
{
    uint16_t data;           // Data stored in the node
    struct Node *next;       // Pointer to the next node      
    pthread_mutex_t lock;    // Mutex for each node
} Node;

// Function declarations
void list_init(Node **head, size_t size);
void list_insert(Node **head, uint16_t data);
void list_insert_after(Node *prev_node, uint16_t data);
void list_insert_before(Node **head, Node *next_node, uint16_t data);
void list_delete(Node **head, uint16_t data);
Node *list_search(Node **head, uint16_t data);
void list_display(Node **head);
void list_display_range(Node **head, Node *start_node, Node *end_node);
int list_count_nodes(Node **head);
void list_cleanup(Node **head);

#endif