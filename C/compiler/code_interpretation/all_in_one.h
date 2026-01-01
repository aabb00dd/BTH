#ifndef ALL_IN_ONE_H
#define ALL_IN_ONE_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <functional>

#include "symbol_table.h"
#include "Node.h"


// --------- TAC opcodes --------- //
/*
0 iload n Push integer value stored in local variable n.
1 iconst v Push integer value v.
2 istore n Pop value v and store it in local variable n.
3 iadd Pop value v1 and v2. Push v2 + v1.
4 isub Pop value v1 and v2. Push v2 - v1.
5 imul Pop value v1 and v2. Push v2 * v1.
6 idiv Pop value v1 and v2. Push v2 / v1.
7 ilt Pop value v1 and v2. Push 1 if v2 <v1, else push 0.
8 igt Pop value v1 and v2. Push 1 if v2 >v1, else push 0.
9 ieq Pop value v1 and v2. Push 1 if v2 == v1, else push 0.
10 iand Pop value v1 and v2. Push 0 if v1 * v2 == 0, else push 1.
11 ior Pop value v1 and v2. Push 0 if v1 + v2 == 0, else push 1.
12 inot Pop value v. Push 1 if v == 0, else push 0.
13 goto i Jump to instruction labeled i unconditionally.
14 iffalse goto i
Pop value v from the data stack. If v == 0 jump to instruction labeled i, else
continue with the following instruction.
15 invokevirtual m
Push current activation to the activation stack and switch to the method having
qualified name m.
16 ireturn Pop the activation from the activation stack and continue.
17 print Pop the value from the data stack and print it.
18 stop Execution completed.
*/
#define ILOAD 0
#define ICONST 1
#define ISTORE 2
#define IADD 3
#define ISUB 4
#define IMUL 5
#define IDIV 6
#define ILT 7
#define IGT 8
#define IEQ 9
#define IAND 10
#define IOR 11
#define INOT 12
#define GOTO 13
#define IFFALSE 14
#define INVOKEVIRTUAL 15
#define IRETURN 16
#define PRINT 17
#define STOP 18

const std::unordered_map<std::string, int> op_str_to_int = {
    {"iload", ILOAD},
    {"iconst", ICONST},
    {"istore", ISTORE},
    {"iadd", IADD},
    {"isub", ISUB},
    {"imul", IMUL},
    {"idiv", IDIV},
    {"ilt", ILT},
    {"igt", IGT},
    {"ieq", IEQ},
    {"iand", IAND},
    {"ior", IOR},
    {"inot", INOT},
    {"goto", GOTO},
    {"iffalse", IFFALSE},
    {"invokevirtual", INVOKEVIRTUAL},
    {"ireturn", IRETURN},
    {"print", PRINT},
    {"stop", STOP}
};


const std::unordered_map<int, std::string> op_int_to_str = {
    {ILOAD, "iload"},
    {ICONST, "iconst"},
    {ISTORE, "istore"},
    {IADD, "iadd"},
    {ISUB, "isub"},
    {IMUL, "imul"},
    {IDIV, "idiv"},
    {ILT, "ilt"},
    {IGT, "igt"},
    {IEQ, "ieq"},
    {IAND, "iand"},
    {IOR, "ior"},
    {INOT, "inot"},
    {GOTO, "goto"},
    {IFFALSE, "iffalse"},
    {INVOKEVIRTUAL, "invokevirtual"},
    {IRETURN, "ireturn"},
    {PRINT, "print"},
    {STOP, "stop"}
};


// Forward declarations for the Tac classes from slides
class Tac;
class Expression;
class UnaryExpression;
class Copy;
class CopyArray;
class ArrayAccess;
class New;
class NewArray;
class Length;
class Parameter;
class MethodCall;
class Return;
class UnconditionalJump;
class ConditionalJump;
class Print;
class Stop;

// The program->method->instruction structure from slides
// Multi method is a collection of methods, used for bytecode generation
class Method_cfg;
class Instruction_cfg;
class Program_cfg;
class Multi_Method_cfg;

// Forward declarations for the BasicBlock class from slides
class BasicBlock;


// Globals needed for program
/*
current_block:
    Working pointer to the block currently being processed
    Instructions are added to this block during AST traversal
    Gets updated as you move through control structures (if/else, while loops)

methods:
    Stores entry points (first block) for each method in your program
    Each method has its own control flow graph starting from these blocks
    Used in generate_cfg_file() to process all methods

rendered_blocks:
    Tracks which blocks have already been rendered in the DOT file
    Prevents duplicate nodes when the CFG contains cycles (loops)
    Reset at the start of generate_cfg_file()

visited:
    Used during code generation to track which blocks have been processed
    Prevents infinite recursion when traversing cyclic control flow
    Essential for correctly handling loops and conditional branches
*/
BasicBlock *current_block;
std::vector<BasicBlock *> methods;
std::vector<std::string> rendered_blocks;
std::vector<std::string> visited;


// --------- CFG to Bytecode stuff --------- //
// Instruction class holds a single instruction in the CFG (id and argument)
class Instruction_cfg {
    public:
        int id; // Type of instruction
        std::string argument; // The instruction's argument (e.g iload n, istorev, ...)

        Instruction_cfg() {}

        void print(ofstream* out) {
            // Print the instruction with the id and argument
            *out << "   " << op_int_to_str.at(id) << " " << argument << std::endl;
        }
};

// Method class holds a method in the CFG (method name and list of instructions)
class Method_cfg {
    public:
        std::string method_name;
        std::vector<Instruction_cfg *> instructions;

        Method_cfg() {}

        void print(ofstream* out) {
            *out << method_name << ":" << std::endl;
            for (auto instruction : instructions) {
                instruction->print(out);
            }
        }
};

