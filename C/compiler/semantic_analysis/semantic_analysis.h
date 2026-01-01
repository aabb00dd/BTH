#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include "symbol_table.h"
#include "stdlib.h"
#include "Node.h"

// Main type checking function
std::string type_checker(SymbolTable* table, Node* root);

// Helper functions for program structure
std::string check_goal(SymbolTable* table, Node* root);
std::string check_main_class(SymbolTable* table, Node* root);
std::string check_class_declaration(SymbolTable* table, Node* root);
std::string check_class_declaration_list(SymbolTable* table, Node* root);
std::string check_method_declaration(SymbolTable* table, Node* root);
std::string check_method_declaration_list(SymbolTable* table, Node* root);

// Helper functions for binary operations
std::string check_binary_bool_operation(SymbolTable* table, Node* root, const std::string& operation_name);
std::string check_binary_int_operation(SymbolTable* table, Node* root, const std::string& operation_name);
std::string check_comparison_operation(SymbolTable* table, Node* root, const std::string& operation_name);
std::string check_equals_operation(SymbolTable* table, Node* root);

// Helper functions for array operations
std::string check_array_expression(SymbolTable* table, Node* root);
std::string check_new_int_array_expression(SymbolTable* table, Node* root);

// Helper functions for object/method operations
std::string check_method_call_expression(SymbolTable* table, Node* root);
std::string check_length_expression(SymbolTable* table, Node* root);
std::string check_new_object_expression(SymbolTable* table, Node* root);

// Helper functions for unary operations
std::string check_not_expression(SymbolTable* table, Node* root);

// Helper functions for statements
std::string check_if_statement(SymbolTable* table, Node* root);
std::string check_if_else_statement(SymbolTable* table, Node* root);
std::string check_while_statement(SymbolTable* table, Node* root);
std::string check_assign_statement(SymbolTable* table, Node* root);
std::string check_array_assign_statement(SymbolTable* table, Node* root);
std::string check_print_statement(SymbolTable* table, Node* root);

// Helper functions for declarations and literals
std::string check_var_declaration(SymbolTable* table, Node* root);
std::string check_method_body(SymbolTable* table, Node* root);
std::string check_identifier(SymbolTable* table, Node* root);
std::string check_this_expression(SymbolTable* table, Node* root);

// Main entry point for semantic analysis
void run_semantic_analysis(SymbolTable* table, Node* root);

#endif // SEMANTIC_ANALYSIS_H