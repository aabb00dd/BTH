#include <iostream>
#include <cstring> // for C-style string manipulation
#include <iomanip> // for cwd printing
#include <unistd.h>
#include <limits> 
#include <vector> // for vector
#include "fs.h"


// --- Filesystem class ---
// Filesystem class constructor
FS::FS() {
    // Read the FAT from the disk and store it in the FAT array
    disk.read(FAT_BLOCK, (uint8_t*)fat);

    // Read the root directory from the disk and store it in the contents_current_cwd array
    disk.read(ROOT_BLOCK, (uint8_t*)contents_current_cwd);

    // Set the current working directory to the root directory
    current_working_directory = "/";
    index_current_cwd = 0;
    parent_folder_indexes[index_current_cwd] = 0;
}


// Filesystem class destructor
FS::~FS() {
}


// --- Helper functions ---
int FS::fetch_free_block_index() {
    // Init return value
    int return_value = -1;

    // Loop through the FAT entries
    for (int i = 2; i < disk.get_no_blocks(); i++) {
        // Check if the entry is free
        if (fat[i] == FAT_FREE) {
            return_value = i;
            break;
        }
    }

    // If no free block was found, return -1, else return the index of the free block
    return return_value;
}


int FS::fetch_file_index_by_path(std::string filepath, dir_entry *entry) {
    // Init return value
    int return_value = -1;

    // Loop through the directory entries in the current working directory
    for (int i = 0; i < DIR_ENTRY_SIZE; i++) {
        if (std::strcmp(entry[i].file_name, filepath.c_str()) == 0) {
            return_value = i;
            break;
        }
    }

    // If no file was found, return -1, else return the index of the file
    return return_value;
}

int FS::change_cwd_using_path(std::string dirpath) {
    if (dirpath.empty()) return -1;

    // Check if we already are at the target directory
    if (dirpath == current_working_directory) {
        return 0;
    }

    // Handle absolute paths
    if (dirpath[0] == '/') {
        current_working_directory = "/";
        index_current_cwd = 0;
        disk.read(ROOT_BLOCK, (uint8_t*)contents_current_cwd);

        if (dirpath == "/") return 0; // Already at root
        dirpath = dirpath.substr(1); // Remove leading '/'
    }

    size_t i = 0;
    while (i < dirpath.size()) {
        size_t j = dirpath.find('/', i);
        if (j == std::string::npos) j = dirpath.size();

        std::string sub = dirpath.substr(i, j - i);
        i = j + 1;

        if (sub == "..") {
            if (index_current_cwd == 0) return 0; // Already at root
            index_current_cwd--;
            disk.read(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);
            current_working_directory = current_working_directory.substr(0, current_working_directory.find_last_of('/'));
            if (current_working_directory.empty()) current_working_directory = "/";
        } 
        else if (!sub.empty()) {
            int8_t dir_index = fetch_file_index_by_path(sub, contents_current_cwd);
            // Ensure directory exists
            if (dir_index < 0) {
                return -1; // Directory not found
            }

            // Ensure it's a directory
            if (contents_current_cwd[dir_index].type != TYPE_DIR) {
                return -2; // Not a directory
            }

            // Update state
            index_current_cwd++;

            // Update the current working directory
            parent_folder_indexes[index_current_cwd] = contents_current_cwd[dir_index].first_blk;
            disk.read(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);
            
            if (current_working_directory != "/") {
                current_working_directory += '/' + sub;
            } 
            else {
                current_working_directory += sub;
            }
        }
    }

    // Return with success
    return 0;
}

int FS::set_permissions(struct dir_entry *file, std::string accessrights) {
    // Check if the accessrights string is of the correct length
    if (accessrights.length() != 3) {
        std::cout << "Error: Access rights must be 3 characters long" << std::endl;
        return -1;
    }

    // Set the access rights for the file
    file->access_rights = 0;
    if (accessrights[0] == 'r') {
        file->access_rights |= READ;
    }
    if (accessrights[1] == 'w') {
        file->access_rights |= WRITE;
    }
    if (accessrights[2] == 'x') {
        file->access_rights |= EXECUTE;
    }

    return 0;
}