// Multi_Method class holds a collection of methods in the CFG (list of variables and list of methods)
class Multi_Method_cfg {
    public:
        std::vector<std::string> variables;
        std::vector<Method_cfg *> methods;

        Multi_Method_cfg() {}

        void print(ofstream* out) {
            for (auto method : methods) {
                method->print(out);
            }
        }

        // Get the instruction at a given program counter
        Instruction_cfg *get_instruction_at_program_counter(int program_counter) {
            int index = 0;
            Method_cfg *current;
            while (program_counter >= 0) {
                current = methods[index++];
                program_counter -= current->instructions.size();
            }
            return current->instructions[current->instructions.size() + program_counter];
        }

        // Calculates the program counter for a given method name (basically finding where a method starts in the bytecode)
        int calculate_program_counter(std::string input_method_name) {
            int index = 0;
            int program_counter = 0;
            Method_cfg *current = methods[index++];
            while (current->method_name != input_method_name) {
                program_counter += current->instructions.size();
                current = methods[index++];
            }
            return program_counter;
        }
};

// Program class holds the entire program in the CFG (map of strings to multi methods)
class Program_cfg {
    public:
        std::unordered_map<std::string, Multi_Method_cfg *> methods;

        Program_cfg() {}

        void print() {
            // Init ofstream
            ofstream out;
            out.open("output.bc");

            // Print methods by going lower in the hierarchy and printing each instruction with the id and argument
            for (std::pair<std::string, Multi_Method_cfg *> pair : methods) {
                pair.second->print(&out);
            }
            std::cout << "Bytecode printed to output.bc" << std::endl;
            out.close();
        }
};


// --------- TAC implementation --------- //
/*
Expression 
    x = y + z 
    x := y op z 
    calculate y op z, store the result in x
Unary expression 
    x = !y 
    x := op y 
    calculate op y, store the result in x
Copy 
    x = y 
    x := y 
    copy y to x
Array Access 
    case 1:
        x = y[i]; 
        x := y[i] 
        copy y[i] to x
    case 2:
        y[i] = x 
        y[i] := x 
        copy x to y[i]
New 
    x = new A; 
    x := new TYPE 
    create a new object of type A, store it in x
New Array 
    x = new int[N] 
    x := new TYPE, N 
    create a new object of type int[], with size N,store in x
Length 
    x = y.length 
    x := length y 
    get the member length of y, store the results in x
Parameter 
    x = this.f(y) 
    param y 
    define parameter y
Method call 
    x = this.f(y) 
    x := call f, N 
    call function f, use N parameters from the stack
Return 
    return y 
    return y 
    return y
Unconditional Jump 
    goto L 
    next execute the instruction labeled L
Conditional Jump 
    iffalse x goto L 
    if x is false, next execute the instruction labeled L
    push parameter y to the stack
*/

// Helper function to convert boolean literals to integers
std::string boolToInt(const std::string& value) {
    if (value == "true") {
        return "1";
    } else if (value == "false") {
        return "0";
    }
    return value;
}

// Helper function to determine if a value should use ICONST (for literals) or ILOAD (for variables)
int determineLoadType(const std::string& value) {
    try {
        std::stoi(value);
        return ICONST; // It's a number literal
    } catch (std::invalid_argument&) {
        return ILOAD;  // It's a variable name
    }
}


// Base class for TAC instructions as per Tac structure skeleton in slides
class Tac {
    public:
        std::string op, lhs, rhs, result;

        Tac(std::string _op, std::string _lhs, std::string _rhs, std::string _result) : op(_op), lhs(_lhs), rhs(_rhs), result(_result) {}

        virtual void generate_code(Method_cfg* method) {
            // Empty since it's a base class
        }
};

// Expression class inherits from Tac
class Expression : public Tac {
    /*
    Expression 
        x = y + z 
        x := y op z 
        calculate y op z, store the result in x
    */
    public:
        Expression(std::string _op, std::string _y, std::string _z, std::string _res) : Tac(_op, _y, _z, _res) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* load_lhs_instruction = new Instruction_cfg();
            Instruction_cfg* load_rhs_instruction = new Instruction_cfg();
            Instruction_cfg* operator_instruction = new Instruction_cfg();
            Instruction_cfg* store_instruction = new Instruction_cfg();

            lhs = boolToInt(lhs); // Call helper to convert boolean literals to integers

            load_lhs_instruction->id = determineLoadType(lhs); // Call helper to determine if we should use ICONST or ILOAD

            load_lhs_instruction->argument = lhs;

            rhs = boolToInt(rhs); // Call helper to convert boolean literals to integers

            load_rhs_instruction->id = determineLoadType(rhs); // Call helper to determine if we should use ICONST or ILOAD

            load_rhs_instruction->argument = rhs;

            if (op == "+") {
                operator_instruction->id = IADD;
            }
            else if (op == "-") {
                operator_instruction->id = ISUB;
            }
            else if (op == "*") {
                operator_instruction->id = IMUL;
            }
            else if (op == "<") {
                operator_instruction->id = ILT;
            }
            else if (op == ">") {
                operator_instruction->id = IGT;
            }
            else if (op == "==") {
                operator_instruction->id = IEQ;
            }
            else if (op == "&&") {
                operator_instruction->id = IAND;
            }
            else if (op == "||") {
                operator_instruction->id = IOR;
            }
            
            store_instruction->id = ISTORE;
            store_instruction->argument = result;

            method->instructions.push_back(load_lhs_instruction);
            method->instructions.push_back(load_rhs_instruction);
            method->instructions.push_back(operator_instruction);
            method->instructions.push_back(store_instruction);    
        }
};

