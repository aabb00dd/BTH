#include "memory_manager.h"


// Global variables for memory management
static char *memory_pool = NULL;        // Pointer to the actual memory space for allocations
static char *header_pool = NULL;        // Pointer to the space for Block headers
static Block *free_list = NULL;         // Pointer to the list of free memory blocks
static size_t memory_pool_size;         // Total size of the memory pool
static pthread_mutex_t memory_lock;     // Mutex to ensure thread-safe memory operations
static pthread_mutex_t resize_lock;     // Mutex to ensure thread-safe resize operations


/**
 * @brief Initializes the memory manager.
 *
 * This function sets up the memory manager by:
 * 1. Initializing mutexes for thread-safe operations
 * 2. Allocating memory for the main memory pool and header pool
 * 3. Setting up the initial free block that spans the entire memory pool
 *
 * @param size The total size of the memory pool to be managed.
 *
 * @note If memory allocation fails for either pool, the program will terminate.
 */
void mem_init(size_t size) 
{
    // Initialize mutexes for thread-safe operations
    pthread_mutex_init(&memory_lock, NULL);
    pthread_mutex_init(&resize_lock, NULL);

    memory_pool_size = size;                            // Set the size of the memory pool
    memory_pool = (char*)malloc(size);                  // Allocate the main memory pool
    header_pool = (char*)malloc(size * 11);             // Allocate space for Block headers

    // Check if memory allocation failed
    if (!memory_pool || !header_pool) 
    {
        exit(EXIT_FAILURE);
    }

    // Initialize the free list with a single large block
    free_list = (Block*)header_pool;           // Initialize free_list to point to the first Block in the header pool
    free_list->size_of_block = size;           // The initial block is as large as the entire pool
    free_list->is_free = 1;                    // Mark the block as free
    free_list->next_block = NULL;              // This is the only block, so no next block
    free_list->data = memory_pool;             // Point to the start of the memory pool
}


/**
 * @brief Allocates a block of memory.
 *
 * This function searches for a suitable free block using a best-fit algorithm.
 * If a suitable block is found, it's split if it's significantly larger than the requested size.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if allocation fails.
 *
 * @note Thread-safe operation is ensured using a mutex.
 * @note If size is 0, it returns a pointer to the first free block without actually allocating.
 * @note If the memory pool or header pool is NULL, it returns NULL.
 * @note If no suitable block is found, it returns NULL.
 */
void* mem_alloc(size_t size) 
{
    pthread_mutex_lock(&memory_lock);

    // Check if memory pool or header pool is NULL
    if (!memory_pool || !header_pool)
    {
        pthread_mutex_unlock(&memory_lock);
        return NULL;
    }

    // If the size is 0, it doesn't actually allocate any memory. Instead, it searches for the first free block in the free list
    if (size == 0)
    {
        Block *current_block = free_list;                // Start at the beginning of the free list
        while (current_block) 
        {
            if (current_block->is_free)                  // If the block is free
            {
                void *result = current_block->data;      // If a free block is found, prepare to return its data pointer
                pthread_mutex_unlock(&memory_lock);
                return result;
            }
            current_block = current_block->next_block;   // Move to the next block
        }
        pthread_mutex_unlock(&memory_lock);
        return NULL;
    }

    Block *current_block = free_list;                    // Start at the beginning of the free list
    Block *best_fit = NULL;                              // Set the best fit block to NULL
    Block *prev_best = NULL;                             // Set the previous best fit block to NULL
    Block *prev_block = NULL;                            // Set the previous block to NULL

    while (current_block) 
    {
        // Check if the current block is a candidate for allocation
        if (current_block->is_free && current_block->size_of_block >= size) 
        {
            // Determine if this block is a better fit than the previously found best fit
            if (!best_fit || current_block->size_of_block < best_fit->size_of_block) 
            {
                best_fit = current_block;                // Update the best fit to this block
                prev_best = prev_block;                  // Keep track of the block before the best fit
            }
        }   
        prev_block = current_block;                     // Update the previous block to be the current block
        current_block = current_block->next_block;      // Move to the next block
    }

    // If a suitable block was found
    if (best_fit) 
    {
        // Check if the block is large enough to split
        if (best_fit->size_of_block > size + sizeof(Block)) 
        {
            // Create a new block from the excess space
            Block *new_block = (Block*)((char*)best_fit + sizeof(Block)); // The new block starts right after the memory for the best_fit block's header
            new_block->size_of_block = best_fit->size_of_block - size;    // Set the size of the new block
            new_block->is_free = 1;                                       // Mark the new block as free
            new_block->next_block = best_fit->next_block;                 // The new block's next pointer should point to whatever best_fit was pointing to
            new_block->data = best_fit->data + size;                      // Set the data pointer of the new block, which points to the start of the allocated memory
            
            best_fit->size_of_block = size;                               // Adjust the size of the best_fit block to exactly match the requested size
            best_fit->next_block = new_block;                             // Update best_fit's next pointer to the newly created block

            // If best_fit was at the start of the free list, update free_list to point to the new block
            if (!prev_best) 
            {
                free_list = new_block; 
            }
        }

        best_fit->is_free = 0;                            // Set the best fit block to be used
        void *result = best_fit->data;                    // Prepare to return the data of the best fit block
        pthread_mutex_unlock(&memory_lock);
        return result;
    }

    pthread_mutex_unlock(&memory_lock);
    return NULL;
}