int FS::fetch_empty_entry_index() {
    // Init return value
    int return_value = -1;

    // Loop through the directory entries in the current working directory
    for (int i = 0; i < DIR_ENTRY_SIZE; i++) {
        // Check if the entry is empty
        if (contents_current_cwd[i].file_name[0] == '\0') {
            // Copy the empty entry to the entry pointer
            return_value = i;
            break;
        }
    }
    // If found index is 64, return -1 since no empty entry was found
    if (return_value == DIR_ENTRY_SIZE) {
        return_value = -1;
    }

    // If no empty entry was found, return -1
    return return_value;
}

// --- Filesystem commands ---
// formats the disk, i.e., creates an empty file system
/*
PDF description:
Formats the disk, i.e., initializes the FAT and marks all blocks as free (except block 0 (the root directory) and block 1
(the FAT block)).
*/
int FS::format() {
    std::memset(fat, FAT_FREE, sizeof(fat));
    fat[FAT_BLOCK] = FAT_EOF;
    fat[ROOT_BLOCK] = FAT_EOF;
    disk.write(FAT_BLOCK, (uint8_t*)fat);
    std::memset(contents_current_cwd, 0, sizeof(contents_current_cwd));
    disk.write(ROOT_BLOCK, (uint8_t*)contents_current_cwd);
    index_current_cwd = 0;
    parent_folder_indexes[index_current_cwd] = 0;
    return 0;
}

// create <filepath> creates a new file on the disk, the data content is
// written on the following rows (ended with an empty row)
/*
PDF description:
Creates a new file with the name filename on the disk. The data contents are written on the following rows. An empty
row ends the user input data. This enables the user to write several lines of input.
When all input data is written, the data is stored in the file filename. Remember to handle the case when a file is larger
than one disk block.
If there already exists a file with the same name, an error should be detected.
*/
int FS::create(std::string filepath) {
    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract file directory and name
    std::string file_dir_path = filepath.substr(0, filepath.find_last_of('/'));
    std::string file_name = filepath.substr(filepath.find_last_of('/') + 1);

    // Change to file directory
    if (file_dir_path != file_name) {
        int change_cwd_result = change_cwd_using_path(file_dir_path);
        if (change_cwd_result == -1) {
            printf("Error: File directory not found\n");
            return -1;
        }
    }

    // Check if there is space for a new file in the directory
    int8_t i = fetch_empty_entry_index();
    if (i == -1) {
        printf("Error: No space for new file in directory\n");
        return -1;
    }

    // Extract filename
    int8_t last_slash_index = filepath.find_last_of('/');
    std::string file1 = filepath.substr(last_slash_index + 1, filepath.size() - last_slash_index);

    // User input for the file content
    std::string file_content;
    std::string input_line;
    while (std::getline(std::cin, input_line) && !input_line.empty()) {
        file_content += input_line + "\n";
    }

    // Check if filename is too long
    if (file1.size() > 55) {
        printf("Error: Filename is too long (more than 55 chars)\n");
        return -1;
    }

    // Check if the file already exists
    int8_t file_index = fetch_file_index_by_path(filepath, contents_current_cwd);
    if (file_index != -1) {
        printf("Error: File already exists\n");
        return -1;
    }

    // Calculate required blocks
    size_t block_size = BLOCK_SIZE;
    size_t content_size = file_content.size();
    size_t blocks_needed = (content_size + block_size - 1) / block_size;

    // Vector to keep track of allocated blocks
    std::vector<int> allocated_blocks;

    // Allocate blocks and write content
    const char* content_ptr = file_content.c_str();
    for (size_t b = 0; b < blocks_needed; ++b) {
        int free_block_index = fetch_free_block_index();
        if (free_block_index == -1) {
            printf("Error: Not enough space on disk\n");

            // Roll back allocations
            for (int blk : allocated_blocks) {
                fat[blk] = 0; // Mark block as free
            }
            disk.write(FAT_BLOCK, (uint8_t*)fat);
            return -1;
        }

        // Write a block of data to the disk
        disk.write(free_block_index, (uint8_t*)content_ptr);
        content_ptr += block_size;

        // Update FAT chain
        if (!allocated_blocks.empty()) {
            fat[allocated_blocks.back()] = free_block_index;
        }
        allocated_blocks.push_back(free_block_index);
    }

    // Mark the last block as EOF
    fat[allocated_blocks.back()] = FAT_EOF;
    disk.write(FAT_BLOCK, (uint8_t*)fat);

    // Set up the metadata for the new file
    dir_entry new_file;
    std::strncpy(new_file.file_name, file1.c_str(), sizeof(new_file.file_name) - 1);
    new_file.file_name[sizeof(new_file.file_name) - 1] = '\0';
    new_file.size = content_size;
    new_file.first_blk = allocated_blocks.front();
    new_file.type = TYPE_FILE;
    new_file.access_rights = READ | WRITE | EXECUTE;

    // Update directory entry in the current working directory
    for (int i = 0; i < DIR_ENTRY_SIZE; i++) {
        if (contents_current_cwd[i].file_name[0] == '\0') {
            contents_current_cwd[i] = new_file;
            break;
        }
    }

    // Write changes to disk
    if (current_working_directory == "/") { // If root directory
        disk.write(ROOT_BLOCK, (uint8_t*)contents_current_cwd);
    } else { // If not root directory
        disk.write(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);
    }

    // Return with success
    return 0;
}


