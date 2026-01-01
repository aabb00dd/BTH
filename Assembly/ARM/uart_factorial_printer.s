// Constants
.equ UART_BASE, 0xff201000     // UART base address
.equ UART_CONTROL_REG_OFFSET, 4 // Offset for the UART control register, used for checking write space
.equ STACK_BASE, 0x10000000    // Base address for the stack

.equ NEW_LINE, 0x0A            // ASCII code for newline character

.global _start
.text


// Function to print a null-terminated string via UART
print_string:
    PUSH {r0-r4, lr}
    LDR r2, =UART_BASE          // Load UART base address into r2
    _ps_loop:                   // Loop label for iterating through the string
        LDRB r1, [r0], #1       // Load byte from string and post-increment address
        CMP  r1, #0             // Compare byte to null terminator
        BEQ  _print_string_end  // If null terminator, jump to end of function
        _ps_busy_wait:
            LDR r4, [r2, #UART_CONTROL_REG_OFFSET] // Load UART control register
            LDR r3, =0xFFFF0000 // Mask for write space bits
            ANDS r4, r4, r3     // Apply mask to check write space
            BEQ _ps_busy_wait   // If no space, loop
        STR  r1, [r2]           // Store byte to UART data register
        B    _ps_loop           // Loop back to process next character
    _print_string_end:
        POP {r0-r4, pc}
	

// Function to perform integer division
idiv:
    MOV r2, r1                  // Move denominator to r2
    MOV r1, r0                  // Move numerator to r1
    MOV r0, #0                  // Initialize quotient to 0
    _loop_check:
        CMP r1, r2              // Compare remainder (r1) with denominator (r2)
        BHS _loop               // If remainder >= denominator, continue loop
        BX lr
    _loop:
        ADD r0, r0, #1          // Increment quotient
        SUB r1, r1, r2          // Subtract denominator from remainder
        B _loop_check
	

// Function to print a number followed by a newline
print_number:
    PUSH {r0-r5, lr}
    MOV r5, #0                  // Initialize digit counter
    _div_loop:                  // Loop label for division to isolate digits
        ADD r5, r5, #1          // Increment digit counter
        MOV r1, #10             // Set denominator to 10 for division
        BL idiv
        PUSH {r1}               // Push remainder (digit) onto stack
        CMP r0, #0              // Check if quotient is 0
        BHI _div_loop           // If not, loop back
    _print_loop:
        POP {r0}                // Pop digit from stack
        LDR r2, =UART_BASE      // Load UART base address
        ADD r0, r0, #0x30       // Convert digit to ASCII
        _print_busy_wait:
            LDR r4, [r2, #UART_CONTROL_REG_OFFSET] // Load UART control register
            LDR r3, =0xFFFF0000 // Mask for write space bits
            ANDS r4, r4, r3     // Apply mask to check write space
            BEQ _print_busy_wait // If no space, loop
        STR r0, [r2]           // Store digit to UART data register
        SUB r5, r5, #1         // Decrement digit counter
        CMP r5, #0             // Check if more digits to print
        BNE _print_loop        // If yes, loop back
    MOV r0, #NEW_LINE           // Load newline character
    STR r0, [r2]                // Print newline
    POP {r0-r5, pc}
	

// Recursive function for factorial calculation
factorial_calculator:
    PUSH {r5, lr}
    MOV r5, r0                  // Copy argument to r5 for multiplication
    CMP r0, #1                  // Check if argument is 1 (base case)
    BEQ _factorial_base         // If yes, jump to base case handling
    SUB r0, r0, #1              // Decrement argument
    BL factorial_calculator     // Recursive call with argument - 1
    MUL r0, r5, r0              // Multiply result by original argument
    B _factorial_end            // Jump to end of function
_factorial_base:
    MOV r0, #1                  // Base case: factorial of 1 is 1
_factorial_end:
    POP {r5, lr}                // Restore r5 and link register
    BX lr                       // Return from function


// Main program entry point
_start:
main:
    MOV r0, #1                  // Initialize loop counter
    _main_loop:
        CMP r0, #10             // Compare counter with 10
        BGT _main_exit          // If counter > 10, exit loop
        PUSH {r0, lr}           // Save counter and link register
        BL factorial_calculator // Calculate factorial of counter
        BL print_number         // Print result
        POP {r0, lr}            // Restore counter and link register
        ADD r0, r0, #1          // Increment counter
        B _main_loop            // Loop back
_main_exit:
        B _main_exit            // Infinite loop to prevent fall-through

//This ARM assembly program calculates and prints the factorial of numbers from 1 to 10 using the UART interface. 
//It involves several key functionalities including string printing via UART, integer division, and recursive factorial calculation.