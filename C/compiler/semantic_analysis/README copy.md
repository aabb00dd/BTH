# Custom File System in C++

This project is a simplified file system simulation implemented in C++, built on top of a virtual block-based disk. It features a custom implementation of FAT (File Allocation Table), directory management, file operations, and access permissions.

## Features

- **FAT-based storage** with dynamic block allocation
- **Hierarchical file system** with directory and subdirectory support
- **Current working directory (CWD)** logic
- **UNIX-like commands** implemented:
  - `format()` – Wipe and reinitialize the filesystem
  - `ls()` – List contents of current directory
  - `mkdir(dirpath)` – Create subdirectories
  - `cd(dirpath)` – Change working directory
  - `pwd()` – Print full current path
  - `create(filepath)` – Create a new file
  - `cat(filepath)` – Display file contents
  - `cp(src, dest)` – Copy files
  - `mv(src, dest)` – Move or rename files
  - `rm(filepath)` – Remove a file
  - `append(src, dest)` – Append file content
  - `chmod(rights, filepath)` – Set file access permissions

## File System Design

- **Blocks:** Fixed-size disk blocks (`BLOCK_SIZE`)
- **FAT Table:** Tracks free, allocated, and EOF blocks
- **Root Directory:** Block 0 is always the root directory