#include "linked_list.h"


static pthread_mutex_t list_mutex;  // Mutex for thread-safe operations


/**
 * @brief Initializes a new linked list.
 * 
 * This function initializes a new linked list by setting up the memory manager,
 * initializing the head pointer to NULL, and creating a mutex for thread-safe operations.
 * 
 * @param head Pointer to the head node pointer of the list.
 * @param size Size of the memory pool to be used by the memory manager.
 * 
 * @note If memory allocation fails in mem_init, the program will terminate.
 */
void list_init(Node** head, size_t size) 
{
    pthread_mutex_init(&list_mutex, NULL);  // Initialize the mutex
    mem_init(size);                         // Initialize the memory manager
    *head = NULL;                           // Set the head of the list to NULL (empty list)
}


/**
 * @brief Inserts a new node at the end of the list.
 * 
 * This function creates a new node with the given data and inserts it at the end of the list.
 * If the list is empty, the new node becomes the head of the list.
 * 
 * @param head Pointer to the head node pointer of the list.
 * @param data The data to be stored in the new node.
 * 
 * @note If head is NULL or memory allocation fails, the function will return without modifying the list.
 *       The function is thread-safe.
 */
void list_insert(Node** head, uint16_t data) 
{
    pthread_mutex_lock(&list_mutex);

    // If the head of the list is NULL, unlock the mutex and return
    if (!head)
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    Node* new_node = (Node*)mem_alloc(sizeof(Node)); // Allocate memory for the new node

    // If the allocation failed, unlock the mutex and return
    if (!new_node) 
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    new_node->data = data;          // Set the data of the new node
    new_node->next = NULL;          // Set the next pointer of the new node to NULL

    if (*head == NULL) 
    {
        *head = new_node;   // If the list is empty, make the new node the head
    } 
    else 
    {
        // Traverse to the end of the list
        Node* current_node = *head;
        while (current_node->next)
        {
            current_node = current_node->next;   // Move to the next node
        }
        current_node->next = new_node; // Add the new node at the end
    }
    pthread_mutex_unlock(&list_mutex);
}


/**
 * @brief Inserts a new node after a specified node.
 * 
 * This function creates a new node with the given data and inserts it after the specified node.
 * 
 * @param prev_node The node after which the new node should be inserted.
 * @param data The data to be stored in the new node.
 * 
 * @note If prev_node is NULL or memory allocation fails, the function will return without modifying the list.
 *       The function is thread-safe.
 */
void list_insert_after(Node* prev_node, uint16_t data) 
{
    pthread_mutex_lock(&list_mutex);
    
    // If the previous node is NULL, unlock the mutex and return
    if (!prev_node) 
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    Node* new_node = (Node*)mem_alloc(sizeof(Node)); // Allocate memory for the new node

    // If the allocation failed, unlock the mutex and return
    if (!new_node) 
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    // Initialize the new node and insert it after prev_node
    new_node->data = data;              // Set the data of the new node 
    new_node->next = prev_node->next;   // Set the next pointer of the new node to the next pointer of the previous node
    prev_node->next = new_node;         // Set the next pointer of the previous node to the new node

    pthread_mutex_unlock(&list_mutex);
}


/**
 * @brief Inserts a new node before a specified node.
 * 
 * This function creates a new node with the given data and inserts it before the specified node.
 * If the specified node is the head of the list, the new node becomes the new head.
 * 
 * @param head Pointer to the head node pointer of the list.
 * @param next_node The node before which the new node should be inserted.
 * @param data The data to be stored in the new node.
 * 
 * @note If head or next_node is NULL, or if memory allocation fails, the function will return without modifying the list.
 *       If next_node is not found in the list, the new node is freed and the list remains unchanged.
 *       The function is thread-safe.
 */