// cat <filepath> reads the content of a file and prints it on the screen
/*
PDF description:
Reads the contents of the file filename and prints it on the screen.
*/
int FS::cat(std::string filepath) {
    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract file directory and name
    std::string file_dir_path = filepath.substr(0, filepath.find_last_of('/'));
    std::string file_name = filepath.substr(filepath.find_last_of('/') + 1);

    // Change to file directory
    if (file_dir_path != file_name) {
        int change_cwd_result = change_cwd_using_path(file_dir_path);
        if (change_cwd_result == -1) {
            printf("Error: File directory not found\n");
            return -1;
        }
    }

    // Check if the file exists
    int file_index = fetch_file_index_by_path(filepath, contents_current_cwd);
    if (file_index == -1) {
        printf("Error: File not found\n");
        return -1;
    }

    // Check if the file is a directory and not a file
    if (contents_current_cwd[file_index].type != TYPE_FILE) {
        printf("Error: Can only cat file\n");
        return -1;
    }

    // Check if the file has read permissions
    if ((contents_current_cwd[file_index].access_rights & READ) == 0) {
        printf("Error: No read permissions\n");
        return -1;
    }

    // Read the file content from the disk and print it
    int file_block_index = contents_current_cwd[file_index].first_blk;
    while (file_block_index != FAT_EOF) {
        uint8_t file_content_buffer[BLOCK_SIZE];
        disk.read(file_block_index, file_content_buffer);
        std::cout << (char*)file_content_buffer;
        file_block_index = fat[file_block_index];
    }

    // Restore the original working directory
    change_cwd_using_path(original_cwd);

    // Return with success
    return 0;
}


// ls lists the content in the currect directory (files and sub-directories)
/*
PDF description:
Lists the contents of the current directory (in this case the root directory) on the form:
name size
file1 37
file2 128
file3 9432
*/
int FS::ls() {
    // Display table headers
    printf("name              type       accessrights     size\n");
    // Iterate through entries in the current directory
    for (int entry_idx = 0; entry_idx < DIR_ENTRY_SIZE; ++entry_idx) {
        const auto& entry = contents_current_cwd[entry_idx];
        
        // Check if the directory entry is valid
        if (entry.file_name[0] != '\0') {
            // Print the file/directory name
            std::cout << entry.file_name;
            int padding = 18 - std::strlen(entry.file_name); // Adjust padding to align columns
            if (padding > 0) std::cout << std::string(padding, ' ');

            // Print the entry type (dir or file)
            std::cout << ((entry.type == TYPE_DIR) ? "dir" : "file") << "       ";

            // Print the access rights (r, w, x)
            std::cout << ((entry.access_rights & READ) ? "r" : "-")
                    << ((entry.access_rights & WRITE) ? "w" : "-")
                    << ((entry.access_rights & EXECUTE) ? "x" : "-") << "           ";

            // Print size or "-" for directories
            if (entry.type == TYPE_DIR)
                std::cout << "-\n";
            else
                std::cout << entry.size << "\n";
        }
    }

    return 0; // Indicate success
}