// UnaryExpression class inherits from Tac
class UnaryExpression : public Tac {
    /*
    Unary expression 
        x = !y 
        x := op y 
        calculate op y, store the result in x
    */
    public:
        UnaryExpression(std::string _op, std::string _rhs, std::string _res) : Tac(_op, "", _rhs , _res) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* load_rhs_instruction = new Instruction_cfg();
            Instruction_cfg* operator_instruction = new Instruction_cfg();
            Instruction_cfg* store_instruction = new Instruction_cfg();

            rhs = boolToInt(rhs); // Call helper to convert boolean literals to integers

            load_rhs_instruction->id = determineLoadType(rhs); // Call helper to determine if we should use ICONST or ILOAD

            load_rhs_instruction->argument = rhs;

            store_instruction->id = ISTORE;
            store_instruction->argument = result;

            method->instructions.push_back(load_rhs_instruction);
            method->instructions.push_back(operator_instruction);
            method->instructions.push_back(store_instruction);
        }
};

// Copy class inherits from Tac
class Copy : public Tac {
    /*
    Copy 
        x = y 
        x := y 
        copy y to x
    */
    public:
        Copy(std::string _y, std::string _res) : Tac("Copy", _y, "", _res) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* load_instruction = new Instruction_cfg();
            Instruction_cfg* store_instruction = new Instruction_cfg();

            lhs = boolToInt(lhs); // Call helper to convert boolean literals to integers

            load_instruction->id = determineLoadType(lhs); // Call helper to determine if we should use ICONST or ILOAD

            load_instruction->argument = lhs;

            store_instruction->id = ISTORE;
            store_instruction->argument = result;
            method->instructions.push_back(load_instruction);
            method->instructions.push_back(store_instruction);
        }
};

// CopyArray class inherits from Tac
class CopyArray : public Tac {
    /*
    Copy Array
        x = y[i]
        x := y[i]
        copy y[i] to x
    */
    public:
        CopyArray(std::string _y, std::string _i, std::string _res) : Tac("CopyArray", _y, _i, _res) {}

        void generate_code(Method_cfg* method) override {}
};

// ArrayAccess class inherits from Tac
class ArrayAccess : public Tac {
    /*
    Array Access 
        case 1:
            x = y[i]; 
            x := y[i] 
            copy y[i] to x
        case 2:
            y[i] = x 
            y[i] := x 
            copy x to y[i]
    */
    public:
        ArrayAccess(std::string _y, std::string _i, std::string _res) : Tac("ArrayAccess", _y, _i, _res) {}

        void generate_code(Method_cfg* method) override {}
};

// New class inherits from Tac
class New : public Tac {
    /*
    New 
        x = new A; 
        x := new TYPE 
        create a new object of type A, store it in x
    */
    public:
        New(std::string _rhs,std::string _res) : Tac("new", "", _rhs, _res) {}

        void generate_code(Method_cfg* method) override {}
};

// NewArray class inherits from Tac
class NewArray : public Tac {
    /*
    New Array 
        x = new int[N] 
        x := new TYPE, N 
        create a new object of type int[], with size N,store in x
    */
    public:
        NewArray(string _type, string _N, string _result) : Tac("new", _type, _N, _result) {}

        void generate_code(Method_cfg* method) override {}
};

// Length class inherits from Tac
class Length : public Tac {
    /*
    Length 
        x = y.length 
        x := length y 
        get the member length of y, store the results in x
    */
    public:
        Length(std::string _y, std::string _res) : Tac("length", _y, "", _res) {}

        void generate_code(Method_cfg* method) override {}
};

// Parameter class inherits from Tac
class Parameter : public Tac {
    /*
    Parameter 
        x = this.f(y) 
        param y 
        define parameter y
    */
    public:
        Parameter(std::string _res) : Tac("param", "", "", _res) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* param_instruction = new Instruction_cfg();

            result = boolToInt(result); // Call helper to convert boolean literals to integers

            param_instruction->id = determineLoadType(result); // Call helper to determine if we should use ICONST or ILOAD

            param_instruction->argument = result;
            method->instructions.push_back(param_instruction);
        }
};

// MethodCall class inherits from Tac
class MethodCall : public Tac {
    /*
    Method call 
        x = this.f(y) 
        x := call f, N 
        call function f, use N parameters from the stack
    */  
    public:
        MethodCall(std::string _f, std::string _N, std::string _res) : Tac("call", _f, _N, _res) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* call_instruction = new Instruction_cfg();
            Instruction_cfg* return_instruction = new Instruction_cfg();

            call_instruction->id = INVOKEVIRTUAL;
            call_instruction->argument = lhs;
            return_instruction->id = ISTORE;
            return_instruction->argument = result;

            method->instructions.push_back(call_instruction);
            method->instructions.push_back(return_instruction);
        }
};

// Return class inherits from Tac
class Return : public Tac {
    /*
    Return 
        return y 
        return y 
        return y
    */
    public:
        Return(std::string _res) : Tac("return", "", "", _res) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* value_instruction = new Instruction_cfg();
            Instruction_cfg* return_instruction = new Instruction_cfg();

            result = boolToInt(result); // Call helper to convert boolean literals to integers

            value_instruction->id = determineLoadType(result); // Call helper to determine if we should use ICONST or ILOAD

            value_instruction->argument = result;
            return_instruction->id = IRETURN;

            method->instructions.push_back(value_instruction);
            method->instructions.push_back(return_instruction);
        }
};

// UnCondJump class inherits from Tac
class UnCondJump : public Tac {
    /*
    Unconditional Jump 
        goto L 
        next execute the instruction labeled L
    */
    public:
        UnCondJump(std::string _label) : Tac("goto", "", "", _label) {}

