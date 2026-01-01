.data
    input_buffer: .space 64              # Buffer for storing input data
    output_buffer: .space 64             # Buffer for storing output data
    input_buffer_pos: .quad 0            # Position pointer in the input buffer
    output_buffer_pos: .quad 0           # Position pointer in the output buffer
    MAXPOS: .quad 64                     # Maximum size for buffers


.text
.global inImage

# Reads input from standard input into the input buffer
inImage:
    pushq $0                            
    movq $input_buffer, %rdi             # arg1 for fgets: the buffer where fgets puts the input
    movq MAXPOS, %rsi                    # arg2 for fgets: the maximum number of bytes to read
    movq stdin, %rdx                     # arg3 for fgets: standard input file descriptor
    call fgets

    # Reset input buffer position
    movq $0, input_buffer_pos

    # Terminate inImage
    pop %rax                        
    ret


.global getInt

# Retrieves an integer from the input buffer
getInt:
    # Check if input buffer is full
    cmpq $input_buffer_pos, MAXPOS
    jl getInt_not_full_or_empty

    # Check if input buffer is empty
    movq $input_buffer, %rax              # Put buffer address in rax
    addq input_buffer_pos, %rax           # Add current position
    movb (%rax), %al                      # Take first char
    cmpb $0, %al                          # Is it zero?
    jne getInt_not_full_or_empty          # If not zero, proceed without calling inImage
    call inImage                          # Call inImage to refill the buffer

    getInt_not_full_or_empty:
        movq $input_buffer, %rdi              # Input buffer start address to rdi
        addq input_buffer_pos, %rdi           # Add current position
        call atoi                             # Returns in rax: atoi gets first integer in string, if no int, returns zero
        movq %rax, %rdi                       # Save atoi output in rdi

        xorq %rax, %rax                       # Reset rax
        xorq %rcx, %rcx                       # Zero rcx (used for length)

        movq $input_buffer, %r8               # Move the input buffer memory address to r8
        addq input_buffer_pos, %r8            # Add the input buffer position to r8
        movb (%r8), %al                       # Load the current character into al
        xorq %r8, %r8                         # Reset r8

        cmpb $'-', %al 
        je handle_extra_sign                  # If we find a minus sign, handle extra sign

        cmpb $'+', %al
        je handle_extra_sign                  # If we find a plus sign, handle extra sign

        cmpb $' ', %al
        je handle_whitespace                  # If we find a whitespace, handle whitespace

        cmpb $'\n', %al
        je redo_but_call_inImage              # If we find a newline, call inImage to refill

        cmpb $'0', %al
        jle handle_true_zero                  # If we find a zero, handle true zero

        # If no numbers are found, return the value in rdi. It should be 0 since atoi didn't find any numbers
        cmpb $'0', %al
        jl handle_not_integer
        cmpb $'9', %al
        jg handle_not_integer

        # If no prefixes found, the int has the same length as the buffer space it took
        jmp getInt_calc_pos_loop
        
    handle_not_integer:
        movq %rdi, %rax                       # Return the value in rax
        ret

    handle_true_zero:
        movq $0, %rax                         # Return 0 if we find a zero
        incq input_buffer_pos                 # Increment the input buffer position
        ret

    handle_whitespace:
        incq %rcx                             # Increment the length of the string
        incq input_buffer_pos                 # Increment the input buffer position
        jmp getInt_calc_pos_loop              # Loop to calculate position

    handle_extra_sign:
        addq $1, %rcx                         # Add one length since any whitespace/sign takes one place in buffer but not as an int

    getInt_calc_pos_loop:
        movq $input_buffer, %r8               # Get the start of the input buffer
        addq input_buffer_pos, %r8            # Add the current position to the start of the input buffer
        addq %rcx, %r8                        # Add the current length of the string to the current position
        movb (%r8), %al                       # Get the character 

        # Exit loop if we find a non-integer character
        cmpb $'0', %al 
        jl exit_getInt_calc_pos_loop
        cmpb $'9', %al
        jg exit_getInt_calc_pos_loop

        incq %rcx                             # Increment the length of the string

        jmp getInt_calc_pos_loop              # Loop to calculate position

    exit_getInt_calc_pos_loop:
        movq input_buffer_pos, %r9            # Get current position of input buffer
        addq %r9, %rcx                        # Add the true position of the reverse buffer to the current position
        movq %rcx, input_buffer_pos           # Update the input buffer position to this new position
        movq %rdi, %rax                       # Return the value in rdi (put in rax)
        ret

    redo_but_call_inImage:
        incq input_buffer_pos                 # Increment the input buffer position
        call inImage                          # Call inImage to refill the buffer
        jmp getInt                            # Restart the getInt function