// cp <sourcepath> <destpath> makes an exact copy of the file
// <sourcepath> to a new file <destpath>
/*
PDF description:
Makes an exact copy of the file sourcefilename to a new file destfilename.
If destfilename already exists, you shall give an error message (i.e., noclobber semantics). Existing files shall never
be overwritten.
*/
int FS::cp(std::string source_path, std::string dest_path) {
    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract source file directory and name
    std::string source_dir_path = source_path.substr(0, source_path.find_last_of('/'));
    std::string file1_name = source_path.substr(source_path.find_last_of('/') + 1);

    // Change to source directory if not present in the current working directory
    // this should be when source path and source file name are not the same
    if (source_dir_path != file1_name || source_dir_path[0] == '.') {
        if (change_cwd_using_path(source_dir_path) == -1) {
            printf("Error: Source directory not found\n");
            return -1;
        }
    }

    // Fetch source file
    int file1_index = fetch_file_index_by_path(file1_name, contents_current_cwd);
    if (file1_index == -1 || contents_current_cwd[file1_index].type != TYPE_FILE) {
        printf("Error: Source file not found or is not a file\n");
        change_cwd_using_path(original_cwd);
        return -1;
    }

    dir_entry file1 = contents_current_cwd[file1_index];

    // Extract destination directory and file name
    std::string dest_dir_path = dest_path.substr(0, dest_path.find_last_of('/'));
    std::string file2_name = dest_path.substr(dest_path.find_last_of('/') + 1);

    // Var to know if destination is a directory so we can use the source file name
    bool dest_is_dir = false;

    // Pre check if dest is in the current working directory and is a dir
    // to override that path and name is same when dest is a dir but doesnt have "/"
    int dest_index = fetch_file_index_by_path(dest_path, contents_current_cwd);
    if (dest_index != -1 && contents_current_cwd[dest_index].type == TYPE_DIR) {
        if (contents_current_cwd[dest_index].file_name == file2_name) {
            dest_is_dir = true;
        }
    }

    // Change to destination directory if not present in the current working directory
    // this should be when dest path and dest file name are not the same
    if (dest_dir_path != file2_name || dest_dir_path[0] == '.' || dest_is_dir) {
        int change_cwd_result = change_cwd_using_path(dest_path);
        if (change_cwd_result == -1) {
            printf("Error: Destination directory not found\n");
            change_cwd_using_path(original_cwd);
            return -1;
        }
        else if (change_cwd_result != -2) {
            dest_is_dir = true;
        }
    }

    // Check if destination file already exists
    int file2_index = fetch_file_index_by_path(file2_name, contents_current_cwd);
    if (file2_index != -1 && contents_current_cwd[file2_index].type == TYPE_FILE) {
        printf("Error: Destination file already exists\n");
        change_cwd_using_path(original_cwd);
        return -1;
    }

    // Find an empty entry for the new file
    int empty_entry_index = fetch_empty_entry_index();
    if (empty_entry_index < 0) {
        printf("Error: No space for new file in destination directory\n");
        change_cwd_using_path(original_cwd);
        return -1;
    }

    // Create a new file entry in the destination
    dir_entry new_file = {};

    // If the destionation file is a directory, use the source file name and if it is a file, use the dest file name. If its ".." use the source file name
    if (file2_name == "..") {
        // Copy the source file name to the new file entry
        std::strncpy(new_file.file_name, file1_name.c_str(), sizeof(new_file.file_name) - 1);
    }
    else if (dest_is_dir) {
        // Copy the source file name to the new file entry
        std::strncpy(new_file.file_name, file1_name.c_str(), sizeof(new_file.file_name) - 1);
    }
    else {
        // Copy the dest file name to the new file entry
        std::strncpy(new_file.file_name, file2_name.c_str(), sizeof(new_file.file_name) - 1);
    }


    new_file.file_name[sizeof(new_file.file_name) - 1] = '\0';
    new_file.size = file1.size;
    new_file.first_blk = file1.first_blk;
    new_file.type = TYPE_FILE;
    new_file.access_rights = file1.access_rights;

    // Write the new entry to disk
    contents_current_cwd[empty_entry_index] = new_file;
    disk.write(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);

    // Restore the original working directory
    change_cwd_using_path(original_cwd);

    // Return with success
    return 0;
}


