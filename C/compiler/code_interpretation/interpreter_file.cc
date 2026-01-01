#include "all_in_one.h"
#include <unordered_map>
#include <algorithm> 
#include <vector>
#include <string>
#include <stack>

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


// --------- Interpreter --------- //
// Forward declarations
std::vector<std::string> split_string(std::string input, std::string delimiter);

bool handleMethodHeader(const std::string& line, Program_cfg* program, 
    Multi_Method_cfg*& current_method, Method_cfg*& current_block);
bool handleBlockHeader(const std::string& line, Multi_Method_cfg*& current_method, Method_cfg*& current_block);
void processInstruction(const std::string& line, Multi_Method_cfg* current_method, Method_cfg* current_block);

Program_cfg *read_bytecode_file(const char *filename);

void interpreter(Program_cfg* program);


// Classes
class Activation {
    public:
        Multi_Method_cfg *method;
        std::unordered_map<std::string, int> variables; // Variables for the current methods
        int program_counter = 0; // Program counter for the current method. Tracks the current instruction being executed

        // Constructor
        Activation(Multi_Method_cfg* _method) {
            // Initialize variables to 0 and store the methods
            method = _method;
            for (auto var : _method->variables) {
                variables[var] = 0;
            }
        }

        // Get the next instruction in the current method
        Instruction_cfg* get_next_instruction() {
            return method->get_instruction_at_program_counter(program_counter++);
        }

        // Jump to a specific block in the current method
        void jump(std::string block_name) {
            program_counter = method->calculate_program_counter(block_name);
        }
};

// Main function (always reads the output.bc file and starts the interpreter)
int main(int argc, char** argv) {
    std::string filename = "output.bc";
    Program_cfg* program = read_bytecode_file(filename.c_str());
    try {
        interpreter(program);
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
};

// Read bytecode file and return the program CFG
Program_cfg* read_bytecode_file(const char* filename) {
    Program_cfg* program = new Program_cfg();
    Multi_Method_cfg* current_method = nullptr;
    Method_cfg* current_block = nullptr;
    ifstream infile(filename);
    
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open bytecode file: " + std::string(filename));
    }
    
    std::string line;
    while (getline(infile, line)) {
        if (line.empty()) {
            continue;
        }
        
        if (handleMethodHeader(line, program, current_method, current_block)) {
            continue;
        }
        
        if (handleBlockHeader(line, current_method, current_block)) {
            continue;
        }
        
        // Handle instruction
        processInstruction(line, current_method, current_block);
    }
    
    return program;
}

// Handle method header and block header
bool handleMethodHeader(const std::string& line, Program_cfg* program, 
            Multi_Method_cfg*& current_method, Method_cfg*& current_block) {
    // Check for main method
    if (line == "main:") {
        // If main method, create a new method and block (single method) and add it to the program
        current_method = new Multi_Method_cfg();
        current_block = new Method_cfg();
        current_block->method_name = "main";
        current_method->methods.push_back(current_block);
        program->methods["main"] = current_method;
        return true;
    }
    
    // Check for class method (contains "." and ":")
    if (line.find(".") != std::string::npos && line.find(":") != std::string::npos) {
        // If it contains both, it is a class method, create a new method and block and add it to the program
        std::string method_name = line.substr(0, line.length() - 1); // Remove trailing ":"
        current_method = new Multi_Method_cfg();
        current_block = new Method_cfg();
        current_block->method_name = method_name;
        current_method->methods.push_back(current_block);
        program->methods[method_name] = current_method;
        return true;
    }
    
    return false;
}

// Handle block header
bool handleBlockHeader(const std::string& line, Multi_Method_cfg*& current_method, Method_cfg*& current_block) {
    // Check for block header (contains ":"), if found, create a new block and add it to the current method
    if (line.find(":") != std::string::npos) {
        std::string block_name = line.substr(0, line.length() - 1); // Remove trailing ":"
        current_block = new Method_cfg();
        current_block->method_name = block_name;
        current_method->methods.push_back(current_block);
        return true;
    }
    return false;
}

// Process instruction
void processInstruction(const std::string& line, Multi_Method_cfg* current_method, Method_cfg* current_block) {
    // Create a new instruction
    Instruction_cfg* instruction = new Instruction_cfg();

    // Split the line by space to get the instruction and argument
    std::vector<std::string> line_split = split_string(line, " ");
    
    // Check if the line is empty. If so, delete the instruction and return (sad noises)
    if (line_split.empty()) {
        delete instruction;
        return;
    }
    
    // Set the instruction id
    instruction->id = op_str_to_int.at(line_split[0]);
    
    // Set the argument if it exists
    if (line_split.size() > 1) {
        instruction->argument = line_split[1];
        
        // Track variables used in load/store operations
        if ((line_split[0] == "iload" || line_split[0] == "istore") 
            && std::find(current_method->variables.begin(), current_method->variables.end(), 
                    line_split[1]) == current_method->variables.end()) {
            current_method->variables.push_back(line_split[1]);
        }
    }
    
    // Add the instruction to the current block
    current_block->instructions.push_back(instruction);
}


