// Symbolic constants for memory-mapped I/O addresses
.equ UART_BASE, 0xff201000 // Base address for UART
.equ UART_DATA_REGISTER_ADDRESS, 0xff201000 // Address for UART data register
.equ UART_CONTROL_REGISTER_ADDRESS, 0xff201004 // Address for UART control register
.equ LEDS_BASE, 0xff200000 // Base address for LEDs
.equ SWITCHES_BASE, 0xff200040 // Base address for switches
.equ PUSH_BUTTONS_BASE, 0xff200050 // Base address for push buttons
.equ DISPLAYS_BASE_1, 0xff200020 // Base address for the first display


.data	
// Array of hexadecimal values for 7-segment display (0-9, A-F)
hex_values: .word 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71   

// Stack base addresses for different processor modes
.equ SVC_MODE_STACK_BASE, 0x3FFFFFFF - 3 // SVC mode stack base, top of DDR3 memory minus 3
.equ IRQ_MODE_STACK_BASE, 0xFFFFFFFF - 3 // IRQ mode stack base, top of A9 on-chip memory minus 3

// GIC (Generic Interrupt Controller) base addresses
.equ GIC_CPU_INTERFACE_BASE, 0xFFFEC100 // Base address for GIC CPU interface
.equ GIC_DISTRIBUTOR_BASE, 0xFFFED000 // Base address for GIC distributor


.text
// Interrupt vector table setup
.org 0x00  
    B _start    // Reset vector: jumps to start of program
    B SERVICE_UND // Undefined instruction vector
    B SERVICE_SVC // Supervisor call vector
    B SERVICE_ABT_INST // Prefetch abort vector
    B SERVICE_ABT_DATA // Data abort vector
    .word 0 // Placeholder for unused vector
    B SERVICE_IRQ // IRQ vector
    B SERVICE_FIQ // FIQ vector


.global _start
_start:

    // Set up stack pointers for IRQ and SVC modes
    MSR CPSR_c, #0b11010010 // Switch to IRQ mode, disable interrupts
    LDR SP, =IRQ_MODE_STACK_BASE // Initialize IRQ mode stack pointer
    MSR CPSR_c, #0b11010011 // Switch to SVC mode, disable interrupts
    LDR SP, =SVC_MODE_STACK_BASE // Initialize SVC mode stack pointer

    // Configure GIC
    MOV R0, #73  // Example interrupt ID, not matching comment
    BL CONFIG_GIC // Call function to configure GIC

    // Initialize UART for receive interrupts
    LDR R0, =PUSH_BUTTONS_BASE // Load base address of push buttons
    ADD R0, R0, #0x8 // Calculate address for interrupt enable register
    MOV R1, #0x3 // Value to enable interrupts
    STR R1, [R0] // Enable interrupts for push buttons

    // Change processor mode and enable interrupts
    MSR CPSR_c, #0b01010011  // Switch to SVC mode, enable IRQ

    // Initialize display with 0
    MOV R0, #0x3f // Hex value for 0 on 7-segment display
    BL write_display // Call function to write to display
    MOV R0, #0 // Reset R0 to 0

    // Main program loop
    _main_loop:
        BL read_terminal // Poll the terminal for input
        BL plus_minus_handler // Handle plus/minus input
        B _main_loop // Loop indefinitely


// Function to calculate modulo 16 of a value
modulo_16:
    PUSH {lr}
    MOV R1, #15 // Modulo value (16-1)
    AND R0, R0, R1 // R0 = R0 mod 16
    POP {lr}
    BX lr


// Function to read input from terminal
read_terminal:
    LDR r0, =UART_DATA_REGISTER_ADDRESS // Load the address of the UART data register into r0
    LDR r1, [r0] // Read the data register into r1
    ANDS r2, r1, #0x8000 // Check if data is valid, bit 15
    BEQ _not_valid // If not valid, branch to _not_valid
    AND r0, r1, #0x00FF // Mask the read data to get bits 0-7
    BX lr
_not_valid:
    B read_terminal // Branch to read_terminal if data not valid


// Function to write to the 7-segment display
write_display:
    PUSH {lr}
    LDR r1, =DISPLAYS_BASE_1 // Load the base address of the display into r1
    STR r0, [r1] // Write the value in r0 to the display
    POP {lr}
    BX lr


// Function to find the address of a hex value in the array
find_hex_address:
    PUSH {lr}
    LDR r1, =hex_values // Load the address of the hex_values array into r1
_loop:
    LDR r2, [r1] // Load the current hex value into r2
    CMP r0, r2 // Compare the target value in r0 with the current hex value in r2
    BEQ _end_loop // If equal, branch to _end_loop
    ADD r1, r1, #4 // Move to the next hex value in the array
    B _loop // Branch to _loop to continue searching
_end_loop:
    MOV r0, r1 // Move the address of the found hex value into r0
    POP {lr}
    BX lr


// Function to read the current display value
read_display_value:
    PUSH {lr}
    LDR r1, =DISPLAYS_BASE_1 // Load the base address of the display into r1
    LDR r0, [r1] // Read the display value into r0
    POP {lr}
    BX lr


// Function to handle plus/minus input
plus_minus_handler:
    PUSH {lr}
    CMP r0, #0x2B // Compare the input value with '+'
    BEQ increase_display_number // If '+', branch to increase_display_number
    CMP r0, #0x2D // Compare the input value with '-'
    BEQ decrease_display_number // If '-', branch to decrease_display_number
    POP {lr}
    BX lr


