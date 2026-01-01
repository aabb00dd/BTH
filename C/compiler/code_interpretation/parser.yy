%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires{
  #include <string>
  #include "Node.h"
  #define USE_LEX_ONLY false // Change to true if you want to use the lexer only
}

%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
}

// Token declarations. Match the tokens from the lexer
%token <std::string> PLUSOP MINUSOP MULTOP INT LP RP STRING_LITERAL INT_ARRAY
%token <std::string> SYSTEM_OUT_PRINTLN PUBLIC CLASS STATIC VOID MAIN STRING BOOLEAN IF ELSE WHILE TRUE FALSE THIS NEW RETURN LENGTH
%token <std::string> AND OR EQ LT GT ASSIGN NOT LCURLY RCURLY LBRACKET RBRACKET DOT COMMA SEMICOLON INT_LITERAL
%token END 0 "end of file"

// Precedence for operators
%right ASSIGN // Assignment is right associative
%left OR AND // Logical operators have the same precedence, left associative
%left EQ LT GT // Comparison operators have the same precedence, left associative
%left PLUSOP MINUSOP // Addition and subtraction have the same precedence, left associative
%left MULTOP // Multiplication goes before addition and subtraction, left associative 
%right NOT // Not then, right associative since it works on the right operand (!A need to evaluate A first)
%left RP LP // Parentheses then, left associative            
%left LBRACKET RBRACKET // Array brackets then, left associative
%left DOT // Dot operator for method calls goes first and is left associative
%nonassoc ELSE // To remove some shift reduce conflicts for if else statements

// Type declarations for the parser rules
// First row are normal ones, second row are the ones which are recursive list structures needed for the parser where 
// multiple of the same type can be added according to the rules
%type <Node*> root Goal MainClass ClassDeclaration VarDeclaration MethodDeclaration Type Statement Expression Identifier NoSemiVarDeclaration
%type <Node*> ExplicitArgumentList MethodBody StatementList VarDeclarationList ArgumentList ClassDeclarationList MethodParameterList MethodDeclarationList 

%%
// Root node in the parse treem, is just the Goal node
root:
  Goal {
      root= $1;
  };

// Goal node, the root of the parse tree
Goal:
  MainClass ClassDeclarationList END {
    // Create new node for Goal.
    $$ = new Node("Goal", "", yylineno);

    // Add MainClass to node
    $$ ->children.push_back($1);

    // Add ClassDeclarationList to node
    $$ ->children.push_back($2);
  };

MainClass:
  PUBLIC CLASS Identifier LCURLY PUBLIC STATIC VOID MAIN LP STRING LBRACKET RBRACKET Identifier RP LCURLY Statement StatementList RCURLY RCURLY{
    // Create a new node for MainClass
    $$ = new Node("MainClass", $3->value, yylineno);

    // Add the first Statement to the MainClass node
    $$->children.push_back($13);

    // Add the StatementList to the MainClass node
    $$->children.push_back($16);

    // Add the second Statement to the MainClass node
    $$ ->children.push_back($17);
  };


// ClassDeclarationList is a list of ClassDeclaration nodes. Works as a recursive list structure with empty base case
ClassDeclarationList:
  %empty {
    // Create a new node for ClassDeclarationList
    $$ = new Node("ClassDeclarationList", "", yylineno);
  }
  | ClassDeclarationList ClassDeclaration {
    // If there are more ClassDeclarations, add them to the list
    $$ = $1;
    $$->children.push_back($2);
  };


// ClassDeclaration is a node representing a class. Contains a list of VarDeclarations and MethodDeclarations
ClassDeclaration:
  CLASS Identifier LCURLY VarDeclarationList MethodDeclarationList RCURLY {
    // Create a new node for ClassDeclaration. Use identifier when creating the node to store the name of the class
    $$ = new Node("ClassDeclaration", $2->value, yylineno);

    // Add the VarDeclarationList to the ClassDeclaration node
    $$ ->children.push_back($4);

    // Add the MethodDeclarationList to the ClassDeclaration node
    $$ ->children.push_back($5);
  };


// VarDeclarationList is a list of VarDeclaration nodes. Works as a recursive list structure with empty base case
VarDeclarationList:
  %empty {
    // Create a new node for VarDeclarationList
    $$ = new Node("VarDeclarationList", "", yylineno);
  }
  | VarDeclarationList VarDeclaration {
    // If there are more VarDeclarations, add them to the list
    $$ = $1;
    $$->children.push_back($2);
  };


// VarDeclaration is a node representing a variable declaration. Contains a type and an identifier
VarDeclaration:
  Type Identifier SEMICOLON {
    // Create a new node for VarDeclaration.
    $$ = new Node("VarDeclaration", "", yylineno);

    // Add the type to the VarDeclaration node
    $$->children.push_back($1);

    // Add the identifier to the VarDeclaration node
    $$->children.push_back($2);
  };