        void generate_code(Method_cfg* method) override {
            Instruction_cfg* goto_instruction = new Instruction_cfg();
            
            // Set up unconditional jump
            goto_instruction->id = GOTO;
            goto_instruction->argument = result;
            
            // Add instruction to method
            method->instructions.push_back(goto_instruction);
        }
};

// CondJump class inherits from Tac
class CondJump : public Tac {
    /*
    Conditional Jump 
        iffalse x goto L 
        if x is false, next execute the instruction labeled L
        push parameter y to the stack
    */
    public:
        CondJump(std::string _x, std::string _res) : Tac("iffalse", _x, "goto", _res) {}

        void generate_code(Method_cfg* method) {
            Instruction_cfg* load_instruction = new Instruction_cfg();
            Instruction_cfg* jump_instruction = new Instruction_cfg();
            
            lhs = boolToInt(lhs); // Call helper to convert boolean literals to integers

            load_instruction->id = determineLoadType(lhs); // Call helper to determine if we should use ICONST or ILOAD

            load_instruction->argument = lhs;
            jump_instruction->id = IFFALSE;
            jump_instruction->argument = result;

            method->instructions.push_back(load_instruction);
            method->instructions.push_back(jump_instruction);
        }
};

// Print class inherits from Tac
class Print : public Tac {
    public:
        Print(std::string _y) : Tac("print", _y, "", "") {}
        
        void generate_code(Method_cfg* method) override {
            Instruction_cfg* load_instruction = new Instruction_cfg();
            Instruction_cfg* print_instruction = new Instruction_cfg();
            
            lhs = boolToInt(lhs); // Call helper to convert boolean literals to integers
            
            load_instruction->id = determineLoadType(lhs); // Call helper to determine if we should use ICONST or ILOAD
            
            load_instruction->argument = lhs;
            
            // Set up print
            print_instruction->id = PRINT;
            
            // Add instructions to method
            method->instructions.push_back(load_instruction);
            method->instructions.push_back(print_instruction);
        }
};


// --------- Basic Blocks, basic bitches --------- //
/*
BasicBlock from slides
class BBlock {
    string name; //unique name
    list<Tac> tacInstructions;
    Tac condition;
    BBlock *trueExit, *falseExit;
    BBlock() : trueExit(NULL),
    falseExit(NULL) {}
}
*/

// Class for basic block for basic block from slides
class BasicBlock {
    public:
        // Basic stuff
        std::string name; // unique name
        std::vector<Tac *> tacInstructions;
        Tac *condition;
        BasicBlock *trueExit, *falseExit;

        // Additional stuff
        inline static int block_counter = 0; // For unique block naming
        inline static int temp_var_counter = 0; // For unique temp variable naming

        // Two constructors depending on if a name is provided or needs to be generated
        BasicBlock() : trueExit(nullptr), falseExit(nullptr), condition(nullptr), name(generate_name_self()) {}
        BasicBlock(std::string _name) : trueExit(nullptr), falseExit(nullptr), condition(nullptr), name(_name) {}

        // Method for generating a unique block name
        static std::string generate_name_self() {
            return "block_" + std::to_string(block_counter++);
        }

        // Method for generating a unique temp variable name
        static std::string generate_temp_var() {
            return "_temp" + std::to_string(temp_var_counter++);
        }

        // Method for generating bytecode from the CFG
        static void generate_bytecode(Program_cfg* program) {
            // Loop through all methods and generate bytecode for each
            for (BasicBlock* block : methods) {
                // If the block has not been visited, generate code for it
                Multi_Method_cfg* new_method = new Multi_Method_cfg();

                // Generate code the method with the block name as the method name
                program->methods[block->name] = new_method;

                // Generate code for the block
                block->generate_code(new_method, block->name);
            }
        }

        // Method for generating code from the CFG
        void generate_code(Multi_Method_cfg* method, std::string methodName) {
            // Create a new method block since we are in a new method
            Method_cfg* method_block = new Method_cfg();

            // Set the method name to the block name
            method_block->method_name = name;

            // Make sure we don't visit the same block twice
            visited.push_back(name);

            // Add the method block to the method
            method->methods.push_back(method_block);

            // Loop through the TAC instructions and generate code for each
            for (Tac* instruction : tacInstructions) {
                instruction->generate_code(method_block);
            }

            // If there is a condition, generate code for it
            if (trueExit != nullptr && falseExit == nullptr) { // Only true exit
                if (find(visited.begin(), visited.end(), trueExit->name) == visited.end()) { // Visit true exit
                    trueExit->generate_code(method, methodName); // Recursively generate code for the true exit
                }
            }
            else if (trueExit != nullptr && falseExit != nullptr) { // Both exits
                if (find(visited.begin(), visited.end(), falseExit->name) == visited.end()) { // Visit false exit first
                    falseExit->generate_code(method, methodName); // Recursively generate code for the false exit
                }
                if (find(visited.begin(), visited.end(), trueExit->name) == visited.end()) { // Visit true exit second
                    trueExit->generate_code(method, methodName); // Recursively generate code for the true exit
                }
            }
            else { // No exits
                if (methodName == "main") { // If we are in the main method (entry point)
                    // Add a stop instruction to the main method
                    Instruction_cfg* stop_instruction = new Instruction_cfg();

                    // Set the stop instruction
                    stop_instruction->id = 18;

                    // Add the stop instruction to the method block
                    method_block->instructions.push_back(stop_instruction);
                }
            }  
        }
};


// --------- AST to IR stuff --------- //
// Post order traversal of AST

