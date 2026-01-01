#include "semantic_analysis.h"
#include "symbol_table.h"


// Helper functions for program structure
std::string check_goal(SymbolTable* table, Node* root) {
    // For Goal node, visit MainClass and ClassDeclarationList
    for (auto child : root->children) {
        if (child->type == "MainClass") {
            // Enter main class scope with the class record
            auto main_class = table->lookup(child->value);
            table->enter_scope("Class: " + child->value, main_class);
            type_checker(table, child);
            table->exit_scope();
        } else {
            type_checker(table, child);
        }
    }
    return "notype";
}

std::string check_main_class(SymbolTable* table, Node* root) {
    // For main class, enter the main method scope
    auto main_method = table->lookup("main");
    table->enter_scope("Method: main", main_method);
    for (auto child : root->children) {
        type_checker(table, child);
    }
    table->exit_scope();
    return "notype";
}

std::string check_class_declaration(SymbolTable* table, Node* root) {
    // For class declaration, enter class scope
    auto class_record = table->lookup(root->value);
    table->enter_scope("Class: " + root->value, class_record);
    
    // First child is VarDeclarationList, second is MethodDeclarationList
    auto var_list = getChild(root, 0);
    auto method_list = getChild(root, 1);
    
    // Process all variable declarations first
    for (auto var_decl : var_list->children) {
        type_checker(table, var_decl);
    }
    
    // Then process all method declarations
    type_checker(table, method_list);
    
    table->exit_scope();
    return "notype";
}

std::string check_class_declaration_list(SymbolTable* table, Node* root) {
    // Visit each class declaration
    for (auto child : root->children) {
        type_checker(table, child);
    }
    return "notype";
}

std::string check_method_declaration(SymbolTable* table, Node* root) {
    // Enter method scope
    int method_line = getChild(root, 0)->lineno;
    auto method_record = table->lookup(root->value);
    table->enter_scope("Method: " + root->value, method_record);
    
    std::string declared_type = getChild(root, 0)->value;
    
    // Process parameters first (index 1)
    type_checker(table, getChild(root, 1));
    
    // Then process method body (index 2)
    type_checker(table, getChild(root, 2));
    
    // Finally check return expression (index 3)
    std::string actual_return_type = type_checker(table, getChild(root, 3));
    
    if (actual_return_type != "error" && actual_return_type != declared_type) {
        std::cerr << "@error at line " << method_line << ". Semantic error: MethodDeclaration return type does not match." << std::endl;
    }
    
    table->exit_scope();
    return declared_type;
}

std::string check_method_declaration_list(SymbolTable* table, Node* root) {
    // Visit each method declaration
    for (auto child : root->children) {
        type_checker(table, child);
    }
    return "notype";
}

// Helper functions for binary operations
std::string check_binary_bool_operation(SymbolTable* table, Node* root, const std::string& operation_name) {
    // Both lhs and rhs must be Bool
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string rhs = type_checker(table, getChild(root, 1));
    if (lhs != "Bool" || rhs != "Bool") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: " << operation_name 
                << " operands must be of type Bool." << std::endl;
    }
    return "Bool";
}

std::string check_binary_int_operation(SymbolTable* table, Node* root, const std::string& operation_name) {
    // Both lhs and rhs must be Int
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string rhs = type_checker(table, getChild(root, 1));
    if (lhs != "Int" || rhs != "Int") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: " << operation_name 
                << " operands must be of type Int." << std::endl;
    }
    return "Int";
}

std::string check_comparison_operation(SymbolTable* table, Node* root, const std::string& operation_name) {
    // Both lhs and rhs must be Int
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string rhs = type_checker(table, getChild(root, 1));
    if (lhs != "Int" || rhs != "Int") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: " << operation_name 
                << " operands must be of type Int." << std::endl;
    }
    return "Bool";
}

std::string check_equals_operation(SymbolTable* table, Node* root) {
    // Both lhs and rhs must be of the same type
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string rhs = type_checker(table, getChild(root, 1));
    if (lhs != rhs) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: EqualsExpression operands must be of the same type." << std::endl;
    }
    return "Bool";
}