// mv <sourcepath> <destpath> renames the file <sourcepath> to the name <destpath>,
// or moves the file <sourcepath> to the directory <destpath> (if dest is a directory)
/*
PDF description:
Renames the file sourcefilename to the name destfilename.
If destfilename already exists, you shall give an error message (i.e., noclobber semantics). Existing files shall never
be overwritten.
Note that it is not ok to first copy the source file using cp and then remove the source file using rm.
*/
int FS::mv(std::string source_path, std::string dest_path) {
    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract source directory and file name
    std::string source_dir_path = source_path.substr(0, source_path.find_last_of('/'));
    std::string file1_name = source_path.substr(source_path.find_last_of('/') + 1);

    // Extract dest directory and file name
    std::string dest_dir_path = dest_path.substr(0, dest_path.find_last_of('/'));
    std::string file2_name = dest_path.substr(dest_path.find_last_of('/') + 1);

    // Change to source directory
    if (change_cwd_using_path(source_path) == -1) {
        printf("Error: Source directory not found\n");
        return -1;
    }

    // Fetch source file
    int file1_index = fetch_file_index_by_path(file1_name, contents_current_cwd);
    if (file1_index == -1 || contents_current_cwd[file1_index].type != TYPE_FILE) {
        printf("Error: Source file not found or is not a file\n");
        change_cwd_using_path(original_cwd);
        return -1;
    }
    // Save metadata
    dir_entry file1 = contents_current_cwd[file1_index];

    // Var to know if destination is a directory so we can use the source file name
    bool dest_is_dir = false;

    // Pre check if dest is in the current working directory and is a dir
    // to override that path and name is same when dest is a dir but doesnt have "/"
    int dest_index = fetch_file_index_by_path(dest_path, contents_current_cwd);
    if (dest_index != -1 && contents_current_cwd[dest_index].type == TYPE_DIR) {
        if (contents_current_cwd[dest_index].file_name == file2_name) {
            dest_is_dir = true;
        }
    }

    // Change to destination directory if not present in the current working directory
    // this should be when dest path and dest file name are not the same
    if (dest_dir_path != file2_name || dest_dir_path[0] == '.' || dest_is_dir || dest_path == "/") {
        int change_cwd_result = change_cwd_using_path(dest_path);
        if (change_cwd_result == -1) {
            printf("Error: Destination directory not found\n");
            change_cwd_using_path(original_cwd);
            return -1;
        }
        else if (change_cwd_result != -2) {
            dest_is_dir = true;
        }
    }

    // Check if destination file already exists
    if (dest_is_dir == false) {
        int file2_index = fetch_file_index_by_path(file2_name, contents_current_cwd);
        if (file2_index != -1 && contents_current_cwd[file2_index].type == TYPE_FILE) {
            printf("Error: Destination file already exists\n");
            change_cwd_using_path(original_cwd);
            return -1;
        }
    }
    
    // Find an empty entry for the new file
    int dest_free_index = fetch_empty_entry_index();
    if (dest_free_index < 0) {
        printf("Error: No space for new file in destination directory\n");
        change_cwd_using_path(original_cwd);
        return -1;
    }

    // If dest is a directory, use the source file name
    if (dest_is_dir) {
        // Copy the source file name to the new file entry
        std::strncpy(file1.file_name, file1_name.c_str(), sizeof(file1.file_name) - 1);
    }
    else {
        // Copy the dest file name to the new file entry
        std::strncpy(file1.file_name, file2_name.c_str(), sizeof(file1.file_name) - 1);
    }

    // Put metadata in the new spot
    contents_current_cwd[dest_free_index] = file1;

    // Write new to disk
    disk.write(parent_folder_indexes[index_current_cwd], (uint8_t *)contents_current_cwd);

    // Go back to sourcefile location and remove it
    change_cwd_using_path(original_cwd);
    change_cwd_using_path(source_dir_path);
    
    // Remove source file metadata
    memset(&contents_current_cwd[file1_index], 0, sizeof(dir_entry));
    disk.write(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);

    // Restore the original working directory
    change_cwd_using_path(original_cwd);

    // Return with success
    return 0;
}


