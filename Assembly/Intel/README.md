# Buffer I/O and Test Program Assembly Code

## Files

### buffer_io.s

This file implements routines for handling input and output buffers in x86-64 assembly. Key features include:

- Input buffer management:
  - `inImage`: Reads input from standard input into the input buffer
  - `getInt`: Retrieves an integer from the input buffer
  - `getText`: Retrieves a text string from the input buffer
  - `getChar`: Retrieves a character from the input buffer
  - Position management with `getInPos` and `setInPos`

- Output buffer management:
  - `outImage`: Outputs buffer contents to standard output
  - `putInt`: Writes an integer to the output buffer
  - `putText`: Writes a text string to the output buffer
  - `putChar`: Writes a character to the output buffer
  - Position management with `getOutPos` and `setOutPos`

- Data section defines:
  - Input and output buffers (64 bytes each)
  - Position pointers for both buffers
  - Maximum buffer size constant (64)

### Mprov64.s

This is a test program that demonstrates the functionality of the buffer I/O routines. It:

1. Displays a start message using `putText` and `outImage`
2. Reads input with `inImage`
3. Processes 5 integers:
   - Reads each with `getInt`
   - Handles negative numbers
   - Maintains a running sum
   - Displays each number with `putInt` and a '+' separator
4. Displays the final sum with '=' prefix
5. Demonstrates text input/output with `getText` and `putText`
6. Displays a hardcoded number (125)
7. Shows an end message

