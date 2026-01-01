#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int find_page_func(int *pages, int no_phys_pages_int, int page_adress) {
    bool page_found = false;
    for (int i = 0; i < no_phys_pages_int; i++) {
        // If page is found
        if (pages[i] == page_adress) {
            // Set page_found to true
            page_found = true;
            // Break out of loop
            break;
        }
    }
    return page_found;
}


int main(int argc, char *argv[]) {
    // Fail fast if too few arguments
    if (argc != 4) {
        // Print error message if too few arguments
        printf("To few args. Use no_phys_pages, page_size, and filename\n");
        // Return with error
        return 1;
    }

    // Grab the arguments
    char* no_phys_pages = argv[1];
    char* page_size = argv[2];
    char* filename = argv[3];

    // Convert arguments to correct types using atoi (ascii to int)
    int no_phys_pages_int = atoi(no_phys_pages);
    int page_size_int = atoi(page_size);
    // Check for invalid page size
    if (page_size_int <= 0 || no_phys_pages_int <= 0) {
        // Print error message
        printf("Invalid page size or invalid number of physical pages\n");
        // Return with error
        return 1;
    }
    
    int first_page = 0; // Need to keep track of first page for FIFO
    int pages[no_phys_pages_int]; // Create array of pages with size no_phys_pages_int
    // Loop through the pages and set them to -1 (empty) 
    for (int i = 0; i < no_phys_pages_int; i++) {
        pages[i] = -1;
    }

    // Print the first two arguments as per instructions
    printf("No physical pages = %d, page size = %d\n", no_phys_pages_int, page_size_int);

    // Print starting to read memory trace from filename as per instructions
    printf("Reading memory trace from %s... ", filename);

    // Open dat bad boi
    FILE *input_file = fopen(filename, "r");
    // Check for read error
    if (input_file == NULL) {
        // Print error message
        printf("Error reading file\n");
        // Return with error
        return 1;
    }

    // Set up remaining variables
    int num_pagefaults = 0; // Number of pagefaults
    int num_memory_references = 0; // Number of memory references
	size_t line_size = 0; // Size of line
	char* line = NULL; // Line to read

    // Read the file line by line
    while (getline(&line, &line_size, input_file) != -1) { // While there are lines to read
        // Increment number of memory references
        num_memory_references++;
        // Create bool to check if page is found
        bool page_found = false;
        // Grab adress from line
        int current_adress = atoi(line);
        // Calculate the page adress (start of page) which is the current adress minus the remainder of the current adress divided by the page size
        int page_adress = current_adress - (current_adress % page_size_int);

        // Check if page is found
        page_found = find_page_func(pages, no_phys_pages_int, page_adress);

        // If statement to catch case where page is not found
        if (!page_found) {
            // Increment number of pagefaults
            num_pagefaults++;
            // Set the page to the current page (replace the oldest page since we are using FIFO)
            pages[first_page] = page_adress;
            // Increment first_page (since we are using FIFO)
            // Calculation means that first_page will loop back to 0 if it reaches no_phys_pages_int
            first_page = (first_page + 1) % no_phys_pages_int; 
        }
    }

    // Free the line and close the file to avoid memory leaks
    free(line);
    fclose(input_file);

    // Print the number of pagefaults as per instructions
    printf("Read %d memory references\n", num_memory_references);
    printf("Result: %d page faults\n", num_pagefaults);

    // Exit with success code 0
    return 0;
}
