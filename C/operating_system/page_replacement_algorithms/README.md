# Page Replacement Algorithm Simulator in C

This project implements three classical page replacement algorithms used in memory management:

- FIFO (First-In-First-Out)
- LRU (Least Recently Used)
- Optimal (Bélády’s Algorithm)

Each implementation simulates how a virtual memory manager decides which page to replace in physical memory on a page fault, given a sequence of memory references from a `.mem` trace file.

---

## Algorithm Descriptions

### FIFO (First-In-First-Out)

Replaces the oldest loaded page in memory.

- Implemented using a rotating index to simulate a queue.
- Fast and simple, but can suffer from Bélády’s anomaly.

**File:** `fifo.c`  
**Functionality:**
- Maintains a fixed-size array.
- Cycles through it as new pages are loaded.

---

### LRU (Least Recently Used)

Replaces the page that hasn’t been used for the longest time.

- Simulated using a singly linked list.
- More accurate to real-world memory usage patterns.

**File:** `lru.c`  
**Functionality:**
- Pages are re-ordered on access.
- Least-recently-used page is at the front of the list.

---

### Optimal (Bélády’s Algorithm)

Replaces the page that **will not be used for the longest time in the future**.

- Requires knowledge of the future (used only in simulations).

**File:** `optimal.c`  
**Functionality:**
- Loads all memory references into an array.
- On each fault, determines which page will be used farthest into the future.