// rm <filepath> removes / deletes the file <filepath>
/*
PDF description:
Deletes the file filename, i.e., removes its directory entry and marks the corresponding disk blocks as free.
*/
int FS::rm(std::string filepath) {
    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract file directory and name
    std::string file_dir_path = filepath.substr(0, filepath.find_last_of('/'));
    std::string file_name = filepath.substr(filepath.find_last_of('/') + 1);

    // Change to file directory
    if (file_dir_path != file_name) {
        int change_cwd_result = change_cwd_using_path(filepath);
        if (change_cwd_result == -1) {
            printf("Error: File directory not found\n");
            return -1;
        }
    }

    // Check if the file exists
    int file_index = fetch_file_index_by_path(file_name, contents_current_cwd);
    if (file_index == -1) {
        printf("Error: File not found\n");
        return -1;
    }

    // Check if the file is a directory and not a file
    if (contents_current_cwd[file_index].type != TYPE_FILE) {
        printf("Error: Can only remove file\n");
        return -1;
    }

    // Free the blocks in the FAT
    int file_block_index = contents_current_cwd[file_index].first_blk;
    while (file_block_index != FAT_EOF) {
        int next_block_index = fat[file_block_index];
        fat[file_block_index] = FAT_FREE;
        file_block_index = next_block_index;
    }

    // Remove the file entry from the directory
    memset(&contents_current_cwd[file_index], 0, sizeof(dir_entry));

    // Write the updated directory to disk
    disk.write(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);

    // Return with success
    return 0;
}


// append <filepath1> <filepath2> appends the contents of file <filepath1> to
// the end of file <filepath2>. The file <filepath1> is unchanged.
/*
PDF description:
Appends the contents of file filename1 to the end of file filename2. The file filename1 is unchanged. You do not need
to cover the special case when appending a file to itself, i.e., ’append <filename1> <filename1>’.
*/
int FS::append(std::string filepath1, std::string filepath2) {
    // Fail fast if both files are the same
    if (filepath1 == filepath2) {
        printf("Error: Cannot append a file to itself\n");
        return -1;
    }

    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract file1 directory and file name
    std::string file1_dir_path = filepath1.substr(0, filepath1.find_last_of('/'));
    std::string file1_name = filepath1.substr(filepath1.find_last_of('/') + 1);

    // Change to file1 directory
    if (file1_dir_path != file1_name) {
        int change_cwd_result = change_cwd_using_path(file1_dir_path);
        if (change_cwd_result == -1) {
            change_cwd_using_path(original_cwd);
            printf("Error: Source directory not found\n");
            return -1;
        }
    }

    // Check if file 1 exists and is a file
    int file1_index = fetch_file_index_by_path(filepath1, contents_current_cwd);
    if (file1_index == -1 || contents_current_cwd[file1_index].type != TYPE_FILE) {
        printf("Error: Source file not found or is not a file\n");
        return -1;
    }

    // Save file1 for later use
    dir_entry file1 = contents_current_cwd[file1_index];

    // Extract file2 directory and file name
    std::string file2_dir_path = filepath2.substr(0, filepath2.find_last_of('/'));
    std::string file2_name = filepath2.substr(filepath2.find_last_of('/') + 1);

    // Change to file2 directory
    if (file2_dir_path != file2_name) {
        int change_cwd_result = change_cwd_using_path(file2_dir_path);
        if (change_cwd_result == -1) {
            change_cwd_using_path(original_cwd);
            printf("Error: Destination directory not found\n");
            return -1;
        }
    }

    // Check if file 2 exists and is a file
    int file2_index = fetch_file_index_by_path(filepath2, contents_current_cwd);
    if (file2_index == -1 || contents_current_cwd[file2_index].type != TYPE_FILE) {
        printf("Error: Destination file not found or is not a file\n");
        return -1;
    }

    // Save file2 for later use
    dir_entry file2 = contents_current_cwd[file2_index];

    // Check read permissions for source file
    if ((file1.access_rights & READ) == 0) {
        printf("Error: No read permissions for source file\n");
        return -1;
    }

    // Check write permissions for destination file
    if ((file2.access_rights & WRITE) == 0) {
        printf("Error: No write permissions for destination file\n");
        return -1;
    }

    // Read file2's content first
    std::string file2_content;
    int curr_block = file2.first_blk;
    while (curr_block != FAT_EOF) {
        uint8_t buffer[BLOCK_SIZE];
        disk.read(curr_block, buffer);
        int bytes_to_read = std::min(BLOCK_SIZE, (int)(file2.size - file2_content.size()));
        file2_content.append((char*)buffer, bytes_to_read);
        curr_block = fat[curr_block];
    }

    // Read file1's content
    std::string file1_content;
    int file1_block_index = file1.first_blk;
    while (file1_block_index != FAT_EOF) {
        uint8_t file1_content_buffer[BLOCK_SIZE];
        disk.read(file1_block_index, file1_content_buffer);
        int bytes_to_read = std::min(BLOCK_SIZE, (int)(file1.size - file1_content.size()));
        file1_content.append((char*)file1_content_buffer, bytes_to_read);
        file1_block_index = fat[file1_block_index];
    }

    // Combine the contents
    std::string combined_content = file2_content + file1_content;

    // Clear the existing FAT chain for file2
    curr_block = file2.first_blk;
    while (curr_block != FAT_EOF) {
        int next_block = fat[curr_block];
        fat[curr_block] = FAT_FREE;
        curr_block = next_block;
    }

    // Write the combined content to new blocks
    file2.first_blk = FAT_EOF;
    int prev_block = FAT_EOF;
    for (int i = 0; i < combined_content.size(); i += BLOCK_SIZE) {
        int free_block_index = fetch_free_block_index();
        if (free_block_index < 0) {
            printf("Error: No free block found\n");
            return -1;
        }

        std::string block_content = combined_content.substr(i, BLOCK_SIZE);
        disk.write(free_block_index, (uint8_t*)block_content.c_str());

        if (prev_block == FAT_EOF) {
            file2.first_blk = free_block_index;
        } 
        else {
            fat[prev_block] = free_block_index;
        }
        fat[free_block_index] = FAT_EOF;
        prev_block = free_block_index;
    }

    // Update the size of file2
    file2.size += file1.size;

    // Write the updated file2 to the disk
    contents_current_cwd[file2_index] = file2;

    // Write the updated directory to the disk
    disk.write(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);

    // Restore the original working directory
    change_cwd_using_path(original_cwd);
    
    // Return with success
    return 0;
}