// Helper functions for array operations
std::string check_array_expression(SymbolTable* table, Node* root) {
    // Lhs must be IntArray, rhs must be Int
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string rhs = type_checker(table, getChild(root, 1));
    if (lhs != "IntArray") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: ArrayExpression base must be of type IntArray." << std::endl;
    }
    if (rhs != "Int") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: ArrayExpression index must be of type Int." << std::endl;
    }
    return "Int";
}

std::string check_new_int_array_expression(SymbolTable* table, Node* root) {
    // Lhs must be Int
    std::string lhs = type_checker(table, getChild(root, 0));
    if (lhs != "Int") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: NewIntArrayExpression size must be of type Int." << std::endl;
    }
    return "IntArray";
}

// Helper functions for object/method operations
std::string check_method_call_expression(SymbolTable* table, Node* root) {
    // First check if the caller expression is valid and get its type
    std::string caller_type = type_checker(table, getChild(root, 0));
    if (caller_type == "error") {
        return "error";
    }
    
    // Get the class record from the global scope
    Scope* current = table->get_current_scope();
    while (current->get_parent() != nullptr) {
        current = current->get_parent();
    }
    auto class_record = current->records.find(caller_type);
    
    if (class_record == current->records.end() || 
        (class_record->second->record_type != "Class" && class_record->second->record_type != "MainClass")) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Cannot call method on non-class type '" << caller_type << "'." << std::endl;
        return "error";
    }
    
    // Cast to Class* to access class-specific methods
    auto caller_class = static_cast<Class*>(class_record->second);
    
    // Check if the method exists in the class
    auto method_name = getChild(root, 1)->value;
    auto method = caller_class->lookup_method(method_name);
    if (method == nullptr) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Method '" << method_name << "' does not exist in class '" << caller_type << "'." << std::endl;
        return "error";
    }
    
    // Check parameter count
    auto method_parameters = method->parameters;
    auto call_parameters = getChild(root, 2);
    if (method_parameters.size() != call_parameters->children.size()) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Method '" << method_name << "' expects " 
                << method_parameters.size() << " parameters but got " << call_parameters->children.size() << "." << std::endl;
        return "error";
    }
    
    // Check each parameter type
    // For each parameter, check if the type matches the expected type 
    for (size_t i = 0; i < method_parameters.size(); i++) {
        // Get the expected type of the parameter
        auto expected_type = method_parameters[i]->get_type();
        // Get the actual type of the parameter
        auto actual_type = type_checker(table, getChild(call_parameters, i));
        
        if (actual_type == "error") { // If the actual type is an error, return an error (error propagated from leaf node)
            return "error";
        }
        // If the actual type does not match the expected type, return an error
        if (actual_type != expected_type) {
            std::cerr << "@error at line " << root->lineno << ". Semantic error: Parameter " << (i + 1) 
                    << " of method '" << method_name << "' expects type '" << expected_type 
                    << "' but got '" << actual_type << "'." << std::endl;
            // Error propagation
            return "error";
        }
    }
    
    // If all parameters match the expected types, return the type of the method
    return method->get_type();
}

std::string check_length_expression(SymbolTable* table, Node* root) {
    // The lhs must be an IntArray
    std::string base = type_checker(table, getChild(root, 0));
    if (base != "IntArray") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: LengthExpression base must be of type IntArray." << std::endl;
    }
    // Return the type of the length expression
    return "Int";
}

std::string check_new_object_expression(SymbolTable* table, Node* root) {
    // Get the class name from the identifier
    std::string class_name = getChild(root, 0)->value;
    
    // Check if the class exists in the global scope
    Scope* current = table->get_current_scope();
    while (current->get_parent() != nullptr) {
        current = current->get_parent();
    }
    auto class_record = current->records.find(class_name);
    
    if (class_record == current->records.end() || class_record->second->record_type != "Class") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Class '" << class_name << "' does not exist." << std::endl;
        return "error";
    }
    
    // Return the class name as the type
    return class_name;
}

// Helper functions for unary operations
std::string check_not_expression(SymbolTable* table, Node* root) {
    // The lhs must be Bool
    std::string expr = type_checker(table, getChild(root, 0));
    if (expr != "Bool") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: NotExpression operand must be of type Bool." << std::endl;
    }
    return "Bool";
}

