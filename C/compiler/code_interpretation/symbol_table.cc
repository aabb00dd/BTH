#include "symbol_table.h"

// Record class implementations
Record::Record(std::string ID, std::string type)
{
    // Just set the Identifier and type attributes for the record object
    this->ID = ID;
    this->type = type;
}
std::string Record::get_ID()
{
    // Just return the Identifier attritbute of the record object
    return ID;
}
std::string Record::get_type()
{
    // Just return the type attribute of the record object
    return type;
}
void Record::set_ID(std::string ID)
{
    // Just set the Identifier attribute of the record object using this
    this->ID = ID;
}
void Record::set_type(std::string type)
{
    // Just set the type attribute of the record object using this
    this->type = type;
}
std::string Record::print_record(){
    // Just print the Identifier and type attributes of the record object
    return "name: " + ID + "; record " + record_type + "; type: " + type;
}

// Variable class implementations
Variable::Variable(std::string ID, std::string type) : Record(ID, type)
{
    record_type = "Variable";
}

// Method class implementations
Method::Method(std::string ID, std::string type) : Record(ID, type)
{
    record_type = "Method";
}
void Method::add_variable(Variable *new_variable)
{
    // We simply insert the new variable into the variables map using the Identifier as the key
    variables.insert({new_variable->get_ID(), new_variable});
}
void Method::add_parameter(Variable *new_parameter)
{
    // We simply insert the new parameter into the parameters map using the Identifier as the key
    parameters.push_back(new_parameter);
}

// Class class implementations
Class::Class(std::string ID, std::string type) : Record(ID, type)
{
    record_type = "Class";
}
void Class::add_variable(Variable *new_variable)
{
    // We simply insert the new variable into the variables map using the Identifier as the key
    variables.insert({new_variable->get_ID(), new_variable});
}
void Class::add_method(Method *new_method)
{
    // We simply insert the new method into the methods map using the Identifier as the key
    methods.insert({new_method->get_ID(), new_method});
}
Variable *Class::lookup_variable(const std::string ID)
{
    if (variables.find(ID) == variables.end())
    {
        return nullptr;
    }
    else
    {
        return variables[ID];
    }
}
Method *Class::lookup_method(const std::string ID)
{
    if (methods.find(ID) == methods.end())
    {
        return nullptr;
    }
    else
    {
        return methods[ID];
    }
}

// Scope class implementations
Scope::Scope(Scope *parent, std::string scope_name, Record *record)
{
    // Simply set the parent scope attribute of the scope object
    this->parent_scope = parent;
    this->scope_name = scope_name;
    this->scope_record = record;
}
Scope *Scope::next_child(std::string scope_name, Record *record)
{
    // Initialize a new child scope object
    Scope *next_child_scope;
    // If next is the size of the child scopes vector, then we need to create a new scope object
    // since the child scope vector is 0-indexed (i.e. the first element is at index 0)
    if (next == child_scopes.size())
    {
        
        // If the next index is the size of the child scopes vector, then we need to create a new scope object
        // and push it into the child scopes vector
        next_child_scope = new Scope(this, scope_name, record);
        child_scopes.push_back(next_child_scope);
    }
    else
    {
        // If the next index is not the size of the child scopes vector, then we can simply return the child scope at the next index
        next_child_scope = child_scopes[next];
    }
    // Increment the next index
    next++;
    // Return the next child scope object
    return next_child_scope;
}
Record *Scope::lookup(std::string key)
{
    // If the key is found in the records map, then we return the record object
    if (records.find(key) != records.end())
    {
        return records[key];
    }
    // If the key is not found in the records map, then we need to check the parent scope (recursively)
    else
    {
        // If the parent scope is null, then we return null
        if (parent_scope == nullptr)
        {
            return nullptr;
        }
        else
        {
            // If the parent scope is not null, then we recursively lookup on the parent scope
            return parent_scope->lookup(key);
        }
    }
}
void Scope::reset_scope()
{
    // We simply reset the next index to 0 for all the child scopes
    next = 0;

    // We also reset the next index for all the child scopes
    // (which will recursively reset the next index for all the child scopes of the child scopes)
    for (Scope *child_scope : child_scopes)
    {
        child_scope->reset_scope();
    }
}
void Scope::generate_symbol_tree(int &count, std::ostream *outStream) {
    // Print the records in the current scope
    int id = count++;

    *outStream << "n" << id << " [label=\"Symbol table: (" << scope_name << ")\\n";
    for (auto i = records.begin(); i != records.end(); ++i) {
        *outStream << records[i->first]->print_record() << "\\n";
    }
    *outStream << "\"];" << std::endl;

    // Process all child scopes recursively
    for (auto i = child_scopes.begin(); i != child_scopes.end(); i++) {
        int child_id = count;
        (*i)->generate_symbol_tree(count, outStream);
        *outStream << "n" << id << " -> n" << child_id << std::endl;
    }
}
void Scope::print_symbol_tree() {
    std::ofstream outStream;
    char* filename = "table.dot";
    outStream.open(filename);

    int count = 0;
    outStream << "digraph {" << std::endl;
    generate_symbol_tree(count, &outStream);
    outStream << "}" << std::endl;
    outStream.close();

    printf("\nBuilt a symbol-table at %s. Use 'make table' to generate the pdf version.\n", filename);
}
void Scope::put(std::string key, Record *item)
{
    // Simply insert the record object into the records map using the key as the key
    // Using insert here should prevent overwriting records with the same key
    records[key] = item;
}
Scope *Scope::get_parent()
{
    // Simply return the parent scope attribute of the scope object
    return parent_scope;
}
Record *Scope::get_scope_record()
{
    // Simply return the scope record attribute of the scope object
    return scope_record;
}