void list_insert_before(Node** head, Node* next_node, uint16_t data) 
{
    pthread_mutex_lock(&list_mutex);

    // If the head or next_node is NULL, unlock the mutex and return
    if (!head || !next_node) 
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    Node* new_node = (Node*)mem_alloc(sizeof(Node)); // Allocate memory for the new node

    // If the allocation failed, unlock the mutex and return
    if (!new_node) 
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    new_node->data = data;          // Set the data of the new node

    // If the head is the next_node, set the next pointer of the new node to the head
    if (*head == next_node) 
    {
        new_node->next = *head;     // Set the next pointer of the new node to the head
        *head = new_node;           // Make the new node the head
    } 
    else 
    {
        Node* current_node = *head;  // Initialize current_node to the head of the list

        // Find the node before next_node
        while (current_node && current_node->next != next_node)
        {
            current_node = current_node->next;  // Move to the next node
        }
        if (current_node)
        {
            new_node->next = current_node->next;      // Set the next pointer of the new node to the next pointer of the current node
            current_node->next = new_node;            // Set the next pointer of the current node to the new node
        }
        else 
        {
            mem_free(new_node); // If the current node is NULL, free the new node
        }
    }
    pthread_mutex_unlock(&list_mutex);
}


/**
 * @brief Deletes the first occurrence of a node with the specified data.
 * 
 * This function searches for a node with the given data and removes it from the list.
 * If the node to be deleted is the head, the head is updated.
 * 
 * @param head Pointer to the head node pointer of the list.
 * @param data The data of the node to be deleted.
 * 
 * @note If head is NULL or the list is empty, the function will return without modifying the list.
 *       If no node with the specified data is found, the list remains unchanged.
 *       The function is thread-safe.
 */
void list_delete(Node** head, uint16_t data) 
{
    pthread_mutex_lock(&list_mutex);

    // If the head is NULL, unlock the mutex and return
    if (!head || !*head) 
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    Node* current_node = *head; // Initialize current_node to the head of the list
    Node* prev_node = NULL;     // Initialize prev_node to NULL

    // Find the node to delete
    while (current_node && current_node->data != data) 
    {
        prev_node = current_node;           // Set the previous node to the current node
        current_node = current_node->next;  // Move to the next node
    }

    if (current_node) 
    {
        if (prev_node) 
        {
            prev_node->next = current_node->next; // If it's not the head, update the previous node's next pointer
        }
        else 
        {
            *head = current_node->next; // If it's the head, update the head   
        }

        mem_free(current_node); // Free the deleted node (current node)
    }
    pthread_mutex_unlock(&list_mutex);
}


/**
 * @brief Searches for a node with the specified data.
 * 
 * This function traverses the list to find the first node containing the given data.
 * 
 * @param head Pointer to the head node pointer of the list.
 * @param data The data to search for in the list.
 * 
 * @return Returns a pointer to the found node, or NULL if not found or if the list is empty.
 * 
 * @note Thread-safe operation is ensured using a mutex.
 * @note If head is NULL, the function returns NULL.
 */
Node* list_search(Node** head, uint16_t data) 
{
    pthread_mutex_lock(&list_mutex);

    // If the head is NULL, return NULL
    if (!head) 
    {
        pthread_mutex_unlock(&list_mutex);
        return NULL;
    }

    Node* current_node = *head; // Initialize current_node to the head of the list

    // Traverse the list to find the node with the given data
    while (current_node) 
    {
        // If the current node's data matches the given data, return the current node
        if (current_node->data == data) 
        {
            pthread_mutex_unlock(&list_mutex);
            return current_node;
        }
        current_node = current_node->next; // Move to the next node
    }

    pthread_mutex_unlock(&list_mutex);
    return NULL;
}


/**
 * @brief Displays the entire list.
 * 
 * This function prints all elements of the list in the format [elem1, elem2, ..., elemN].
 * 
 * @param head Pointer to the head node pointer of the list.
 * 
 * @note Thread-safe operation is ensured using a mutex.
 * @note If head is NULL or the list is empty, it prints an empty list [].
 */