// Helper functions for statements
std::string check_if_statement(SymbolTable* table, Node* root) {
    // The expression must be of type Bool (its the condition)
    std::string expr = type_checker(table, getChild(root, 0));
    if (expr != "Bool") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: IfStatement condition must be of type Bool." << std::endl;
    }
    
    // Check the body
    type_checker(table, getChild(root, 1));
    
    return "notype";
}

std::string check_if_else_statement(SymbolTable* table, Node* root) {
    // The expression must be of type Bool (its the condition)
    std::string lhs = type_checker(table, getChild(root, 0));
    if (lhs != "Bool") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: IfElseStatement condition must be of type Bool." << std::endl;
    }
    
    // Check both branches
    type_checker(table, getChild(root, 1));
    type_checker(table, getChild(root, 2));
    
    return "notype";
}

std::string check_while_statement(SymbolTable* table, Node* root) {
    // Lhs must be Bool
    std::string lhs = type_checker(table, getChild(root, 0));
    if (lhs != "Bool") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: WhileStatement condition must be of type Bool." << std::endl;
    }
    
    // Check the body
    type_checker(table, getChild(root, 1));
    
    return "notype";
}

std::string check_assign_statement(SymbolTable* table, Node* root) {
    // lhs must be of the same type as rhs
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string rhs = type_checker(table, getChild(root, 1));
    if (lhs != rhs) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: AssignStatement types do not match. Cannot assign " << rhs << " to " << lhs << "." << std::endl;
    }
    
    // If lhs exists check if it is declared after or before the assign statement
    std::string lhs_name = getChild(root, 0)->value;
    int lhs_lineno = getChild(root, 0)->lineno;
    
    // Check current scope first
    auto scope = table->get_current_scope();
    auto record = scope->records.find(lhs_name);
    
    // If not in current scope, check parent (class) scope
    if (record == scope->records.end() && scope->get_parent() != nullptr) {
        record = scope->get_parent()->records.find(lhs_name);
    }
    
    if (record != scope->records.end()) {
        // Only do the line number check for local variables
        if (record->second->record_type == "Variable" && 
            record->second->line_number > lhs_lineno) {
            std::cerr << "@error at line " << root->lineno << ". Semantic error: Variable '" << lhs_name << "' used before declaration." << std::endl;
            return "error";
        }
    } else {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Variable '" << lhs_name << "' is not defined." << std::endl;
        return "error";
    }

    return "notype";
}

std::string check_array_assign_statement(SymbolTable* table, Node* root) {
    // lhs must be IntArray, rhs must be Int
    std::string lhs = type_checker(table, getChild(root, 0));
    std::string lhs_index = type_checker(table, getChild(root, 1));
    std::string rhs = type_checker(table, getChild(root, 2));
    if (lhs != "IntArray") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: ArrayAssignStatement base must be of type IntArray." << std::endl;
    }
    if (lhs_index != "Int") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: ArrayAssignStatement index must be of type Int." << std::endl;
    }
    if (rhs != "Int") {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: ArrayAssignStatement value must be of type Int." << std::endl;
    }
    return "notype";
}

std::string check_print_statement(SymbolTable* table, Node* root) {
    // The lhs (expression) must be of type Int or Bool
    std::string lhs = type_checker(table, getChild(root, 0));
    if ((lhs == "Int" || lhs == "Bool") == false) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: PrintStatement expression must be of type Int or Bool." << std::endl;
    }
    return "notype";
}

// Helper functions for declarations and literals
std::string check_var_declaration(SymbolTable* table, Node* root) {
    // Get the type node
    Node* type_node = getChild(root, 0);
    std::string type = type_node->value;
    
    // If it's not a primitive type, verify the class exists
    if (type != "Int" && type != "Bool" && type != "IntArray") {
        auto class_record = table->lookup(type);
        if (class_record == nullptr || !class_record->is_valid()) {
            std::cerr << "@error at line " << root->lineno << ". Semantic error: Type '" << type << "' is undefined." << std::endl;
            return "error";
        }
    }
    
    return type;
}

std::string check_method_body(SymbolTable* table, Node* root) {
    // Visit all children
    for (auto child : root->children) {
        type_checker(table, child);
    }
    return "notype";
}

std::string check_identifier(SymbolTable* table, Node* root) {
    auto record = table->lookup(root->value);
    if (record == nullptr || !record->is_valid()) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Identifier " << root->value << " is not defined." << std::endl;
        return "error";
    }
    return record->get_type();
}