// SymbolTable class implementations
SymbolTable::SymbolTable()
{
    // Initialize the root scope and current scope attributes of the symbol table object
    root_scope = new Scope(nullptr, "Goal", nullptr);
    current_scope = root_scope;
}
void SymbolTable::enter_scope(std::string scope_name, Record *record)
{
    // Simply set the current scope attribute of the symbol table object to the next child scope
    current_scope = current_scope->next_child(scope_name, record);
}
void SymbolTable::exit_scope()
{
    // Simply set the current scope attribute of the symbol table object to the parent scope
    current_scope = current_scope->get_parent();
}
void SymbolTable::put(std::string key, Record *item)
{
    // Simply call the put method of the current scope object with the key and item as arguments
    current_scope->put(key, item);
}
Record *SymbolTable::lookup(std::string key)
{
    // Simply call the lookup method of the current scope object with the key as an argument
    return current_scope->lookup(key);
}
void SymbolTable::print_table()
{
    std::cout << "\nGenerating symbol table visualization..." << std::endl;
    root_scope->print_symbol_tree();
}
void SymbolTable::reset_table()
{
    // Simply call the reset scope method of the root scope object
    // (which will recursively reset all the child scopes, effectively resetting the entire symbol table)
    root_scope->reset_scope();
}
Scope* SymbolTable::get_current_scope() {
    return current_scope;
}

// Main function to build the symbol table from the AST
// Helper function to access the nth child from a Node's children list.
Node *getChild(Node *node, int index)
{
    auto it = node->children.begin();
    std::advance(it, index);
    return *it;
}