// Recursive function to generate TAC from AST. Similar to how we did in semantic analysis (string recursive function)
// Add these function declarations before tac_from_ast
std::string handle_main_class(SymbolTable *table, Node *root);
std::string handle_class_declaration(SymbolTable *table, Node *root);
std::string handle_class_list(SymbolTable *table, Node *root);
std::string handle_method_declaration(SymbolTable *table, Node *root);
std::string handle_method_list(SymbolTable *table, Node *root);
std::string handle_method_params(SymbolTable *table, Node *root);
std::string handle_binary_expression(SymbolTable *table, Node *root);
std::string handle_arithmetic_expression(SymbolTable *table, Node *root);
std::string handle_array_expression(SymbolTable *table, Node *root);
std::string handle_new_int_array(SymbolTable *table, Node *root);
std::string handle_method_call(SymbolTable *table, Node *root);
std::string handle_length_expression(SymbolTable *table, Node *root);
std::string handle_new_object(SymbolTable *table, Node *root);
std::string handle_not_expression(SymbolTable *table, Node *root);
std::string handle_if_statement(SymbolTable *table, Node *root);
std::string handle_if_else_statement(SymbolTable *table, Node *root);
std::string handle_while_statement(SymbolTable *table, Node *root);
std::string handle_assign_statement(SymbolTable *table, Node *root);
std::string handle_array_assign_statement(SymbolTable *table, Node *root);
std::string handle_print_statement(SymbolTable *table, Node *root);
std::string handle_simple_value(SymbolTable *table, Node *root);

// The refactored main function becomes a dispatcher
std::string tac_from_ast(SymbolTable *table, Node *root) {
    if (!root) { // Base case
        return "";
    }
    
    // Dispatch to appropriate handler based on node type
    if (root->type == "MainClass") {
        return handle_main_class(table, root);
    }
    else if (root->type == "ClassDeclaration") {
        return handle_class_declaration(table, root);
    }
    else if (root->type == "ClassDeclarationList") {
        return handle_class_list(table, root);
    }
    else if (root->type == "MethodDeclaration") {
        return handle_method_declaration(table, root);
    }
    else if (root->type == "MethodDeclarationList") {
        return handle_method_list(table, root);
    }
    else if (root->type == "MethodParameterList") {
        return handle_method_params(table, root);
    }
    else if (root->type == "AndExpression" 
        || root->type == "OrExpression" 
        || root->type == "EqualsExpression" 
        || root->type == "LessThanExpression" 
        || root->type == "GreaterThanExpression") {
        return handle_binary_expression(table, root);
    }
    else if (root->type == "PlusOperationExpression" 
        || root->type == "MinusOperationExpression" 
        || root->type == "MultiplicationOperationExpression") {
        return handle_arithmetic_expression(table, root);
    }
    else if (root->type == "ArrayExpression") {
        return handle_array_expression(table, root);
    }
    else if (root->type == "NewIntArrayExpression") {
        return handle_new_int_array(table, root);
    }
    else if (root->type == "MethodCallExpression") {
        return handle_method_call(table, root);
    }
    else if (root->type == "LengthExpression") {
        return handle_length_expression(table, root);
    }
    else if (root->type == "NewObjectExpression") {
        return handle_new_object(table, root);
    }
    else if (root->type == "NotExpression") {
        return handle_not_expression(table, root);
    }
    else if (root->type == "IfStatement") {
        return handle_if_statement(table, root);
    }
    else if (root->type == "IfElseStatement") {
        return handle_if_else_statement(table, root);
    }
    else if (root->type == "WhileStatement") {
        return handle_while_statement(table, root);
    }
    else if (root->type == "AssignStatement") {
        return handle_assign_statement(table, root);
    }
    else if (root->type == "ArrayAssignStatement") {
        return handle_array_assign_statement(table, root);
    }
    else if (root->type == "PrintStatement") {
        return handle_print_statement(table, root);
    }
    else if (root->type == "VarDeclaration" 
        || root->type == "Parameter" 
        || root->type == "IntLiteralExpression" 
        || root->type == "BoolLiteral" 
        || root->type == "Identifier" 
        || root->type == "ThisExpression") {
        return handle_simple_value(table, root);
    }
    else if (root->type == "ParenthesesExpression") {
        return tac_from_ast(table, root->children.front());
    }
    else if (root->type == "MethodBody") {
        // Process all children and return empty string
        for (auto child : root->children) {
            tac_from_ast(table, child);
        }
        return "";
    }
    else { // Generic handler for any other node types
        for (auto child : root->children) {
            tac_from_ast(table, child);
        }
        return root->value;
    }
}
// NOTE to self, about temp variables //
/*
Temporary variables needs to be added to the symbol table since they are used in the TAC instructions.
This wont affect the symbol table during semantic analysis since this is done before cfg step in compilation
and the graph is saved to the dot file before this too.
*/

// --- Implementation of helper functions --- //
// Mainclass handler
std::string handle_main_class(SymbolTable *table, Node *root) {
    // Enter main class scope
    table->enter_scope("", nullptr);

    // Loop through children and generate TAC recursively
    for (auto child : root->children) {
        tac_from_ast(table, child);
    }

    // Exit main class scope
    table->exit_scope();

    // Return the main class name
    return root->value;
}

// Class declaration handler
std::string handle_class_declaration(SymbolTable *table, Node *root) {
    // Enter class scope
    table->enter_scope("", nullptr);

    // Loop through children and generate TAC recursively
    for (auto child : root->children) {
        tac_from_ast(table, child);
    }

    // Exit class scope
    table->exit_scope();

    // Return the class name
    return root->value;
}

// Class declaration list handler
std::string handle_class_list(SymbolTable *table, Node *root) {
    // Loop through children and generate TAC recursively
    for (auto child : root->children) {
        tac_from_ast(table, child);
    }

    // Return empty string since class lists dont have values themselves
    return "";
}