std::string check_this_expression(SymbolTable* table, Node* root) {
    auto record = table->lookup(root->value);
    if (record == nullptr || !record->is_valid()) {
        std::cerr << "@error at line " << root->lineno << ". Semantic error: Identifier " << root->value << " is not defined." << std::endl;
        return "error";
    }
    return record->get_type();
}

// Main type checking function
std::string type_checker(SymbolTable* table, Node* root) {
    // Program structure
    if (root->type == "Goal") {
        return check_goal(table, root);
    }
    else if (root->type == "MainClass") {
        return check_main_class(table, root);
    }
    else if (root->type == "ClassDeclaration") {
        return check_class_declaration(table, root);
    }
    else if (root->type == "ClassDeclarationList") {
        return check_class_declaration_list(table, root);
    }
    else if (root->type == "MethodDeclaration") {
        return check_method_declaration(table, root);
    }
    else if (root->type == "MethodDeclarationList") {
        return check_method_declaration_list(table, root);
    }
    
    // Binary operations
    else if (root->type == "AndExpression") {
        return check_binary_bool_operation(table, root, "AndExpression");
    }
    else if (root->type == "OrExpression") {
        return check_binary_bool_operation(table, root, "OrExpression");
    }
    else if (root->type == "LessThanExpression") {
        return check_comparison_operation(table, root, "LessThanExpression");
    }
    else if (root->type == "GreaterThanExpression") {
        return check_comparison_operation(table, root, "GreaterThanExpression");
    }
    else if (root->type == "EqualsExpression") {
        return check_equals_operation(table, root);
    }
    else if (root->type == "PlusOperationExpression") {
        return check_binary_int_operation(table, root, "PlusOperationExpression");
    }
    else if (root->type == "MinusOperationExpression") {
        return check_binary_int_operation(table, root, "MinusOperationExpression");
    }
    else if (root->type == "MultiplicationOperationExpression") {
        return check_binary_int_operation(table, root, "MultiplicationOperationExpression");
    }
    
    // Array operations
    else if (root->type == "ArrayExpression") {
        return check_array_expression(table, root);
    }
    else if (root->type == "NewIntArrayExpression") {
        return check_new_int_array_expression(table, root);
    }
    
    // Object/method operations
    else if (root->type == "MethodCallExpression") {
        return check_method_call_expression(table, root);
    }
    else if (root->type == "LengthExpression") {
        return check_length_expression(table, root);
    }
    else if (root->type == "NewObjectExpression") {
        return check_new_object_expression(table, root);
    }
    
    // Unary operations
    else if (root->type == "NotExpression") {
        return check_not_expression(table, root);
    }
    
    // Statements
    else if (root->type == "IfStatement") {
        return check_if_statement(table, root);
    }
    else if (root->type == "IfElseStatement") {
        return check_if_else_statement(table, root);
    }
    else if (root->type == "WhileStatement") {
        return check_while_statement(table, root);
    }
    else if (root->type == "AssignStatement") {
        return check_assign_statement(table, root);
    }
    else if (root->type == "ArrayAssignStatement") {
        return check_array_assign_statement(table, root);
    }
    else if (root->type == "PrintStatement") {
        return check_print_statement(table, root);
    }
    
    // Declarations and literals
    else if (root->type == "VarDeclaration" || root->type == "Parameter") {
        return check_var_declaration(table, root);
    }
    else if (root->type == "MethodBody") {
        return check_method_body(table, root);
    }
    else if (root->type == "IntLiteralExpression") {
        return "Int";
    }
    else if (root->type == "BoolLiteral") {
        return "Bool";
    }
    else if (root->type == "Identifier") {
        return check_identifier(table, root);
    }
    else if (root->type == "ThisExpression") {
        return check_this_expression(table, root);
    }
    else if (root->type == "ParenthesesExpression") {
        // Return the type of the inner expression
        return type_checker(table, getChild(root, 0));
    }
    else if (root->type == "error") {
        return "error";
    }
    
    return root->value;
}

void run_semantic_analysis(SymbolTable* table, Node* root) {
    /*
    Run the semantic analysis on the AST
    */
    table->reset_table();
    type_checker(table, root);
}