.global getText

# Retrieves a text string from the input buffer
getText:
    # Save the inputs to r9 and r10
    movq %rsi, %r9                        
    movq %rdi, %r10                       

    getText_loop:
        call getChar                          # Get current char. getChar handles empty/full buffer
        cmpb $0, %al                          # Is char 0? Then we are done
        je exit_getText_loop                 

        cmpq $0, %r9                          # Have we exceeded max number of chars to read?
        je exit_getText_loop                  

        movb %al, (%r10)                      # Move char to destination buffer
        incq %r10                             # Increase position 
        decq %r9                              # Decrease counter

        jmp getText_loop                      

    exit_getText_loop:
        subq %rsi, %r9                        # Calculate how many chars we read
        movq %r9, %rax                        # Return the value in rax
        ret 


.global getChar

# Retrieves a character from the input buffer
getChar:
    # Check if input buffer is empty
    movq $input_buffer, %rax              # Get buffer address
    addq input_buffer_pos, %rax           # Add current position
    movb (%rax), %al                      # Get first char
    cmpb $0, %al                          # Is it zero?
    jne getChar_not_full_or_empty         # If not zero, proceed without calling inImage

    # Check if input buffer is full
    cmpq $input_buffer_pos, MAXPOS
    jl getChar_not_full_or_empty          # If not full, proceed without calling inImage

    call inImage                          # Call inImage to refill the buffer

    getChar_not_full_or_empty:
        movq $input_buffer, %rax              # Load input buffer address into rax
        addq input_buffer_pos, %rax           # Add position onto address to get current address
        movb (%rax), %al                      # Get that char

        incq input_buffer_pos                 # Increment the input buffer position

    # Terminate getChar
    ret


.global getInPos

# Retrieves the current position in the input buffer
getInPos:
    movq input_buffer_pos, %rax           # Move the value of input_buffer_pos to rax
    ret


.global setInPos

# Sets the current position in the input buffer
setInPos:
    cmpq $0, %rdi                         # Compare the input to 0
    jle setInPos_zero                     # If it is less than or equal to zero, set to zero
    cmpq $MAXPOS, %rdi                    # Compare the input to MAXPOS
    jge setInPos_max                      # If it is greater than or equal to MAXPOS, set to MAXPOS
    movq %rdi, input_buffer_pos           # Set the input buffer position to the input
    jmp exit_setInPos

    setInPos_zero:
        movq $0, input_buffer_pos             # Set the input buffer position to 0
        jmp exit_setInPos

    setInPos_max:
        movq $MAXPOS, input_buffer_pos        # Set the input buffer position to MAXPOS
        jmp exit_setInPos

    exit_setInPos:
        ret


.global outImage

# Outputs the contents of the output buffer to standard output
outImage:
    push $0                              

    movq $output_buffer, %rdi             # Move value of output buffer to rdi
    call puts                             # Puts prints buffer in rdi to terminal

    movq $0, output_buffer_pos            # Reset output buffer position
    
    # Terminate outImage
    popq %rax                           
    ret


.global putInt