/**
 * @brief Frees a previously allocated block of memory.
 *
 * This function marks the specified block as free and attempts to merge it with
 * adjacent free blocks to reduce fragmentation.
 *
 * @param block A pointer to the memory block to be freed.
 *
 * @note Thread-safe operation is ensured using a mutex.
 * @note If block is NULL, the memory pool is NULL, or the header pool is NULL, the function returns without action.
 * @note If the block to free is not found in the managed memory, the function returns without action.
 */
void mem_free(void* block) 
{
    pthread_mutex_lock(&memory_lock);

    // Check for invalid inputs or uninitialized memory manager
    if (!block || !memory_pool || !header_pool) 
    {
        pthread_mutex_unlock(&memory_lock);
        return;
    }

    Block *block_to_free = NULL;     // Start at the beginning of the free list
    Block *current = free_list;      // Pointer to the current block in the free list
    Block *prev = NULL;              // Pointer to the previous block in the free list

    while (current) 
    {
        // Search for the block to free by comparing data pointers
        if (current->data == block) 
        {
            block_to_free = current;    // If the block is found, set block_to_free to the current block
            break;
        }
        prev = current;                 // Keep track of the previous block
        current = current->next_block;  // Move to the next block
    }

    // If the block to free was not found, unlock the mutex and return
    if (!block_to_free) 
    {
        pthread_mutex_unlock(&memory_lock);
        return;
    }

    block_to_free->is_free = 1; // Mark the block as free

    // Merge with the next block if it's free
    if (block_to_free->next_block && block_to_free->next_block->is_free) 
    {
        block_to_free->size_of_block += block_to_free->next_block->size_of_block;  // Combine the sizes of current and next block
        block_to_free->next_block = block_to_free->next_block->next_block;         // Remove the next block from the list
    }

    // Merge with the previous block if it's free
    if (prev && prev->is_free) 
    {
        prev->size_of_block += block_to_free->size_of_block;                      // Combine the sizes of previous and current block
        prev->next_block = block_to_free->next_block;                             // Remove the current block from the list
        block_to_free = prev;                                                     // The merged block is now represented by the previous block
    }

    pthread_mutex_unlock(&memory_lock);
}


/**
 * @brief Resizes a previously allocated memory block.
 *
 * This function attempts to resize an existing memory block. If the current block
 * is large enough, it's returned unchanged. Otherwise, a new block is allocated,
 * data is copied, and the old block is freed.
 *
 * @param block A pointer to the memory block to be resized.
 * @param size The new size for the memory block.
 * @return A pointer to the resized memory block, or NULL if resizing fails.
 *
 * @note Thread-safe operation is ensured using a mutex.
 * @note If block is NULL, it behaves like mem_alloc(size).
 * @note If size is 0, it frees the block and returns NULL.
 * @note If the block is not found in the managed memory, it returns NULL.
 * @note If allocation of a new block fails, the original block is left unchanged and NULL is returned.
 */
void* mem_resize(void* block, size_t size) 
{

    pthread_mutex_lock(&resize_lock);

    // If the input block is NULL, simply allocate a new block
    if (!block) 
    {
        void* result = mem_alloc(size);
        pthread_mutex_unlock(&resize_lock);
        return result;
    }

    // If the requested size is 0, free the block and return NUL
    if (size == 0) 
    {
        mem_free(block);
        pthread_mutex_unlock(&resize_lock);
        return NULL;
    }

    Block* old_block = NULL;              // Initialize old_block to NULL
    Block* current = (Block*)header_pool; // Start at the beginning of the header pool

    // Iterate through all blocks in the header pool
    while (current < (Block*)(header_pool + memory_pool_size * sizeof(Block))) 
    {
        // Check if the current block's data pointer matches the input block
        if (current->data == block) 
        {
            old_block = current; // If the block is found, set old_block to the current block
            break;
        }
        current = (Block*)((char*)current + sizeof(Block)); // Move to the next block in the header pool
    }

    // If the block wasn't found, return NULL
    if (!old_block) 
    {
        pthread_mutex_unlock(&resize_lock);
        return NULL;
    }

    // If the current block is large enough, no need to resize
    if (old_block->size_of_block >= size) 
    {
        pthread_mutex_unlock(&resize_lock);
        return block;
    }

    // Allocate a new block with the requested size
    void* new_block = mem_alloc(size);

    // If allocation was successful, copy data and free the old block
    if (new_block) 
    {
        memcpy(new_block, block, old_block->size_of_block); 
        mem_free(block);                                  
    }
    
    pthread_mutex_unlock(&resize_lock);
    return new_block;
}


/**
 * @brief Deinitializes the memory manager.
 *
 * This function cleans up all resources used by the memory manager:
 * 1. Frees the memory pool and header pool
 * 2. Resets all global variables
 * 3. Destroys the mutexes used for thread-safe operations
 *
 * @note After this function is called, the memory manager should not be used anymore
 *       without re-initialization.
 */
void mem_deinit() 
{
    // Free the memory pool and header pool
    free(memory_pool); 
    free(header_pool);

    // Reset all global variables
    memory_pool = NULL; 
    header_pool = NULL; 
    free_list = NULL;   
    memory_pool_size = 0;

    // Destroy the mutexes
    pthread_mutex_destroy(&memory_lock); 
    pthread_mutex_destroy(&resize_lock);
}