// Just like VarDeclaration, but without a semicolon at the end. Used in various places where we need to declare variables without a semicolon
// in order to create a list of VarDeclarations
NoSemiVarDeclaration:
  Type Identifier {
    // Create a new node for VarDeclaration.
    $$ = new Node("Parameter", "", yylineno);

    // Add the type to the VarDeclaration node
    $$->children.push_back($1);

    // Add the identifier to the VarDeclaration node
    $$->children.push_back($2);
  };


// MethodParameterList is a list of Type and Identifier combinations. Works as a recursive list structure with empty base case or a type and identifier base case
// Is used in MethodDeclaration to store the parameters of the method since we cannot use a list of var declarations since they end with a semicolon
MethodParameterList:
  %empty {
    // Create a new node for MethodParameterList
    $$ = new Node("MethodParameterList", "", yylineno);
  }
  | NoSemiVarDeclaration {
    // Create a new node for MethodParameterList
    $$ = new Node("MethodParameterList", "", yylineno);

    // Add the type to the MethodParameterList node
    $$->children.push_back($1);
  }
  | MethodParameterList COMMA NoSemiVarDeclaration {
    // Create a new node for MethodParameterList
    $$ = $1;

    // Add the type to the MethodParameterList node
    $$->children.push_back($3);
  };


// MethodBody is a node representing the body of a method. Contains a list of VarDeclarations and Statements as per the grammar
// Works as a recursive list structure with empty base case
MethodBody:
  %empty { 
    // Create a new node for MethodBody
    $$ = new Node("MethodBody", "", yylineno);
  }
  | MethodBody VarDeclaration {
    // If there are more VarDeclarations, add them to the list
    $$ = $1;
    $$->children.push_back($2);
  }
  | MethodBody Statement {
    // If there are more Statements, add them to the list
    $$ = $1;
    $$->children.push_back($2);
  };
  

// MethodDeclarationList is a list of MethodDeclaration nodes. Works as a recursive list structure with empty base case
MethodDeclarationList:
  %empty {
    // Create a new node for MethodDeclarationList
    $$ = new Node("MethodDeclarationList", "", yylineno);
  }
  | MethodDeclarationList MethodDeclaration {
    // If there are more MethodDeclarations, add them to the list
    $$ = $1;
    $$->children.push_back($2);
  };


// Node representing a method declaration. Contains a type, an identifier, a list of parameters, a list of VarDeclarations and a MethodBody
MethodDeclaration:
  PUBLIC Type Identifier LP MethodParameterList RP LCURLY MethodBody RETURN Expression SEMICOLON RCURLY {
    // Create a new node for MethodDeclaration. Use identifier when creating the node to store the name of the method
    $$ = new Node("MethodDeclaration", $3->value, yylineno);

    // Add the type to the MethodDeclaration node
    $$->children.push_back($2);

    // Add the VarDeclarationList to the MethodDeclaration node
    $$->children.push_back($5);

    // Add the MethodBody to the MethodDeclaration node
    $$->children.push_back($8);

    // Add the return Expression to the MethodDeclaration node
    $$->children.push_back($10);
  };


// Type node representing a type. Can be Int, Bool, IntArray or an Identifier as per the grammar
// Simply creates a node with the type as name, no children, and the type as value
Type:
  INT_ARRAY {
    $$ = new Node("Type", "IntArray", yylineno);
  }
  | BOOLEAN {
    $$ = new Node("Type", "Bool", yylineno);
  }
  | INT {
    $$ = new Node("Type", "Int", yylineno);
  }
  | Identifier {
    $$ = new Node("Type", $1->value, yylineno);
  };


// StatementList is a list of Statement nodes. Works as a recursive list structure with empty base case
StatementList:
  %empty {
    // Create a new node for StatementList
    $$ = new Node("StatementList", "", yylineno);
  }
  | StatementList Statement {
    // If there are more Statements, add them to the list
    $$ = $1;
    $$->children.push_back($2);
  };