# Writes an integer to the output buffer
putInt:
    movq %rdi, %rax                       # Save rdi in rax, since we need rdi to use putChar
    cmpq $0, %rax                         # Compare the int to 0
    jge putInt_input_positive             # If it is greater than or equal to zero, use the convert loop
    movq $45, %rdi                        # Put ASCII for minus sign in rdi
    call putChar                          # Put the minus sign with putChar
    imulq $-1, %rax                       # Convert to a positive number

    putInt_input_positive:
        movq $0, %rcx                         # Zero rcx (used for length)
        movq $10, %r10                        # Base 10 for division
    putInt_convert_int_loop:
        movq $0, %rdx                         # Zero rdx for division
        idivq %r10                            # Divide rax by 10, remainder in rdx, quotient in rax

        addq $48, %rdx                        # Convert the digit to ASCII (adding ASCII offset)
        pushq %rdx                            # Push the ASCII value to the stack

        incq %rcx                             # Increment counter

        cmpq $0, %rax                         # Check if we have a quotient left
        jne putInt_convert_int_loop           # If we have a quotient left, continue loop

    putInt_put_loop:
        popq %rdi                             # Pop the ASCII value from the stack into rdi
        call putChar                          # Call putChar to write the character to the output buffer
        decq %rcx                             # Decrease the counter
        cmpq $0, %rcx                         # Check if we have more chars to put
        jne putInt_put_loop                   # If we have more chars to put, continue loop

    # Terminate putInt
    ret


.global putText

# Writes a text string to the output buffer
putText:
    movq %rdi, %r10                       # Move the input to r10 so we can use rdi in our loop

    putText_loop:
        cmpb $0, (%r10)                       # Check if the buffer given as input is empty
        je exit_putText_loop                  # If empty, exit
        movb (%r10), %rdi                     # If not empty, move char to rdi
        call putChar                          # Call putChar to write the char. putChar handles full buffer
        incq %r10                             # Increment the r10 address to read next char next time
        jmp putText_loop                    

    exit_putText_loop:
        # Terminate putText
        ret


.global putChar

# Writes a character to the output buffer
putChar:
    # Check if output buffer is full
    cmpq $output_buffer_pos, MAXPOS 
    jl putChar_not_full
    call outImage                         # Call outImage to output buffer contents

    putChar_not_full:
        movq $output_buffer, %r8              # Load output buffer address into r8
        addq output_buffer_pos, %r8           # Add position onto address to get current address
        movb %rdi, (%r8)                      # Put the char into the output buffer

        incq output_buffer_pos                # Increment the output buffer position

    # Terminate putChar
    ret


.global getOutPos

# Retrieves the current position in the output buffer
getOutPos:
    movq output_buffer_pos, %rax          # Move the value of output_buffer_pos to rax
    ret


.global setOutPos

# Sets the current position in the output buffer
setOutPos:
    cmpq $0, %rdi                         # Compare the input to 0
    jle setOutPos_zero                    # If it is less than or equal to zero, set to zero
    cmpq $MAXPOS, %rdi                    # Compare the input to MAXPOS
    jge setOutPos_max                     # If it is greater than or equal to MAXPOS, set to MAXPOS
    movq %rdi, output_buffer_pos          # Set the output buffer position to the input
    jmp exit_setOutPos

    setOutPos_zero:
        movq $0, output_buffer_pos            # Set the output buffer position to 0
        jmp exit_setOutPos

    setOutPos_max:
        movq $MAXPOS, output_buffer_pos       # Set the output buffer position to MAXPOS
        jmp exit_setOutPos

    exit_setOutPos:
        ret

# This assembly code implements routines for handling input and output buffers. 
# It includes procedures for reading from the input buffer, processing integers and text, and managing the output buffer. 
# The code reserves space for input and output buffers and maintains variables for the current positions within these buffers. 
# The input functions ensure the buffer is refilled when necessary, 
# and the output functions handle full buffers by writing out the contents and resetting the position.