void list_display(Node** head)
{
    pthread_mutex_lock(&list_mutex);

    // If the head is NULL, return
    if (!head || !*head) 
    {
        printf("[]");
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    Node* current_node = *head; // Initialize current_node to the head of the list

    printf("[");
    
    while (current_node) 
    {
        printf("%d", current_node->data);  // Print the data of the current node 
        current_node = current_node->next; // Move to the next node
        if (current_node) printf(", ");    // If the current node is not NULL, print a comma and space
    }

    printf("]");
    pthread_mutex_unlock(&list_mutex);
}


/**
 * @brief Displays a range of nodes in the list.
 * 
 * This function prints elements of the list from start_node to end_node (inclusive) 
 * in the format [elem1, elem2, ..., elemN].
 * 
 * @param head Pointer to the head node pointer of the list.
 * @param start_node The node to start displaying from. If NULL, starts from the head.
 * @param end_node The node to end displaying at. If NULL, displays to the end of the list.
 * 
 * @note Thread-safe operation is ensured using a mutex.
 * @note If head is NULL or the list is empty, it prints an empty list [].
 * @note If start_node is after end_node in the list, behavior is undefined.
 */
void list_display_range(Node** head, Node* start_node, Node* end_node) 
{
    pthread_mutex_lock(&list_mutex);

    // If the head is NULL, return
        if (!head || !*head) 
    {
        printf("[]");
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    // If start_node is NULL, set current_node to the head of the list
    Node* current_node;
    if (start_node) {
        current_node = start_node;
    } else {
        current_node = *head;
    }

    printf("[");
    bool first_element = true;

    while (current_node) 
    {
        // If it's not the first element, print a comma and space
        if (!first_element) 
        {
            printf(", ");
        } 
        else 
        {
            first_element = false;
        }

        printf("%d", current_node->data);       // Print the data of the current node
        if (current_node == end_node) break;    // If the current node is the end node, break the loop
        current_node = current_node->next;      // Move to the next node
    }

    printf("]");
    pthread_mutex_unlock(&list_mutex);
}


/**
 * @brief Counts the number of nodes in the list.
 * 
 * This function traverses the entire list and counts the number of nodes.
 * 
 * @param head Pointer to the head node pointer of the list.
 * 
 * @return Returns the number of nodes in the list.
 * 
 * @note Thread-safe operation is ensured using a mutex.
 * @note If head is NULL, the function returns 0.
 */
int list_count_nodes(Node** head) 
{
    pthread_mutex_lock(&list_mutex);

    // If the head is NULL, return 0
    if (!head) 
    {
        pthread_mutex_unlock(&list_mutex);
        return 0;
    }

    int node_count = 0;         // Initialize node_count to 0
    Node* current_node = *head; // Initialize current_node to the head of the list
    
    // Traverse the list to count the nodes
    while (current_node) 
    {
        node_count++;                       // Increment the node count
        current_node = current_node->next;  // Move to the next node
    }

    pthread_mutex_unlock(&list_mutex);
    return node_count;
}


/**
 * @brief Cleans up the entire list and releases associated resources.
 * 
 * This function:
 * 1. Frees all nodes in the list
 * 2. Sets the head to NULL
 * 3. Deinitializes the memory manager
 * 4. Destroys the mutex used for thread-safe operations
 * 
 * @param head Pointer to the head node pointer of the list.
 * 
 * @note If head is NULL, the function returns without performing any operations.
 * @note After this function is called, the list should not be used anymore.
 */
void list_cleanup(Node** head) 
{  
    // If the head is NULL, return
    if (!head) return;

    Node* current_node = *head; // Initialize current_node to the head of the list

    // Traverse the list to free the nodes
    while (current_node) 
    {
        Node* temp_node = current_node;     // Initialize temp_node to the current node
        current_node = current_node->next;  // Move to the next node
        mem_free(temp_node);                // Free the temporary node
    }

    *head = NULL; // Set the head to NULL
    mem_deinit(); // Deinitialize the memory manager

    pthread_mutex_destroy(&list_mutex); // Destroy the mutex
}