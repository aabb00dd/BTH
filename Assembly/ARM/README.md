# ARM Assembly Programs

## 1. `display_controller.s`

### Description
This program manages a seven-segment display, UART communication, and push buttons on an ARM processor. It sets up stack pointers for different processor modes, configures the Generic Interrupt Controller (GIC), and initializes UART for receive interrupts. The main loop reads input from the terminal and updates the seven-segment display based on user input or push button interrupts.

### Key Features
- **Memory-mapped I/O**: Uses predefined addresses for UART, LEDs, switches, push buttons, and displays.
- **Interrupt Handling**: Configures and services interrupts for push buttons.
- **Display Management**: Controls a seven-segment display, allowing increment/decrement of displayed values via terminal input (`+`/`-`) or push buttons.
- **Hex Array**: Contains predefined hexadecimal values for displaying digits 0-9 and letters A-F.

### Functions
- `read_terminal`: Polls the terminal for valid input.
- `write_display`: Writes a value to the seven-segment display.
- `plus_minus_handler`: Processes `+` and `-` inputs to adjust the display.
- `increase_display_number` / `decrease_display_number`: Increments or decrements the displayed value.
- `PUSH_INTERRUPT_HANDLER`: Handles push button interrupts to adjust the display.

---

## 2. `uart_factorial_printer.s`

### Description
This program calculates and prints the factorial of numbers from 1 to 10 using the UART interface. It demonstrates recursive function calls, integer division, and UART communication.

### Key Features
- **UART Communication**: Prints strings and numbers via UART with busy-wait synchronization.
- **Recursive Factorial**: Computes factorial values recursively.
- **Integer Division**: Implements division to isolate digits for printing.
- **Stack Usage**: Manages the stack for function calls and digit storage.

### Functions
- `print_string`: Prints a null-terminated string via UART.
- `idiv`: Performs integer division.
- `print_number`: Prints a number followed by a newline.
- `factorial_calculator`: Recursively calculates the factorial of a number.
- `main`: Entry point that iterates from 1 to 10, computes factorials, and prints them.
