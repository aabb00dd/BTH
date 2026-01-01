# Symbol Table for MiniJava Compiler

This module implements a hierarchical **Symbol Table** for a MiniJava-like language. It is responsible for storing, organizing, and resolving identifiers such as variables, methods, parameters, and classes during **semantic analysis**.

---

## Overview

The symbol table enables the compiler to:

- Manage **nested scopes** (class scope, method scope, block scope).
- Detect **duplicate declarations**.
- Resolve identifiers during **type checking**.
- Track **types**, **line numbers**, and **record kinds** (Variable, Method, Class).
- Generate a **symbol table visualization** (`table.dot`).

It is tightly integrated with the AST (`Node`) and used during both symbol collection and semantic analysis.