// Function to increase the display number
increase_display_number:
    PUSH {lr}
    BL read_display_value // Call read_display_value to get the current display value
    CMP r0, #0x71 // Compare the current display value with the last hex value
    BEQ _reset_hex_array_min // If equal, branch to _reset_hex_array_min
    BL find_hex_address // Call find_hex_address to get the address of the current display value in the array
    ADD r0, #4 // Move to the next value in the array
    LDR r0, [r0] // Load the next value from the array
    B _exit_increase_display_number
_reset_hex_array_min:
    MOV r0, #0x3F // Reset the display value to the first hex value
_exit_increase_display_number:
    BL write_display // Call write_display to update the display
    POP {lr}
    BX lr


// Function to decrease the display number
decrease_display_number:
    PUSH {lr}
    BL read_display_value // Call read_display_value to get the current display value
    CMP r0, #0x3F // Compare the current display value with the first hex value
    BEQ _reset_hex_array_max // If equal, branch to _reset_hex_array_max
    BL find_hex_address // Call find_hex_address to get the address of the current display value in the array
    SUB r0, #4 // Move to the previous value in the array
    LDR r0, [r0] // Load the previous value from the array
    B _exit_decrease_display_number
_reset_hex_array_max:
    MOV r0, #0x71 // Reset the display value to the last hex value
_exit_decrease_display_number:
    BL write_display // Call write_display to update the display
    POP {lr}
    BX lr


// Interrupt service routines
SERVICE_IRQ:
    PUSH {R0-R7, LR}
    LDR R4, =GIC_CPU_INTERFACE_BASE // Load the base address of the GIC CPU interface
    LDR R5, [R4, #0x0C] // Read the current interrupt ID from ICCIAR


// Check the interrupt ID to determine the source
CHECK_PUSH_INTERRUPT:
    CMP R5, #73 // Compare the interrupt ID with the PUSH interrupt ID
    BNE SERVICE_IRQ_DONE // If not a PUSH interrupt, branch to SERVICE_IRQ_DONE
    BL PUSH_INTERRUPT_HANDLER // Call the push buttons interrupt handler


// Acknowledge the interrupt and return
SERVICE_IRQ_DONE:
    STR R5, [R4, #0x10] // Write to ICCEOIR to acknowledge the interrupt
    POP {R0-R7, LR}
    SUBS PC, LR, #4 // Return from interrupt


// Push button interrupt handler
PUSH_INTERRUPT_HANDLER:
    PUSH {LR}
    LDR R0, =PUSH_BUTTONS_BASE // Load the base address of the push buttons
    ADD R0, R0, #0xC // Offset to the push buttons
    LDR R1, [R0] // Read the push buttons
    STR R1, [R0] // Write to the push buttons to clear the interrupt
    CMP R1, #0x1 // Check if push button 0 is pressed
    BLEQ increase_display_number // If push button 0 is pressed, call increase_display_number
    CMP R1, #0x2 // Check if push button 1 is pressed
    BLEQ decrease_display_number // If push button 1 is pressed, call decrease_display_number
    POP {lr}
    BX lr

SERVICE_UND:
    B SERVICE_UND // Branch to itself, undefined instruction handler

SERVICE_SVC:
    B SERVICE_SVC // Branch to itself, software interrupt handler

SERVICE_ABT_DATA:
    B SERVICE_ABT_DATA // Branch to itself, data abort handler

SERVICE_ABT_INST:
    B SERVICE_ABT_INST // Branch to itself, instruction abort handler

SERVICE_FIQ:
    B SERVICE_FIQ // Branch to itself, fast interrupt request handler


CONFIG_GIC:
    PUSH {LR}
    MOV R1, #1 // Set R1 to target CPU0 specifically
    BL CONFIG_INTERRUPT
    LDR R0, =GIC_CPU_INTERFACE_BASE // Load base address of GIC CPU Interface
    LDR R1, =0xFFFF // Set mask to enable all priority levels
    STR R1, [R0, #0x04] // Store the mask value in ICCPMR
    MOV R1, #1 // Prepare to enable the CPU Interface
    STR R1, [R0] // Enable the CPU Interface
    LDR R0, =GIC_DISTRIBUTOR_BASE // Load base address of GIC Distributor
    STR R1, [R0] // Enable the Distributor
    POP {PC}


CONFIG_INTERRUPT:
    PUSH {R4-R5, LR}
    LSR R4, R0, #3 // Calculate register offset for ICDISER
    BIC R4, R4, #3 // Align offset to 4-byte boundary
    LDR R2, =0xFFFED100 // Base address of ICDISERn
    ADD R4, R2, R4 // Calculate address of target ICDISER register
    AND R2, R0, #0x1F // Calculate bit position within register
    MOV R5, #1 // Set bit to enable interrupt
    LSL R2, R5, R2 // Shift enable bit into correct position
    LDR R3, [R4] // Read current value of ICDISER register
    ORR R3, R3, R2 // Set the enable bit
    STR R3, [R4] // Write back the modified value
    BIC R4, R0, #3 // Calculate register offset for ICDIPTR
    LDR R2, =0xFFFED800 // Base address of ICDIPTRn
    ADD R4, R2, R4 // Calculate address of target ICDIPTR register
    AND R2, R0, #0x3 // Calculate byte position within register
    ADD R4, R2, R4 // Adjust address to target specific byte
    STRB R1, [R4] // Write CPU target to the appropriate byte
    POP {R4-R5, PC}
    
//This assembly program for an ARM processor manages a seven-segment display, UART, and push buttons. 
//The program sets up the stack pointers for different modes, configures the Generic Interrupt Controller, 
//and initializes the UART for receive interrupts. It then enters a main loop, 
//where it reads input from the terminal and updates the seven-segment display accordingly