// Helper functions for interpreter
void handleStackOperation(std::stack<int>& data_stack, int operation) {
    // Standard poppin 'n pushin'
    int v1 = data_stack.top();
    data_stack.pop();
    int v2 = data_stack.top();
    data_stack.pop();
    
    // Handle the different operations
    int result = 0;
    switch(operation) {
        case IADD: result = v1 + v2; break;
        case ISUB: result = v2 - v1; break;
        case IMUL: result = v2 * v1; break;
        case ILT:  result = v2 < v1; break;
        case IGT:  result = v2 > v1; break;
        case IEQ:  result = v2 == v1; break;
        case IAND: result = v2 * v1 != 0; break;
        case IOR:  result = v2 + v1 != 0; break;
    }
    
    // Push the result back to the stack
    data_stack.push(result);
}

void handleVariableOperation(std::stack<int>& data_stack, Activation* current_activation, 
            int operation, const std::string& argument) {
    switch(operation) {
        case ILOAD:
            // If iload, push the variable value to the stack
            data_stack.push(current_activation->variables[argument]);
            break;
        case ISTORE:
            // If istore, pop the value from the stack and store it in the variable
            current_activation->variables[argument] = data_stack.top();
            data_stack.pop();
            break;
        case ICONST:
            // If iconst, push the constant value to the stack
            data_stack.push(stoi(argument));
            break;
    }
}

void handleControlFlow(std::stack<int>& data_stack, Activation* current_activation,
            int operation, const std::string& argument) {
    switch(operation) {
        case GOTO:
            // If goto, jump to the block with the given label
            current_activation->jump(argument);
            break;
        case IFFALSE: {
            // If iffalse, pop the value from the stack and jump if it is 0
            int v = data_stack.top();
            data_stack.pop();
            if (v == 0) {
                current_activation->jump(argument);
            }
            break;
        }
    }
}

Activation* handleMethodCall(std::stack<Activation*>& activation_stack, Activation* current_activation,
            Program_cfg* program, int operation, const std::string& argument) {
    switch(operation) {
        case INVOKEVIRTUAL: {
            // If invokevirtual, push the current activation to the stack and switch to the new method
            activation_stack.push(current_activation);
            Multi_Method_cfg *call_method = program->methods[argument];
            return new Activation(call_method);
        }
        case IRETURN:
            // If ireturn, pop the activation from the stack and continue
            current_activation = activation_stack.top();
            activation_stack.pop();
            return current_activation;
        default:
            return current_activation;
    }
}

void interpreter(Program_cfg* program) {
    std::stack<Activation*> activation_stack;
    std::stack<int> data_stack;
    Activation *current_activation = new Activation(program->methods["main"]);

    int instruction_id = -1;

    while (instruction_id != STOP) {
        Instruction_cfg *instruction = current_activation->get_next_instruction();
        instruction_id = instruction->id;

        // Handle different instruction types
        if (instruction_id == IADD || instruction_id == ISUB || instruction_id == IMUL || 
            instruction_id == ILT || instruction_id == IGT || instruction_id == IEQ 
            || instruction_id == IAND || instruction_id == IOR) {
            handleStackOperation(data_stack, instruction_id);
        } 
        else if (instruction_id == ILOAD || instruction_id == ISTORE || instruction_id == ICONST) {
            handleVariableOperation(data_stack, current_activation, instruction_id, instruction->argument);
        }
        else if (instruction_id == GOTO || instruction_id == IFFALSE) {
            handleControlFlow(data_stack, current_activation, instruction_id, instruction->argument);
        }
        else if (instruction_id == INVOKEVIRTUAL || instruction_id == IRETURN) {
            current_activation = handleMethodCall(activation_stack, current_activation, program, 
                                                instruction_id, instruction->argument);
        }
        else if (instruction_id == INOT) {
            int v = data_stack.top();
            data_stack.pop();
            data_stack.push(v == 0);
        }
        else if (instruction_id == PRINT) {
            int v = data_stack.top();
            data_stack.pop();
            std::cout << v << std::endl;
        }
        // STOP case handled by the while loop condition
    }
    
    // Clean up memory
    delete current_activation;
}

// String splitter helper needed for getting instruction and argument from instruction line
std::vector<std::string> split_string(std::string input, std::string delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = input.find(delimiter);
    
    while (end != std::string::npos) {
        if (end > start) {
            result.push_back(input.substr(start, end - start));
        }
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }
    
    // Add the last part if there is any
    if (start < input.length()) {
        result.push_back(input.substr(start));
    }
    
    return result;
}