// --- Build symbol table stuff --- //
void build_symbol_table(Node *root, SymbolTable *table)
{
    // Check if the root even exist. If not, fail fast
    if (!root)
    {
        return;
    }
    // For the goal node in the syntax tree
    if (root->type == "Goal")
    {   
        // Add the main class to the symbol table (this is a special class that is the entry point of the program)
        auto main_class = new Class(getChild(root, 0)->value, getChild(root, 0)->value);
        // Set the record type of the main class to "MainClass"
        main_class->record_type = "MainClass";
        // Add the main class to the symbol table
        table->put(getChild(root, 0)->value, main_class);
        // Enter the scope of the main class
        table->enter_scope("Class: " + getChild(root, 0)->value, main_class);
        // Build the symbol table for the main class (recursively)
        build_symbol_table(getChild(root, 0), table);
        // Exit the scope of the main class
        table->exit_scope();

        // Then do the class declarations (ClassDeclarationList)
        build_symbol_table(getChild(root, 1), table);
    }

    // If the node is a Main Class
    else if (root->type == "MainClass")
    {
        // Add the "this" variable to the symbol table (this is a special variable that refers to the current object)
        table->put("this", new Variable("this", root->value));
        // Add the "main" method to the symbol table (this is a special method that is the entry point of the program)
        table->put("main", new Method("main", "void"));
        // Enter the scope of the main method
        table->enter_scope("Method: main", nullptr);
        // Add the "args" variable to the symbol table (this is a special variable that holds the command line arguments)
        table->put(getChild(root, 0)->value, new Variable(getChild(root, 0)->value, "String[]"));
        // Exit the scope of the main method
        table->exit_scope();
    }
    else if (root->type == "ClassDeclarationList")
    {
        // Go through the children of the ClassDeclarationList node and add them to the symbol table
        // The classdeclarationlist is always a list of classes in the goal node
        for (Node *child : root->children)
        {
            // Only check for duplicates in the global scope
            auto existing_class = table->get_current_scope()->records.find(child->value); // Find the existing class in the symbol table
            if (existing_class != table->get_current_scope()->records.end()) // If the class is found
            {
                std::cerr << "@error at line " << getChild(child, 0)->lineno << ". Semantic error: Duplicate class name " << child->value << "." << std::endl;
                existing_class->second->has_error = true;  // Mark the existing class as invalid (needed for special case in semantic analysis)
            }
            
            // Create a new class object with the Identifier of the child as the name and type
            auto new_class = new Class(child->value, child->value);
            if (existing_class != table->get_current_scope()->records.end()) {
                new_class->has_error = true;  // Mark the new class as invalid if it's a duplicate
            }

            // Insert the new class object into the symbol table
            table->put(child->value, new_class);

            // Enter the scope of the new class
            table->enter_scope("Class: " + child->value, new_class);

            // Build the symbol table for the class (recursively)
            build_symbol_table(child, table);

            // Exit the scope of the new class
            table->exit_scope();
        }
    }
    else if (root->type == "ClassDeclaration")
    {
        // The class node has already been added to the symbol table in the ClassDeclarationList case
        // This is the root of the tree for the class
        // We need to add the "this" variable to the symbol table for the class
        table->put("this", new Variable("this", root->value));

        // In the new scope we need to add the variables and methods of the class to the symbol table
        // by recursively building the symbol table for the children of the class
        for (Node *child : root->children)
        {
            build_symbol_table(child, table);
        }
    }
    else if (root->type == "MethodDeclarationList")
    {
        for (Node* child : root->children) {
            // Use the method declaration's own line number
            int declaration_line = getChild(child, 0)->lineno;
            
            // Only check for duplicates in the current class scope
            auto current_class = static_cast<Class*>(table->get_current_scope()->get_scope_record());
            // Find the existing method in the current class
            auto existing_method = current_class->lookup_method(child->value);
            
            if (existing_method != nullptr) {
                std::cerr << "@error at line " << declaration_line << ". Semantic error: Duplicate method name " << child->value << " in class " << current_class->get_ID() << "." << std::endl;
                existing_method->has_error = true;
            }
                
            // Method ID is value (name) of the method, type is the first child's value (the return type)
            auto new_method = new Method(child->value, getChild(child, 0)->value);
            if (existing_method != nullptr) {
                new_method->has_error = true;  // Mark as invalid if it's a duplicate in same class
            }
            table->put(child->value, new_method);
            
            // Add the method to the current class
            current_class->add_method(new_method);
            
            // Enter the scope of the new method
            table->enter_scope("Method: " + child->value, new_method);
            
            // Build the symbol table for the method (recursively)
            build_symbol_table(child, table);
            
            // Exit the scope of the new method
            table->exit_scope();
        }
    }
    else if (root->type == "MethodParameterList") {
        for (Node *child : root->children) {
            build_symbol_table(child, table);
            auto new_parameter = new Variable(getChild(child, 1)->value, getChild(child, 0)->value);
            new_parameter->record_type = "Parameter";
            
            // Add to method's parameters list
            ((Method*)table->get_current_scope()->get_scope_record())->add_parameter(new_parameter);
            
            // Add to current scope
            table->put(getChild(child, 1)->value, new_parameter);
        }
    }
    else if (root->type == "VarDeclaration")
    {
        // Only check for duplicates in the current scope, not parent scopes
        auto existing_record = table->get_current_scope()->records.find(getChild(root, 1)->value);
        if (existing_record != table->get_current_scope()->records.end()) {
            std::cerr << "@error at line " << root->lineno << ". Semantic error: Duplicate variable name " << getChild(root, 1)->value << " in current scope." << std::endl;
            existing_record->second->has_error = true;
        }
        
        auto new_variable = new Variable(getChild(root, 1)->value, getChild(root, 0)->value);
        if (existing_record != table->get_current_scope()->records.end()) {
            new_variable->has_error = true;  // Mark as invalid only if duplicate in same scope
        }
        new_variable->line_number = root->lineno;
        table->put(getChild(root, 1)->value, new_variable);
    }
    else if (root->type == "Parameter")
    {
        // For parameters, we only check for conflicts with other parameters in the same method scope
        // NOT with class fields
        auto existing_record = table->get_current_scope()->records.find(getChild(root, 1)->value);
        if (existing_record != table->get_current_scope()->records.end()) {
            std::cerr << "@error at line " << root->lineno << ". Semantic error: Duplicate parameter name " << getChild(root, 1)->value << "." << std::endl;
            existing_record->second->has_error = true;
        }
        
        auto new_parameter = new Variable(getChild(root, 1)->value, getChild(root, 0)->value);
        new_parameter->record_type = "Parameter";
        if (existing_record != table->get_current_scope()->records.end()) {
            new_parameter->has_error = true;  // Mark as invalid only if duplicate in same scope
        }
        table->put(getChild(root, 1)->value, new_parameter);
    }
    else {
        // For all other cases, just build the symbol table for the children of the node
        for (Node *child : root->children)
        {
            build_symbol_table(child, table);
        }
    }
}