// mkdir <dirpath> creates a new sub-directory with the name <dirpath>
// in the current directory
/*
PDF description:
Creates a new sub-directory named dirname in the current directory. When a sub-directory is created, it should always
contain a "sub-directory" named ’..’ which refers to the parent directory (i.e., the directory where we created the
sub-directory).
Change the ls command so it also prints the type, i.e., if a directory entry is a regular file or a directory. The data
structure struct dir_entry already has support for this, see Listing 1 and the file fs.h.
*/
int FS::mkdir(std::string dirpath)
{

    // Check if the directory already exists
    int8_t file_index = fetch_file_index_by_path(dirpath, contents_current_cwd);
    if (file_index != -1) {
        std::cout << "Directory already exists\n";
        return -1;
    }

    // Save the original state
    std::string original_cwd = current_working_directory;
    int8_t original_parent_folder_indexes[64];
    int8_t original_index = index_current_cwd;
    std::memcpy(original_parent_folder_indexes, parent_folder_indexes, sizeof(parent_folder_indexes));

    size_t i = 0;
    while (i < dirpath.size()) {
        size_t j = dirpath.find('/', i);
        if (j == std::string::npos) j = dirpath.size();

        std::string dirname = dirpath.substr(i, j - i);
        i = j + 1;

        if (dirname.empty() || dirname == "..") {
            change_cwd_using_path(dirname);
            continue;
        }

        if (dirname.size() > 55) {
            std::cout << "Directory name exceeds 55 characters\n";
            return -1;
        }

        int8_t file_index = fetch_file_index_by_path(dirname, contents_current_cwd);
        if (file_index >= 0) {
            change_cwd_using_path(dirname);
            continue;
        }

        int free_block = fetch_free_block_index();
        if (free_block < 0) {
            std::cout << "No free blocks available\n";
            return -1;
        }

        // Update FAT and create the directory entry
        fat[free_block] = FAT_EOF;
        disk.write(FAT_BLOCK, (uint8_t*)fat);

        dir_entry new_dir = {};
        std::strncpy(new_dir.file_name, dirname.c_str(), sizeof(new_dir.file_name) - 1);
        new_dir.file_name[sizeof(new_dir.file_name) - 1] = '\0';
        new_dir.first_blk = free_block;
        new_dir.type = TYPE_DIR;
        new_dir.access_rights = READ | WRITE | EXECUTE;

        // Find an empty slot in the current directory
        for (int k = 0; k < DIR_ENTRY_SIZE; ++k) {
            if (contents_current_cwd[k].file_name[0] == '\0') {
                contents_current_cwd[k] = new_dir;
                break;
            }
        }

        disk.write(current_working_directory == "/" ? ROOT_BLOCK : parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);

        // Initialize the new directory block
        dir_entry empty_dir[DIR_ENTRY_SIZE] = {};
        disk.write(free_block, (uint8_t*)empty_dir);

        change_cwd_using_path(dirname);
    }

    // Restore original state
    current_working_directory = original_cwd;
    index_current_cwd = original_index;
    std::memcpy(parent_folder_indexes, original_parent_folder_indexes, sizeof(parent_folder_indexes));
    disk.read(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);

    // Return with success
    return 0;
}