// Method declaration handler
std::string handle_method_declaration(SymbolTable *table, Node *root) {
    // Enter method scope
    table->enter_scope("", nullptr);

    // Create a new basic block for the method, named after the class and method.
    // We create blocks in method since this is where a split in the control flow can occur.
    current_block = new BasicBlock(table->current_scope->get_parent()->scope_name.substr(7) + "." + root->value);

    // Add the block to the list of methods of the global variable
    methods.push_back(current_block);
    
    // Process all children except the last one (which is the return expression)
    for (int i = 0; i < root->children.size() - 1; i++) {
        tac_from_ast(table, getChild(root, i));
    }
    
    // Handle the return expression (last child)
    if (!root->children.empty()) {
        // Get the return expression
        Node* returnExpr = root->children.back();

        // Generate TAC for the return expression (recursively)
        std::string returnValue = tac_from_ast(table, returnExpr);

        // Create a Return TAC instruction since its a return expression
        current_block->tacInstructions.push_back(new Return(returnValue));
    }

    // Exit method scope
    table->exit_scope();

    // Return the method name
    return root->value;
}

// Method list handler
std::string handle_method_list(SymbolTable *table, Node *root) {
    // Loop through children and generate TAC recursively
    for (auto child : root->children) {
        tac_from_ast(table, child);
    }

    // Return empty string since method lists dont have values themselves
    return "";
}

// Method parameter list handler
std::string handle_method_params(SymbolTable *table, Node *root) {
    // Loop through children and generate TAC recursively
    for (auto child : root->children) {
        // Get the name of the parameter and add it to the symbol table (this is done via recursive propagation)
        std::string name = tac_from_ast(table, child);

        // Create a new Parameter TAC instruction 
        Tac *instruction = new Parameter(name);

        // Add it to the current block (since this is a parameter)
        current_block->tacInstructions.push_back(instruction);
    }

    // Return empty string since method parameter lists dont have values themselves
    return "";
}

// Binary expression handler
std::string handle_binary_expression(SymbolTable *table, Node *root) {
    // Generate a temporary variable name
    std::string name = current_block->generate_temp_var();
    
    // Add it to the symbol table with type "Bool"
    table->put(name, new Variable(name, "Bool"));
    
    // Get the left and right operands using recursive propagation
    std::string lhs_name = tac_from_ast(table, getChild(root, 0));
    std::string rhs_name = tac_from_ast(table, getChild(root, 1));
    
    // Determine the operator based on the node type
    std::string op;
    if (root->type == "AndExpression") op = "&&";
    else if (root->type == "OrExpression") op = "||";
    else if (root->type == "EqualsExpression") op = "==";
    else if (root->type == "LessThanExpression") op = "<";
    else if (root->type == "GreaterThanExpression") op = ">";
    
    // Create and add the expression instruction (since this is an expression)
    Tac* instruction = new Expression(op, lhs_name, rhs_name, name);
    current_block->tacInstructions.push_back(instruction);
    
    // Reurn the temporary variable name for the expression (for propagation purposes)
    return name;
}

// Arithmetic expression handler
std::string handle_arithmetic_expression(SymbolTable *table, Node *root) {
    // Generate a temporary variable name
    std::string name = current_block->generate_temp_var();
    
    // Add it to the symbol table with type "Int"
    table->put(name, new Variable(name, "Int"));
    
    // Get the left and right operands using recursive propagation
    std::string lhs_name = tac_from_ast(table, getChild(root, 0));
    std::string rhs_name = tac_from_ast(table, getChild(root, 1));
    
    // Determine the operator based on the node type
    std::string op;
    if (root->type == "PlusOperationExpression") op = "+";
    else if (root->type == "MinusOperationExpression") op = "-";
    else if (root->type == "MultiplicationOperationExpression") op = "*";
    
    // Create and add the expression instruction (since this is an expression)
    Tac* instruction = new Expression(op, lhs_name, rhs_name, name);
    current_block->tacInstructions.push_back(instruction);
    
    // Reurn the temporary variable name for the expression (for propagation purposes)
    return name;
}

