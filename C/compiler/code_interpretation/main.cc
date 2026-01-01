#include <iostream>
#include "parser.tab.hh"
#include "Node.h"
#include "symbol_table.h"
#include "semantic_analysis.h"
#include "all_in_one.h"

extern Node *root;
extern FILE *yyin;
extern int yylineno;
extern int lexical_errors;
extern yy::parser::symbol_type yylex();

// Error codes
enum errCodes
{
    SUCCESS = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    AST_ERROR = 3,
    SEMANTIC_ERROR = 4,
    SEGMENTATION_FAULT = 139
};

int errCode = errCodes::SUCCESS;

// Handling Syntax Errors
void yy::parser::error(std::string const &err)
{
    if (!lexical_errors)
    {
        std::cerr << "Syntax errors found! See the logs below:" << std::endl;
        std::cerr << "\t@error at line " << yylineno << ". Cannot generate a syntax tree for this input: " << err.c_str() << std::endl;
        std::cerr << "End of syntax errors!" << std::endl;
        errCode = errCodes::SYNTAX_ERROR;
    }
}

int main(int argc, char **argv)
{
    // Reads from file if a file name is passed as an argument. Otherwise, reads from stdin.
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r")))
        {
            perror(argv[1]);
            return 1;
        }
    }

    if (USE_LEX_ONLY)
    {
        yylex();
        if (lexical_errors)
        {
            errCode = errCodes::LEXICAL_ERROR;
        }
    }
    else
    {
        yy::parser parser;

        bool parseSuccess = !parser.parse();

        if (lexical_errors)
        {
            errCode = errCodes::LEXICAL_ERROR;
        }

        if (parseSuccess && !lexical_errors)
        {
            std::cout << "Successfully generated syntax tree. \n" << std::endl;
            try
            {
                // root->print_tree();
                root->generate_tree();
                SymbolTable *symbol_table = new SymbolTable();
                build_symbol_table(root, symbol_table);
                symbol_table->print_table();
                run_semantic_analysis(symbol_table, root);

                // part 3 stuff
                BasicBlock* entry = new BasicBlock("main");
                current_block = entry;
                methods.push_back(current_block);
                symbol_table->reset_table();
                tac_from_ast(symbol_table, root);
                create_control_flow_graph(entry);
                Program_cfg* program = new Program_cfg();
                BasicBlock::generate_bytecode(program);
                program->print();
            }
            catch (...)
            {
                errCode = errCodes::AST_ERROR;
            }

        }
    }
    return errCode;
}