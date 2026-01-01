#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "Node.h"
#include <unordered_map> // for dictionary
#include <memory>        // for unique_ptr
#include <vector>        // for vector (list)
#include <iostream>      // for std::cout used in print_table
#include <fstream>       // for file I/O
#include <string>        // for std::string

class Record
{
private:
    // Attributes needed for a record according to the assignment
    std::string ID;
    std::string type;

public:
    std::string record_type;
    int line_number;
    bool has_error = false;  // New field to track if this record has errors

    // Constructor
    Record(std::string ID, std::string type);

    // Get attribute functions (since they are private attributes)
    std::string get_ID();
    std::string get_type();

    // Set attribute functions (since they are private attributes)
    void set_ID(std::string ID);
    void set_type(std::string type);

    // Print function
    std::string print_record();

    // Validity check
    virtual bool is_valid() const { return !has_error && !ID.empty() && !type.empty(); }
};

class Variable : public Record
{
    /*
    Class for Variable record.
    Inherits from Record class for the id and type attributes, as well as the Record methods.
    Since this is the lowest level of the hierarchy, it does not have any additional attributes or methods
    except for the constructor.
    */
public:
    // Constructor
    Variable(std::string ID, std::string type);

    // Override is_valid to add Variable-specific checks
    bool is_valid() const override { return Record::is_valid(); }
};

class Method : public Record
{
private:
    // Attributes needed for a method according to the assignment
    std::unordered_map<std::string, Variable *> variables;

public:
    std::vector<Variable *> parameters;

    // Constructor
    Method(std::string ID, std::string type);

    // Add methods for adding variables and parameters
    void add_variable(Variable *new_variable);
    void add_parameter(Variable *new_parameter);

    // Override is_valid to add Method-specific checks
    bool is_valid() const override { return Record::is_valid(); }
};

class Class : public Record
{
private:
    // Attributes needed for a class according to the assignment
    std::unordered_map<std::string, Variable *> variables;
    std::unordered_map<std::string, Method *> methods;

public:
    // Constructor
    Class(std::string ID, std::string type);

    // Add methods for adding variables and methods
    void add_variable(Variable *new_variable);
    void add_method(Method *new_method);

    // Lookup methods for variables and methods
    Variable *lookup_variable(const std::string ID);
    Method *lookup_method(const std::string ID);

    // Override is_valid to add Class-specific checks
    bool is_valid() const override { return Record::is_valid(); }
};

class Scope
{
private:
    // Attributes needed for a scope according to the assignment
    int next = 0;
    Scope *parent_scope;
    std::vector<Scope *> child_scopes;
    

public:
    // idk had to move to public
    std::unordered_map<std::string, Record *> records;

    // Additional attributes for easier printing of the symbol table
    std::string scope_name;
    Record *scope_record;

    // Constructor
    Scope(Scope *parent, std::string scope_name, Record *record);

    // Scope methods as per assignment
    Scope *next_child(std::string scope_name, Record *record);
    Record *lookup(std::string key);
    void reset_scope();

    // Refactored printing methods
    void generate_symbol_tree(int &count, std::ostream *outStream);
    void print_symbol_tree();

    // For adding a record to the scope
    void put(std::string key, Record *item);
    
    // For getting the parent scope (since it is a private attribute)
    Scope *get_parent();
    
    // For getting the scope record
    Record *get_scope_record();
};

class SymbolTable
{
private:
    // Attributes needed for a symbol table according to the assignment
    Scope *root_scope;
    
public:
    Scope *current_scope;

    // Constructor
    SymbolTable();

    // --- Symbol table methods as per assignment ---
    // For entering and exiting scopes
    void enter_scope(std::string scope_name, Record *record);
    void exit_scope();

    // For adding a record to the current scope
    void put(std::string key, Record *item);

    // For looking up a record in the current scope
    Record *lookup(std::string key);

    // Print the entire symbol table
    void print_table();

    // Reset the entire symbol table
    void reset_table();

    Scope *get_current_scope();
};

Node *getChild(Node *node, int index);

void build_symbol_table(Node *root, SymbolTable *table);

#endif