// Array expression handler
std::string handle_array_expression(SymbolTable *table, Node *root) {
    // Generate a temporary variable name
    std::string name = BasicBlock::generate_temp_var();

    // Add it to the symbol table with type "Int"
    table->put(name, new Variable(name, "Int"));

    // Get left operand (which is name, first child) using recursive propagation
    Node* child = getChild(root, 0);
    std::string lhs_name = tac_from_ast(table, child);

    // Get right operand (which is index, second child) using recursive propagation
    child = getChild(root, 1);
    std::string index = tac_from_ast(table, child);

    // Create and add the array access instruction (since this is an array access)
    Tac *instruction = new ArrayAccess(index, lhs_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the temporary variable name for the array access (for propagation purposes)
    return name;
}

// New int array expression handler
std::string handle_new_int_array(SymbolTable *table, Node *root) {
    // Get the length of the array using recursive propagation
    Node* child = getChild(root, 0);
    std::string len_name = tac_from_ast(table, child);

    // Generate a temporary variable name
    std::string name = BasicBlock::generate_temp_var();

    // Add it to the symbol table with type "IntArray"
    table->put(name, new Variable(name, "IntArray"));

    // Create and add the new array instruction (since this is a new array)
    Tac *instruction = new NewArray("IntArray", len_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the temporary variable name for the new array (for propagation purposes)
    return name;
}

// Method call expression handler
std::string handle_method_call(SymbolTable *table, Node *root) {
    // Generate a temporary variable name
    std::string name = BasicBlock::generate_temp_var();

    // Add it to the symbol table with type "Int"
    table->put(name, new Variable(name, ""));

    // Get the caller using recursive propagation
    Node *child = getChild(root, 0);
    std::string caller = tac_from_ast(table, child);

    // Get the method name using recursive propagation
    child = getChild(root, 1);
    std::string table_method_name = table->lookup(caller)->get_type() + "." + child->value;

    // Get the arguments using recursive propagation
    Node* parameter_list = getChild(root, 2); // This is the parameter list for the method call
    if (parameter_list != nullptr) { // Does it exist?
        tac_from_ast(table, parameter_list); // If so, generate TAC for it
    }

    // Calculate length (1 + number of parameters (if any))
    int length = 1 + (parameter_list != nullptr ? parameter_list->children.size() : 0);

    // Create and add the method call instruction (since this is a method call)
    Tac *instruction = new MethodCall(table_method_name, to_string(length), name);
    current_block->tacInstructions.push_back(instruction);

    // Return the temporary variable name for the method call (for propagation purposes)
    return name;
}

// Length expression handler
std::string handle_length_expression(SymbolTable *table, Node *root) {
    // Generate a temporary variable name
    std::string name = BasicBlock::generate_temp_var();

    // Add it to the symbol table with type "Int"
    table->put(name, new Variable(name, "Int"));

    // Get the lhs_name using recursive propagation
    Node* child = getChild(root, 0);
    std::string lhs_name = tac_from_ast(table, child);

    // Create and add the length instruction (since this is a length expression)
    Tac* instruction = new Length(lhs_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the temporary variable name for the length expression (for propagation purposes)
    return name;
}

// New object expression handler
std::string handle_new_object(SymbolTable *table, Node *root) {
    // Get the class name using recursive propagation
    Node* child = getChild(root, 0);
    std::string class_name = tac_from_ast(table, child);

    // Generate a temporary variable name
    std::string name = BasicBlock::generate_temp_var();

    // Add it to the symbol table with type of class name (since this is a new object)
    table->put(name, new Variable(name, class_name));

    // Create and add the new object instruction (since this is a new object)
    Tac *instruction = new New(class_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the temporary variable name for the new object (for propagation purposes)
    return name;
}

// Not expression handler
std::string handle_not_expression(SymbolTable *table, Node *root) {
    // Generate a temporary variable name
    std::string name = BasicBlock::generate_temp_var();

    // Add it to the symbol table with type "Bool"
    table->put(name, new Variable(name, "Bool"));

    // Get the rhs_name (the expression) using recursive propagation
    Node* child = getChild(root, 0);
    std::string rhs_name = tac_from_ast(table, child);

    // Create and add the unary expression instruction (since this is a unary expression)
    Tac* instruction = new UnaryExpression("!", rhs_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the temporary variable name for the unary expression (for propagation purposes)
    return name;
}

// If statement handler
std::string handle_if_statement(SymbolTable *table, Node *root) {
    // Create blocks for the true branch and join point
    BasicBlock* trueBlock = new BasicBlock();
    BasicBlock* joinBlock = new BasicBlock();
    
    // Get the condition expression
    std::string condValue = tac_from_ast(table, getChild(root, 0));
    
    // Create conditional jump instruction
    CondJump* condJump = new CondJump(condValue, joinBlock->name);
    current_block->tacInstructions.push_back(condJump);
    current_block->condition = condJump;
    
    // Set up the control flow
    current_block->trueExit = trueBlock;
    current_block->falseExit = joinBlock;
    
    // Process the true branch
    current_block = trueBlock;
    tac_from_ast(table, getChild(root, 1));
    
    // Add jump to join block at the end of true branch
    UnCondJump* trueJump = new UnCondJump(joinBlock->name);
    current_block->tacInstructions.push_back(trueJump);
    current_block->trueExit = joinBlock;
    
    // Continue with the join block
    current_block = joinBlock;
    
    // Return empty string since if statements dont have values themselves
    return "";
}

// If-else statement handler
std::string handle_if_else_statement(SymbolTable *table, Node *root) {
    // Create blocks for true branch, false branch, and join point
    BasicBlock* trueBlock = new BasicBlock();
    BasicBlock* falseBlock = new BasicBlock();
    BasicBlock* joinBlock = new BasicBlock();
    
    // Get the condition expression
    std::string condValue = tac_from_ast(table, getChild(root, 0));
    
    // Create conditional jump instruction
    CondJump* condJump = new CondJump(condValue, falseBlock->name);
    current_block->tacInstructions.push_back(condJump);
    current_block->condition = condJump;
    
    // Set up the control flow
    current_block->trueExit = trueBlock;
    current_block->falseExit = falseBlock;
    
    // Process the true branch
    current_block = trueBlock;
    tac_from_ast(table, getChild(root, 1));
    
    // Add jump to join block at the end of true branch
    UnCondJump* trueJump = new UnCondJump(joinBlock->name);
    current_block->tacInstructions.push_back(trueJump);
    current_block->trueExit = joinBlock;
    
    // Process the false branch
    current_block = falseBlock;
    tac_from_ast(table, getChild(root, 2));
    
    // Add jump to join block at the end of false branch
    UnCondJump* falseJump = new UnCondJump(joinBlock->name);
    current_block->tacInstructions.push_back(falseJump);
    current_block->trueExit = joinBlock;
    
    // Continue with the join block
    current_block = joinBlock;
    
    // Return empty string since if-else statements dont have values themselves
    return "";
}

// While statement handler
std::string handle_while_statement(SymbolTable *table, Node *root) {
    // Create blocks for the header, true branch, and join point
    BasicBlock *header_block = new BasicBlock();
    BasicBlock *true_block = new BasicBlock();
    BasicBlock *join_block = new BasicBlock();

    // Set current block true exit to header block (since this is a loop)
    current_block->trueExit = header_block;

    // Create unconditional jump to header block and add to current block instructions
    Tac *jump = new UnCondJump(header_block->name);
    current_block->tacInstructions.push_back(jump);

    // Grab first child (condition)
    Node* child = getChild(root, 0);

    // Process the condition
    current_block = header_block;
    std::string condition_name = tac_from_ast(table, child);
    Tac *condition = new CondJump(condition_name, join_block->name); // Create conditional jump instruction
    current_block->tacInstructions.push_back(condition);
    current_block->condition = condition;
    Tac *jump_true = new UnCondJump(true_block->name); // Create unconditional jump to true block
    current_block->tacInstructions.push_back(jump_true);

    // Grab second child (body)
    child = getChild(root, 1);

    current_block = true_block;
    tac_from_ast(table, child);
    current_block->tacInstructions.push_back(jump);
    current_block->trueExit = header_block;

    header_block->trueExit = true_block;
    header_block->falseExit = join_block;
    current_block = join_block;

    // Return empty string since while statements dont have values themselves
    return "";
}

// Assign statement handler
std::string handle_assign_statement(SymbolTable *table, Node *root) {
    // Get the name of the variable to assign to by getting the first child (which is the identifier)
    Node* child = getChild(root, 0);
    std::string name = child->value;

    // Get the value to assign (rhs_name) by using recursive propagation on the second child
    child = getChild(root, 1);
    std::string rhs_name = tac_from_ast(table, child);

    // Create and add the copy instruction (since this is an assignment)
    Tac *instruction = new Copy(rhs_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the name of the variable assigned to (for propagation purposes)
    return name;
}

// Array assign statement handler
std::string handle_array_assign_statement(SymbolTable *table, Node *root) {
    // Get the array name by getting the first child (which is the array expression)
    Node* child = getChild(root, 0);
    std::string name = child->value;

    // Get the index of the array to assign to (lhs_name) by using recursive propagation on the second child
    child = getChild(root, 1);
    std::string lhs_name = tac_from_ast(table, child);

    // Get the value to assign (rhs_name) by using recursive propagation on the third child
    child = getChild(root, 2);
    std::string rhs_name = tac_from_ast(table, child);

    // Create and add the copy array instruction (since this is an array assignment)
    Tac *instruction = new CopyArray(lhs_name, rhs_name, name);
    current_block->tacInstructions.push_back(instruction);

    // Return the name of the array assigned to (for propagation purposes)
    return name;
}

// Print statement handler
std::string handle_print_statement(SymbolTable *table, Node *root) {
    // Grab first child (value to print)
    Node* child = getChild(root, 0);

    // Get the name of the value to print using recursive propagation
    std::string name = tac_from_ast(table, child);

    // Create and add the print instruction (since this is a print statement)
    Tac *instruction = new Print(name);
    current_block->tacInstructions.push_back(instruction);

    // Return empty string since print statements dont have
    return "";
}

std::string handle_simple_value(SymbolTable *table, Node *root) {
    // For simple values like identifiers, literals, etc. - just return the value
    return root->value;
}


// --------- CFG file Generation --------- //
void generate_cfg_file() {
    std::ofstream out;
    out.open("cfg.dot");
    rendered_blocks.clear(); // Clear the global rendered_blocks vector

    // Write the DOT file header
    out << "digraph {\ngraph [splines=ortho]\nnode [shape=box]\n";
    
    // Helper function to recursively create the CFG for a block and its children
    std::function<void(BasicBlock*)> create_block_cfg = [&](BasicBlock* block) {
        // Skip if this block has already been rendered
        if (std::find(rendered_blocks.begin(), rendered_blocks.end(), block->name) != rendered_blocks.end()) {
            return;
        }
        
        // Add this block to the rendered list
        rendered_blocks.push_back(block->name);
        
        // Format the block name for Graphviz (replace dots with underscores)
        std::string graphviz_name = block->name;
        size_t dot_pos = graphviz_name.find(".");
        if (dot_pos != std::string::npos) {
            graphviz_name.replace(dot_pos, 1, "_");
        }
        
        // Write the node definition with its instructions
        out << graphviz_name << " [label=\"" << block->name << "\\n";
        for (auto instruction : block->tacInstructions) {
            std::string op = instruction->op;
            std::string lhs = instruction->lhs;
            std::string rhs = instruction->rhs;
            std::string result = instruction->result;
            
            // Escape any characters that might cause problems in DOT
            // For example, replace < with &lt; if needed
            if (op == "<") op = "\\<";
            if (op == ">") op = "\\>";
            
            // out << op << " " << lhs << " " << rhs << " " << result << "\\n";
            out << result << " := " << lhs << " " << op << " " << rhs << "\\n";
        }
        out << "\"];" << std::endl;
        
        // Process true exit if it exists
        if (block->trueExit != nullptr) {
            out << graphviz_name << " -> " << block->trueExit->name << " [xlabel=\"true\"];" << std::endl;
            create_block_cfg(block->trueExit);
        }
        
        // Process false exit if it exists
        if (block->falseExit != nullptr) {
            out << graphviz_name << " -> " << block->falseExit->name << " [xlabel=\"false\"];" << std::endl;
            create_block_cfg(block->falseExit);
        }
    };
    
    // Process all method blocks
    for (auto method : methods) {
        create_block_cfg(method);
    }
    
    // Close the DOT file
    out << "\n}";
    out.close();
    std::cout << "CFG generated in cfg.dot" << std::endl;
}

void create_control_flow_graph(BasicBlock* entry) {
    // Generate a DOT file visualization of the control flow graph
    generate_cfg_file();
}


#endif // ALL_IN_ONE_H