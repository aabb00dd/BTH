# Custom Memory Manager in C

This is a school project developed for the **Programming in C** course. The project implements a simplified version of dynamic memory management using a **custom memory allocator** with support for thread-safety, best-fit allocation, memory resizing, and coalescing of free blocks all built using a **custom linked list** structure.

---

## Features

- Manual implementation of `malloc`, `free`, and `realloc`-like functionality:
  - `mem_alloc(size_t size)`
  - `mem_free(void* ptr)`
  - `mem_resize(void* ptr, size_t new_size)`
- Best-fit block selection strategy for efficient memory usage
- Automatic block splitting and merging (coalescing)
- Thread-safe via `pthread_mutex_t`
- Internal linked list structure to manage memory blocks
- Edge-case handling (e.g., allocation size 0, invalid free)
