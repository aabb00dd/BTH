# Bytecode Interpreter for TAC (Three-Address Code)

This project implements a **stack-based interpreter** for a simplified bytecode language inspired by Java Virtual Machine instructions. It supports core arithmetic, logic, control flow, method calls, and stack manipulation operations.

---

## Features

- Reads and executes instructions from a `.bc` (bytecode) file.
- Handles basic arithmetic and logical expressions.
- Supports conditional and unconditional jumps.
- Supports method invocations and returns via an **activation stack**.
- Maintains a **variable store per method** using hash maps.
- Emits runtime output using a simulated `print` instruction.
- Includes a full set of **TAC-style opcodes**.