#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>


// Block structure for memory management
typedef struct Block {
    size_t size_of_block;           // Size of the block
    int is_free;                    // Block free status: 1 if free, 0 if allocated
    struct Block* next_block;       // Pointer to the next block
    void* data;                     // Pointer to the data in the block
} Block;


#ifdef __cplusplus
extern "C"
{
#endif
    // Function declarations
    void mem_init(size_t size);
    void *mem_alloc(size_t size);
    void mem_free(void *block);
    void *mem_resize(void *block, size_t size);
    void mem_deinit();
#ifdef __cplusplus
}
#endif

#endif