// Statement node representing a statement
Statement:
  LCURLY StatementList RCURLY {
    // Create a new node to represent a block statement
    $$ = new Node("BlockStatement", "", yylineno);

    $$ = $2;
  }
  | IF LP Expression RP Statement {
    // Create new node for if statement
    $$ = new Node("IfStatement", "", yylineno);

    // Add Expression to node
    $$->children.push_back($3);

    // Add Statement to node
    $$->children.push_back($5);
  }
  | IF LP Expression RP Statement ELSE Statement {
    // Create new node for if else statement
    $$ = new Node("IfElseStatement", "", yylineno);

    // Add Expression to node
    $$->children.push_back($3);

    // Add Statement to node
    $$->children.push_back($5);

    // Add second Statement to node
    $$->children.push_back($7);
  }
  | WHILE LP Expression RP Statement {
    // Create new node for while statement
    $$ = new Node("WhileStatement", "", yylineno);

    // Add Expression to node
    $$->children.push_back($3);

    // Add Statement to node
    $$->children.push_back($5);
  }
  | SYSTEM_OUT_PRINTLN LP Expression RP SEMICOLON {
    // Create new node for print statement
    $$ = new Node("PrintStatement", "", yylineno);

    // Add Expression to node
    $$->children.push_back($3);
  }
  | Identifier ASSIGN Expression SEMICOLON {
    // Create new node for assign statement. Use Identifier as name in node
    $$ = new Node("AssignStatement", "", yylineno);

    // Add Identifier to node
    $$->children.push_back($1);

    // Add Expression to node
    $$->children.push_back($3);
  }
  | Identifier LBRACKET Expression RBRACKET ASSIGN Expression SEMICOLON {
    // Create new node for array assign statment. Use identifier as name in node
    $$ = new Node("ArrayAssignStatement", "", yylineno);

    // Add Identifier to node
    $$->children.push_back($1);

    // Add Expression for node
    $$->children.push_back($3);

    // Add second Expression for node
    $$->children.push_back($6);
  };


// ArgumentList is a list of Expressions. Works as a recursive list structure with empty base case
// This is a special case where we need to call an explicit list since we otherwise would have some ambiguity in the grammar
// the parser would not be able to decide if it should use the Expression rule or the ArgumentList rule when for example parsing a method call
// where the first argument is empty, followed by a comma and then an expression. 
ArgumentList:
    %empty {
      // Create new node for ArgumentList
      $$ = new Node("ArgumentList", "", yylineno);
    }
    | ExplicitArgumentList {
      // If non empty, just return the ExplicitArgumentList, which handles the list of expressions and commas
      $$ = $1;
    };


ExplicitArgumentList:
    Expression {
      // Create new node for ArgumentList
      $$ = new Node("ArgumentList", "", yylineno);

      // Add Expression to node
      $$->children.push_back($1);
    }
    | ExplicitArgumentList COMMA Expression {
      // If there are more expressions, add them to the list
      $$ = $1;

      // Add Expression to node
      $$->children.push_back($3);
    };


// All the different types of expressions. Each expression is a node in the parse tree
Expression:
  Expression AND Expression {
    $$ = new Node("AndExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression OR Expression {
    $$ = new Node("OrExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression LT Expression {
    $$ = new Node("LessThanExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression GT Expression {
    $$ = new Node("GreaterThanExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression EQ Expression {
    $$ = new Node("EqualsExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression PLUSOP Expression {
    $$ = new Node("PlusOperationExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression MINUSOP Expression {
    $$ = new Node("MinusOperationExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression MULTOP Expression {
    $$ = new Node("MultiplicationOperationExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression LBRACKET Expression RBRACKET {
    $$ = new Node("ArrayExpression", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($3);
  }
  | Expression DOT LENGTH {
    $$ = new Node("LengthExpression", "", yylineno);
    $$->children.push_back($1);
  }
  | Expression DOT Identifier LP ArgumentList RP {
    // Create new node for method call expression. Use identifier as name for node
    $$ = new Node("MethodCallExpression", "", yylineno);

    // Add Expression to node
    $$->children.push_back($1);

    // Add Identifier to node
    $$->children.push_back($3);

    // Add ArgumentList to node
    $$ ->children.push_back($5);
  }
  | INT_LITERAL {
    // Create new node for int literal. Use the literal int as name
    $$ = new Node("IntLiteralExpression", $1, yylineno);
  }
  | TRUE {
    $$ = new Node("BoolLiteral", "true", yylineno);
  }
  | FALSE {
    $$ = new Node("BoolLiteral", "false", yylineno);
  }
  | Identifier {
    // In this case its just Identifier
    $$ = $1;
  }
  | THIS {
    $$ = new Node("ThisExpression", "this", yylineno);
  }
  | NEW INT LBRACKET Expression RBRACKET {
    $$ = new Node("NewIntArrayExpression", "", yylineno);
    $$->children.push_back($4);
  }
  | NEW Identifier LP RP {
    $$ = new Node("NewObjectExpression", "", yylineno);

    // Add Identifier to node
    $$->children.push_back($2);
  }
  | NOT Expression {
    $$ = new Node("NotExpression", "", yylineno);
    $$->children.push_back($2);
  }
  | LP Expression RP {
    // In this case its just the Expression since we need to evaluate the expression in the parentheses first
    $$ = new Node("ParenthesesExpression", "", yylineno);
    $$->children.push_back($2);
  }


// Identifier node representing an identifier. Contains the name of the identifier
Identifier:
  STRING_LITERAL {
    // Create new node for string literal. Use the literal string as name
    $$ = new Node("Identifier", $1, yylineno);
  };

%%