// cd <dirpath> changes the current (working) directory to the directory named <dirpath>
/*
PDF description:
Changes the current (working) directory to the directory named dirname. Remember to cover the case when dirname
is ’..’
*/
int FS::cd(std::string dirpath) {
    // Save the original state
    std::string original_cwd = current_working_directory;
    dir_entry original_direct[DIR_ENTRY_SIZE];
    int8_t original_parent_folder_indexes[64];
    int8_t original_index = index_current_cwd;

    std::memcpy(original_direct, contents_current_cwd, sizeof(contents_current_cwd));
    std::memcpy(original_parent_folder_indexes, parent_folder_indexes, sizeof(parent_folder_indexes));

    int8_t result = change_cwd_using_path(dirpath);

    // Restore state only if navigation fails
    if (result < 0) {
        current_working_directory = original_cwd;
        index_current_cwd = original_index;
        std::memcpy(contents_current_cwd, original_direct, sizeof(contents_current_cwd));
        std::memcpy(parent_folder_indexes, original_parent_folder_indexes, sizeof(parent_folder_indexes));

        if (result == -2) {
            std::cout << dirpath << " is not a directory\n";
        } 
        else { // (result == -1)
            std::cout << dirpath << " not found\n";
        }

        return -1;
    }

    return 0;
}


// pwd prints the full path, i.e., from the root directory, to the current
// directory, including the currect directory name
/*
PDF description:
Prints the full path, i.e., from the root directory, to the current directory (including the current directory name). A
path to a directory is written as ’/dir1/subdir2’, and the root directory should be ’/’
*/
int FS::pwd() {
    // Print the current working directory
    std::cout << current_working_directory << "\n";

    // Return with success
    return 0;
}

// chmod <accessrights> <filepath> changes the access rights for the
// file <filepath> to <accessrights>.
/*
PDF description:
Changes the access rights for the file <filepath> to <accessrights>.
*/
int FS::chmod(std::string accessrights, std::string filepath) {
    // Save the current working directory
    std::string original_cwd = current_working_directory;

    // Extract file directory and name
    std::string file_dir_path = filepath.substr(0, filepath.find_last_of('/'));
    std::string file_name = filepath.substr(filepath.find_last_of('/') + 1);

    // Change to file directory
    if (file_dir_path != file_name || file_dir_path[0] == '.') {
        int change_cwd_result = change_cwd_using_path(file_dir_path);
        if (change_cwd_result == -1) {
            printf("Error: File directory not found\n");
            return -1;
        }
    }

    // Fetch file index (if it exists)
    int file_index = fetch_file_index_by_path(filepath, contents_current_cwd);
    if (file_index == -1) {
        printf("Error: File not found\n");
        return -1;
    }

    // Translate numeric access rights to string
    if (accessrights == "0") {
        accessrights = "---";
    } 
    else if (accessrights == "1") {
        accessrights = "--x";
    } 
    else if (accessrights == "2") {
        accessrights = "-w-";
    } 
    else if (accessrights == "3") {
        accessrights = "-wx";
    } 
    else if (accessrights == "4") {
        accessrights = "r--";
    } 
    else if (accessrights == "5") {
        accessrights = "r-x";
    } 
    else if (accessrights == "6") {
        accessrights = "rw-";
    } 
    else if (accessrights == "7") {
        accessrights = "rwx";
    }

    // Set the permissions for the file
    int set_permissions_result = set_permissions(&contents_current_cwd[file_index], accessrights);
    if (set_permissions_result == -1) {
        printf("Error: Could not set permissions for file\n");
        return -1;
    }

    // Write the updated file to the disk
    if (index_current_cwd == ROOT_BLOCK) { // Special case for root directory
        disk.write(ROOT_BLOCK, (uint8_t*)contents_current_cwd);
    }
    else {
        disk.write(parent_folder_indexes[index_current_cwd], (uint8_t*)contents_current_cwd);
    }

    // Restore the original working directory
    change_cwd_using_path(original_cwd);

    // Return with success
    return 0;
}

