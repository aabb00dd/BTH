// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 13 "parser.yy"

  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;

#line 54 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 127 "parser.tab.cc"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_NoSemiVarDeclaration: // NoSemiVarDeclaration
      case symbol_kind::S_MethodParameterList: // MethodParameterList
      case symbol_kind::S_MethodBody: // MethodBody
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_ArgumentList: // ArgumentList
      case symbol_kind::S_ExplicitArgumentList: // ExplicitArgumentList
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_Identifier: // Identifier
        value.YY_MOVE_OR_COPY< Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_INT_ARRAY: // INT_ARRAY
      case symbol_kind::S_SYSTEM_OUT_PRINTLN: // SYSTEM_OUT_PRINTLN
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_NoSemiVarDeclaration: // NoSemiVarDeclaration
      case symbol_kind::S_MethodParameterList: // MethodParameterList
      case symbol_kind::S_MethodBody: // MethodBody
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_ArgumentList: // ArgumentList
      case symbol_kind::S_ExplicitArgumentList: // ExplicitArgumentList
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_Identifier: // Identifier
        value.move< Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_INT_ARRAY: // INT_ARRAY
      case symbol_kind::S_SYSTEM_OUT_PRINTLN: // SYSTEM_OUT_PRINTLN
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_NoSemiVarDeclaration: // NoSemiVarDeclaration
      case symbol_kind::S_MethodParameterList: // MethodParameterList
      case symbol_kind::S_MethodBody: // MethodBody
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_ArgumentList: // ArgumentList
      case symbol_kind::S_ExplicitArgumentList: // ExplicitArgumentList
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_Identifier: // Identifier
        value.copy< Node* > (that.value);
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_INT_ARRAY: // INT_ARRAY
      case symbol_kind::S_SYSTEM_OUT_PRINTLN: // SYSTEM_OUT_PRINTLN
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_NoSemiVarDeclaration: // NoSemiVarDeclaration
      case symbol_kind::S_MethodParameterList: // MethodParameterList
      case symbol_kind::S_MethodBody: // MethodBody
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_ArgumentList: // ArgumentList
      case symbol_kind::S_ExplicitArgumentList: // ExplicitArgumentList
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_Identifier: // Identifier
        value.move< Node* > (that.value);
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_INT_ARRAY: // INT_ARRAY
      case symbol_kind::S_SYSTEM_OUT_PRINTLN: // SYSTEM_OUT_PRINTLN
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_Goal: // Goal
      case symbol_kind::S_MainClass: // MainClass
      case symbol_kind::S_ClassDeclarationList: // ClassDeclarationList
      case symbol_kind::S_ClassDeclaration: // ClassDeclaration
      case symbol_kind::S_VarDeclarationList: // VarDeclarationList
      case symbol_kind::S_VarDeclaration: // VarDeclaration
      case symbol_kind::S_NoSemiVarDeclaration: // NoSemiVarDeclaration
      case symbol_kind::S_MethodParameterList: // MethodParameterList
      case symbol_kind::S_MethodBody: // MethodBody
      case symbol_kind::S_MethodDeclarationList: // MethodDeclarationList
      case symbol_kind::S_MethodDeclaration: // MethodDeclaration
      case symbol_kind::S_Type: // Type
      case symbol_kind::S_StatementList: // StatementList
      case symbol_kind::S_Statement: // Statement
      case symbol_kind::S_ArgumentList: // ArgumentList
      case symbol_kind::S_ExplicitArgumentList: // ExplicitArgumentList
      case symbol_kind::S_Expression: // Expression
      case symbol_kind::S_Identifier: // Identifier
        yylhs.value.emplace< Node* > ();
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_STRING_LITERAL: // STRING_LITERAL
      case symbol_kind::S_INT_ARRAY: // INT_ARRAY
      case symbol_kind::S_SYSTEM_OUT_PRINTLN: // SYSTEM_OUT_PRINTLN
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_EQ: // EQ
      case symbol_kind::S_LT: // LT
      case symbol_kind::S_GT: // GT
      case symbol_kind::S_ASSIGN: // ASSIGN
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_LCURLY: // LCURLY
      case symbol_kind::S_RCURLY: // RCURLY
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_INT_LITERAL: // INT_LITERAL
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // root: Goal
#line 48 "parser.yy"
       {
      root= yystack_[0].value.as < Node* > ();
  }
#line 835 "parser.tab.cc"
    break;

  case 3: // Goal: MainClass ClassDeclarationList "end of file"
#line 54 "parser.yy"
                                     {
    // Create new node for Goal.
    yylhs.value.as < Node* > () = new Node("Goal", "", yylineno);

    // Add MainClass to node
    yylhs.value.as < Node* > () ->children.push_back(yystack_[2].value.as < Node* > ());

    // Add ClassDeclarationList to node
    yylhs.value.as < Node* > () ->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 850 "parser.tab.cc"
    break;

  case 4: // MainClass: PUBLIC CLASS Identifier LCURLY PUBLIC STATIC VOID MAIN LP STRING LBRACKET RBRACKET Identifier RP LCURLY Statement StatementList RCURLY RCURLY
#line 66 "parser.yy"
                                                                                                                                               {
    // Create a new node for MainClass
    yylhs.value.as < Node* > () = new Node("MainClass", yystack_[16].value.as < Node* > ()->value, yylineno);

    // Add the first Statement to the MainClass node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[6].value.as < Node* > ());

    // Add the StatementList to the MainClass node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());

    // Add the second Statement to the MainClass node
    yylhs.value.as < Node* > () ->children.push_back(yystack_[2].value.as < Node* > ());
  }
#line 868 "parser.tab.cc"
    break;

  case 5: // ClassDeclarationList: %empty
#line 83 "parser.yy"
         {
    // Create a new node for ClassDeclarationList
    yylhs.value.as < Node* > () = new Node("ClassDeclarationList", "", yylineno);
  }
#line 877 "parser.tab.cc"
    break;

  case 6: // ClassDeclarationList: ClassDeclarationList ClassDeclaration
#line 87 "parser.yy"
                                          {
    // If there are more ClassDeclarations, add them to the list
    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 887 "parser.tab.cc"
    break;

  case 7: // ClassDeclaration: CLASS Identifier LCURLY VarDeclarationList MethodDeclarationList RCURLY
#line 96 "parser.yy"
                                                                          {
    // Create a new node for ClassDeclaration. Use identifier when creating the node to store the name of the class
    yylhs.value.as < Node* > () = new Node("ClassDeclaration", yystack_[4].value.as < Node* > ()->value, yylineno);

    // Add the VarDeclarationList to the ClassDeclaration node
    yylhs.value.as < Node* > () ->children.push_back(yystack_[2].value.as < Node* > ());

    // Add the MethodDeclarationList to the ClassDeclaration node
    yylhs.value.as < Node* > () ->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 902 "parser.tab.cc"
    break;

  case 8: // VarDeclarationList: %empty
#line 110 "parser.yy"
         {
    // Create a new node for VarDeclarationList
    yylhs.value.as < Node* > () = new Node("VarDeclarationList", "", yylineno);
  }
#line 911 "parser.tab.cc"
    break;

  case 9: // VarDeclarationList: VarDeclarationList VarDeclaration
#line 114 "parser.yy"
                                      {
    // If there are more VarDeclarations, add them to the list
    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 921 "parser.tab.cc"
    break;

  case 10: // VarDeclaration: Type Identifier SEMICOLON
#line 123 "parser.yy"
                            {
    // Create a new node for VarDeclaration.
    yylhs.value.as < Node* > () = new Node("VarDeclaration", "", yylineno);

    // Add the type to the VarDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());

    // Add the identifier to the VarDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 936 "parser.tab.cc"
    break;

  case 11: // NoSemiVarDeclaration: Type Identifier
#line 138 "parser.yy"
                  {
    // Create a new node for VarDeclaration.
    yylhs.value.as < Node* > () = new Node("Parameter", "", yylineno);

    // Add the type to the VarDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());

    // Add the identifier to the VarDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 951 "parser.tab.cc"
    break;

  case 12: // MethodParameterList: %empty
#line 153 "parser.yy"
         {
    // Create a new node for MethodParameterList
    yylhs.value.as < Node* > () = new Node("MethodParameterList", "", yylineno);
  }
#line 960 "parser.tab.cc"
    break;

  case 13: // MethodParameterList: NoSemiVarDeclaration
#line 157 "parser.yy"
                         {
    // Create a new node for MethodParameterList
    yylhs.value.as < Node* > () = new Node("MethodParameterList", "", yylineno);

    // Add the type to the MethodParameterList node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 972 "parser.tab.cc"
    break;

  case 14: // MethodParameterList: MethodParameterList COMMA NoSemiVarDeclaration
#line 164 "parser.yy"
                                                   {
    // Create a new node for MethodParameterList
    yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > ();

    // Add the type to the MethodParameterList node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 984 "parser.tab.cc"
    break;

  case 15: // MethodBody: %empty
#line 176 "parser.yy"
         { 
    // Create a new node for MethodBody
    yylhs.value.as < Node* > () = new Node("MethodBody", "", yylineno);
  }
#line 993 "parser.tab.cc"
    break;

  case 16: // MethodBody: MethodBody VarDeclaration
#line 180 "parser.yy"
                              {
    // If there are more VarDeclarations, add them to the list
    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1003 "parser.tab.cc"
    break;

  case 17: // MethodBody: MethodBody Statement
#line 185 "parser.yy"
                         {
    // If there are more Statements, add them to the list
    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1013 "parser.tab.cc"
    break;

  case 18: // MethodDeclarationList: %empty
#line 194 "parser.yy"
         {
    // Create a new node for MethodDeclarationList
    yylhs.value.as < Node* > () = new Node("MethodDeclarationList", "", yylineno);
  }
#line 1022 "parser.tab.cc"
    break;

  case 19: // MethodDeclarationList: MethodDeclarationList MethodDeclaration
#line 198 "parser.yy"
                                            {
    // If there are more MethodDeclarations, add them to the list
    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1032 "parser.tab.cc"
    break;

  case 20: // MethodDeclaration: PUBLIC Type Identifier LP MethodParameterList RP LCURLY MethodBody RETURN Expression SEMICOLON RCURLY
#line 207 "parser.yy"
                                                                                                        {
    // Create a new node for MethodDeclaration. Use identifier when creating the node to store the name of the method
    yylhs.value.as < Node* > () = new Node("MethodDeclaration", yystack_[9].value.as < Node* > ()->value, yylineno);

    // Add the type to the MethodDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[10].value.as < Node* > ());

    // Add the VarDeclarationList to the MethodDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[7].value.as < Node* > ());

    // Add the MethodBody to the MethodDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());

    // Add the return Expression to the MethodDeclaration node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
  }
#line 1053 "parser.tab.cc"
    break;

  case 21: // Type: INT_ARRAY
#line 228 "parser.yy"
            {
    yylhs.value.as < Node* > () = new Node("Type", "IntArray", yylineno);
  }
#line 1061 "parser.tab.cc"
    break;

  case 22: // Type: BOOLEAN
#line 231 "parser.yy"
            {
    yylhs.value.as < Node* > () = new Node("Type", "Bool", yylineno);
  }
#line 1069 "parser.tab.cc"
    break;

  case 23: // Type: INT
#line 234 "parser.yy"
        {
    yylhs.value.as < Node* > () = new Node("Type", "Int", yylineno);
  }
#line 1077 "parser.tab.cc"
    break;

  case 24: // Type: Identifier
#line 237 "parser.yy"
               {
    yylhs.value.as < Node* > () = new Node("Type", yystack_[0].value.as < Node* > ()->value, yylineno);
  }
#line 1085 "parser.tab.cc"
    break;

  case 25: // StatementList: %empty
#line 244 "parser.yy"
         {
    // Create a new node for StatementList
    yylhs.value.as < Node* > () = new Node("StatementList", "", yylineno);
  }
#line 1094 "parser.tab.cc"
    break;

  case 26: // StatementList: StatementList Statement
#line 248 "parser.yy"
                            {
    // If there are more Statements, add them to the list
    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1104 "parser.tab.cc"
    break;

  case 27: // Statement: LCURLY StatementList RCURLY
#line 257 "parser.yy"
                              {
    // Create a new node to represent a block statement
    yylhs.value.as < Node* > () = new Node("BlockStatement", "", yylineno);

    yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
  }
#line 1115 "parser.tab.cc"
    break;

  case 28: // Statement: IF LP Expression RP Statement
#line 263 "parser.yy"
                                  {
    // Create new node for if statement
    yylhs.value.as < Node* > () = new Node("IfStatement", "", yylineno);

    // Add Expression to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());

    // Add Statement to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1130 "parser.tab.cc"
    break;

  case 29: // Statement: IF LP Expression RP Statement ELSE Statement
#line 273 "parser.yy"
                                                 {
    // Create new node for if else statement
    yylhs.value.as < Node* > () = new Node("IfElseStatement", "", yylineno);

    // Add Expression to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());

    // Add Statement to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());

    // Add second Statement to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1148 "parser.tab.cc"
    break;

  case 30: // Statement: WHILE LP Expression RP Statement
#line 286 "parser.yy"
                                     {
    // Create new node for while statement
    yylhs.value.as < Node* > () = new Node("WhileStatement", "", yylineno);

    // Add Expression to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());

    // Add Statement to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1163 "parser.tab.cc"
    break;

  case 31: // Statement: SYSTEM_OUT_PRINTLN LP Expression RP SEMICOLON
#line 296 "parser.yy"
                                                  {
    // Create new node for print statement
    yylhs.value.as < Node* > () = new Node("PrintStatement", "", yylineno);

    // Add Expression to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
  }
#line 1175 "parser.tab.cc"
    break;

  case 32: // Statement: Identifier ASSIGN Expression SEMICOLON
#line 303 "parser.yy"
                                           {
    // Create new node for assign statement. Use Identifier as name in node
    yylhs.value.as < Node* > () = new Node("AssignStatement", "", yylineno);

    // Add Identifier to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());

    // Add Expression to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 1190 "parser.tab.cc"
    break;

  case 33: // Statement: Identifier LBRACKET Expression RBRACKET ASSIGN Expression SEMICOLON
#line 313 "parser.yy"
                                                                        {
    // Create new node for array assign statment. Use identifier as name in node
    yylhs.value.as < Node* > () = new Node("ArrayAssignStatement", "", yylineno);

    // Add Identifier to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[6].value.as < Node* > ());

    // Add Expression for node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());

    // Add second Expression for node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 1208 "parser.tab.cc"
    break;

  case 34: // ArgumentList: %empty
#line 333 "parser.yy"
           {
      // Create new node for ArgumentList
      yylhs.value.as < Node* > () = new Node("ArgumentList", "", yylineno);
    }
#line 1217 "parser.tab.cc"
    break;

  case 35: // ArgumentList: ExplicitArgumentList
#line 337 "parser.yy"
                           {
      // If non empty, just return the ExplicitArgumentList, which handles the list of expressions and commas
      yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > ();
    }
#line 1226 "parser.tab.cc"
    break;

  case 36: // ExplicitArgumentList: Expression
#line 344 "parser.yy"
               {
      // Create new node for ArgumentList
      yylhs.value.as < Node* > () = new Node("ArgumentList", "", yylineno);

      // Add Expression to node
      yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1238 "parser.tab.cc"
    break;

  case 37: // ExplicitArgumentList: ExplicitArgumentList COMMA Expression
#line 351 "parser.yy"
                                            {
      // If there are more expressions, add them to the list
      yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > ();

      // Add Expression to node
      yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1250 "parser.tab.cc"
    break;

  case 38: // Expression: Expression AND Expression
#line 362 "parser.yy"
                            {
    yylhs.value.as < Node* > () = new Node("AndExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1260 "parser.tab.cc"
    break;

  case 39: // Expression: Expression OR Expression
#line 367 "parser.yy"
                             {
    yylhs.value.as < Node* > () = new Node("OrExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1270 "parser.tab.cc"
    break;

  case 40: // Expression: Expression LT Expression
#line 372 "parser.yy"
                             {
    yylhs.value.as < Node* > () = new Node("LessThanExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1280 "parser.tab.cc"
    break;

  case 41: // Expression: Expression GT Expression
#line 377 "parser.yy"
                             {
    yylhs.value.as < Node* > () = new Node("GreaterThanExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1290 "parser.tab.cc"
    break;

  case 42: // Expression: Expression EQ Expression
#line 382 "parser.yy"
                             {
    yylhs.value.as < Node* > () = new Node("EqualsExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1300 "parser.tab.cc"
    break;

  case 43: // Expression: Expression PLUSOP Expression
#line 387 "parser.yy"
                                 {
    yylhs.value.as < Node* > () = new Node("PlusOperationExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1310 "parser.tab.cc"
    break;

  case 44: // Expression: Expression MINUSOP Expression
#line 392 "parser.yy"
                                  {
    yylhs.value.as < Node* > () = new Node("MinusOperationExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1320 "parser.tab.cc"
    break;

  case 45: // Expression: Expression MULTOP Expression
#line 397 "parser.yy"
                                 {
    yylhs.value.as < Node* > () = new Node("MultiplicationOperationExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1330 "parser.tab.cc"
    break;

  case 46: // Expression: Expression LBRACKET Expression RBRACKET
#line 402 "parser.yy"
                                            {
    yylhs.value.as < Node* > () = new Node("ArrayExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 1340 "parser.tab.cc"
    break;

  case 47: // Expression: Expression DOT LENGTH
#line 407 "parser.yy"
                          {
    yylhs.value.as < Node* > () = new Node("LengthExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
  }
#line 1349 "parser.tab.cc"
    break;

  case 48: // Expression: Expression DOT Identifier LP ArgumentList RP
#line 411 "parser.yy"
                                                 {
    // Create new node for method call expression. Use identifier as name for node
    yylhs.value.as < Node* > () = new Node("MethodCallExpression", "", yylineno);

    // Add Expression to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[5].value.as < Node* > ());

    // Add Identifier to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());

    // Add ArgumentList to node
    yylhs.value.as < Node* > () ->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 1367 "parser.tab.cc"
    break;

  case 49: // Expression: INT_LITERAL
#line 424 "parser.yy"
                {
    // Create new node for int literal. Use the literal int as name
    yylhs.value.as < Node* > () = new Node("IntLiteralExpression", yystack_[0].value.as < std::string > (), yylineno);
  }
#line 1376 "parser.tab.cc"
    break;

  case 50: // Expression: TRUE
#line 428 "parser.yy"
         {
    yylhs.value.as < Node* > () = new Node("BoolLiteral", "true", yylineno);
  }
#line 1384 "parser.tab.cc"
    break;

  case 51: // Expression: FALSE
#line 431 "parser.yy"
          {
    yylhs.value.as < Node* > () = new Node("BoolLiteral", "false", yylineno);
  }
#line 1392 "parser.tab.cc"
    break;

  case 52: // Expression: Identifier
#line 434 "parser.yy"
               {
    // In this case its just Identifier
    yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > ();
  }
#line 1401 "parser.tab.cc"
    break;

  case 53: // Expression: THIS
#line 438 "parser.yy"
         {
    yylhs.value.as < Node* > () = new Node("ThisExpression", "this", yylineno);
  }
#line 1409 "parser.tab.cc"
    break;

  case 54: // Expression: NEW INT LBRACKET Expression RBRACKET
#line 441 "parser.yy"
                                         {
    yylhs.value.as < Node* > () = new Node("NewIntArrayExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 1418 "parser.tab.cc"
    break;

  case 55: // Expression: NEW Identifier LP RP
#line 445 "parser.yy"
                         {
    yylhs.value.as < Node* > () = new Node("NewObjectExpression", "", yylineno);

    // Add Identifier to node
    yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
  }
#line 1429 "parser.tab.cc"
    break;

  case 56: // Expression: NOT Expression
#line 451 "parser.yy"
                   {
    yylhs.value.as < Node* > () = new Node("NotExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
  }
#line 1438 "parser.tab.cc"
    break;

  case 57: // Expression: LP Expression RP
#line 455 "parser.yy"
                     {
    // In this case its just the Expression since we need to evaluate the expression in the parentheses first
    yylhs.value.as < Node* > () = new Node("ParenthesesExpression", "", yylineno);
    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
  }
#line 1448 "parser.tab.cc"
    break;

  case 58: // Identifier: STRING_LITERAL
#line 464 "parser.yy"
                 {
    // Create new node for string literal. Use the literal string as name
    yylhs.value.as < Node* > () = new Node("Identifier", yystack_[0].value.as < std::string > (), yylineno);
  }
#line 1457 "parser.tab.cc"
    break;


#line 1461 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -50;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
       6,    25,    47,   -50,   -50,    54,   -50,    67,   -50,    30,
     -50,    54,   -50,    58,    39,    61,   -50,    64,   299,    66,
     -50,   -50,   -50,   -50,    -7,    54,   -50,    83,   299,   -50,
     -50,    50,    75,    54,   -50,    56,    88,    70,   299,    54,
     -50,    -1,    54,   106,    80,   299,   -50,    89,   -50,   -50,
     124,   275,   109,   112,   122,   -50,   -50,   -29,     2,   -50,
     -50,   -29,     2,     2,     2,   268,   279,     2,     2,     2,
     -50,   -50,   -50,    13,     2,   -50,    81,   -50,    27,   153,
     167,   -50,   -50,    60,   123,   183,   197,    93,   125,   -27,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    -6,
      95,    96,   124,   124,   -50,   -50,   103,   -50,     2,   131,
      12,    12,   -27,    11,    11,    84,    84,    84,   213,   -50,
     139,   -50,   -50,   127,   -50,     2,   227,   -50,   -50,     2,
     124,   137,   -50,   136,   110,   243,   -50,   -50,   -50,     2,
     243
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     2,     5,     0,     1,     0,    58,     0,
       3,     0,     6,     0,     0,     0,     8,     0,    18,     0,
      23,    21,    22,     9,     0,     0,    24,     0,     0,     7,
      19,     0,     0,     0,    10,     0,     0,     0,    12,     0,
      13,     0,     0,     0,     0,     0,    11,     0,    15,    14,
       0,     0,     0,     0,     0,    25,    25,     0,     0,    16,
      17,    24,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,    53,     0,     0,    49,     0,    52,     0,     0,
       0,    27,    26,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,    32,     0,    57,     0,     0,
      43,    44,    45,    38,    39,    42,    40,    41,     0,    47,
       0,    20,    31,    28,    30,     0,     0,    55,    46,    34,
       0,     0,    54,     0,    35,    36,    29,    33,    48,     0,
      37
  };

  const signed char
  parser::yypgoto_[] =
  {
     -50,   -50,   -50,   -50,   -50,   -50,   -50,    98,   118,   -50,
     -50,   -50,   -50,    24,   117,   -49,   -50,   -50,     9,    -5
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     4,     7,    12,    18,    23,    40,    41,
      51,    24,    30,    25,    65,    82,   133,   134,    76,    77
  };

  const unsigned char
  parser::yytable_[] =
  {
       9,    56,    60,     8,    67,    28,    14,    44,    68,    69,
      98,     8,    99,    26,    90,    91,    92,    92,     1,    87,
      31,   119,     8,    26,    70,    71,    72,    73,    36,    29,
      90,    91,    92,    26,    43,   101,    74,    46,     5,    45,
      26,    95,    96,    97,    75,    57,    61,     6,    98,    98,
      99,    99,    33,   123,   124,    93,    94,    95,    96,    97,
      57,    57,    42,     8,    98,    13,    99,    10,    88,    42,
      15,    78,    79,    80,    16,    17,    84,    85,    86,    19,
      11,   136,    27,    89,    90,    91,    92,    90,    91,    92,
      32,    34,    35,    37,   120,    38,   104,    57,    57,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    39,    93,
      94,    95,    96,    97,    47,    48,    62,   126,    98,    63,
      99,    98,   100,    99,    50,    57,    90,    91,    92,    64,
     108,   121,   109,     8,   131,    52,   125,   122,   135,   127,
      90,    91,    92,    53,   138,    54,   129,   130,   140,    59,
     139,    93,    94,    95,    96,    97,    90,    91,    92,    55,
      98,   102,    99,    49,   105,    93,    94,    95,    96,    97,
      90,    91,    92,    66,    98,   103,    99,     0,   137,     0,
       0,    93,    94,    95,    96,    97,    90,    91,    92,     0,
      98,     0,    99,     0,     0,    93,    94,    95,    96,    97,
      90,    91,    92,     0,    98,   107,    99,     0,     0,     0,
       0,    93,    94,    95,    96,    97,    90,    91,    92,     0,
      98,   106,    99,     0,     0,    93,    94,    95,    96,    97,
      90,    91,    92,     0,    98,     0,    99,     0,     0,     0,
       0,    93,    94,    95,    96,    97,    90,    91,    92,     0,
      98,   128,    99,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,   132,    99,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     8,     0,    52,
      98,    20,    99,     0,     8,    21,    52,    53,     8,    54,
      52,     0,     0,    22,    53,     0,    54,     0,    53,     0,
      54,    58,     0,    55,    81,    20,     0,     0,     8,    21,
      55,     0,     0,     0,    55,    83,     0,    22
  };

  const short
  parser::yycheck_[] =
  {
       5,    50,    51,     9,    33,    12,    11,     8,    37,     7,
      37,     9,    39,    18,     3,     4,     5,     5,    12,     6,
      25,    27,     9,    28,    22,    23,    24,    25,    33,    36,
       3,     4,     5,    38,    39,     8,    34,    42,    13,    40,
      45,    30,    31,    32,    42,    50,    51,     0,    37,    37,
      39,    39,    28,   102,   103,    28,    29,    30,    31,    32,
      65,    66,    38,     9,    37,    35,    39,     0,    73,    45,
      12,    62,    63,    64,    35,    14,    67,    68,    69,    15,
      13,   130,    16,    74,     3,     4,     5,     3,     4,     5,
       7,    41,    17,    37,    99,     7,    36,   102,   103,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    38,    28,
      29,    30,    31,    32,     8,    35,     7,   108,    37,     7,
      39,    37,    41,    39,    35,   130,     3,     4,     5,     7,
      37,    36,     7,     9,   125,    11,    33,    41,   129,     8,
       3,     4,     5,    19,     8,    21,     7,    20,   139,    51,
      40,    28,    29,    30,    31,    32,     3,     4,     5,    35,
      37,     8,    39,    45,    41,    28,    29,    30,    31,    32,
       3,     4,     5,    56,    37,     8,    39,    -1,    41,    -1,
      -1,    28,    29,    30,    31,    32,     3,     4,     5,    -1,
      37,    -1,    39,    -1,    -1,    28,    29,    30,    31,    32,
       3,     4,     5,    -1,    37,     8,    39,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    32,     3,     4,     5,    -1,
      37,    38,    39,    -1,    -1,    28,    29,    30,    31,    32,
       3,     4,     5,    -1,    37,    -1,    39,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    32,     3,     4,     5,    -1,
      37,    38,    39,    -1,    -1,    28,    29,    30,    31,    32,
      -1,    -1,    -1,    -1,    37,    38,    39,    -1,    -1,    -1,
      -1,    28,    29,    30,    31,    32,    -1,     9,    -1,    11,
      37,     6,    39,    -1,     9,    10,    11,    19,     9,    21,
      11,    -1,    -1,    18,    19,    -1,    21,    -1,    19,    -1,
      21,    26,    -1,    35,    36,     6,    -1,    -1,     9,    10,
      35,    -1,    -1,    -1,    35,    36,    -1,    18
  };

  const signed char
  parser::yystos_[] =
  {
       0,    12,    44,    45,    46,    13,     0,    47,     9,    62,
       0,    13,    48,    35,    62,    12,    35,    14,    49,    15,
       6,    10,    18,    50,    54,    56,    62,    16,    12,    36,
      55,    62,     7,    56,    41,    17,    62,    37,     7,    38,
      51,    52,    56,    62,     8,    40,    62,     8,    35,    51,
      35,    53,    11,    19,    21,    35,    58,    62,    26,    50,
      58,    62,     7,     7,     7,    57,    57,    33,    37,     7,
      22,    23,    24,    25,    34,    42,    61,    62,    61,    61,
      61,    36,    58,    36,    61,    61,    61,     6,    62,    61,
       3,     4,     5,    28,    29,    30,    31,    32,    37,    39,
      41,     8,     8,     8,    36,    41,    38,     8,    37,     7,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    27,
      62,    36,    41,    58,    58,    33,    61,     8,    38,     7,
      20,    61,    38,    59,    60,    61,    58,    41,     8,    40,
      61
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    43,    44,    45,    46,    47,    47,    48,    49,    49,
      50,    51,    52,    52,    52,    53,    53,    53,    54,    54,
      55,    56,    56,    56,    56,    57,    57,    58,    58,    58,
      58,    58,    58,    58,    59,    59,    60,    60,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    62
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     3,    19,     0,     2,     6,     0,     2,
       3,     2,     0,     1,     3,     0,     2,     2,     0,     2,
      12,     1,     1,     1,     1,     0,     2,     3,     5,     7,
       5,     5,     4,     7,     0,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     3,     6,     1,
       1,     1,     1,     1,     5,     4,     2,     3,     1
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "PLUSOP", "MINUSOP",
  "MULTOP", "INT", "LP", "RP", "STRING_LITERAL", "INT_ARRAY",
  "SYSTEM_OUT_PRINTLN", "PUBLIC", "CLASS", "STATIC", "VOID", "MAIN",
  "STRING", "BOOLEAN", "IF", "ELSE", "WHILE", "TRUE", "FALSE", "THIS",
  "NEW", "RETURN", "LENGTH", "AND", "OR", "EQ", "LT", "GT", "ASSIGN",
  "NOT", "LCURLY", "RCURLY", "LBRACKET", "RBRACKET", "DOT", "COMMA",
  "SEMICOLON", "INT_LITERAL", "$accept", "root", "Goal", "MainClass",
  "ClassDeclarationList", "ClassDeclaration", "VarDeclarationList",
  "VarDeclaration", "NoSemiVarDeclaration", "MethodParameterList",
  "MethodBody", "MethodDeclarationList", "MethodDeclaration", "Type",
  "StatementList", "Statement", "ArgumentList", "ExplicitArgumentList",
  "Expression", "Identifier", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    48,    48,    54,    66,    83,    87,    96,   110,   114,
     123,   138,   153,   157,   164,   176,   180,   185,   194,   198,
     207,   228,   231,   234,   237,   244,   248,   257,   263,   273,
     286,   296,   303,   313,   333,   337,   344,   351,   362,   367,
     372,   377,   382,   387,   392,   397,   402,   407,   411,   424,
     428,   431,   434,   438,   441,   445,   451,   455,   464
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2047 "parser.tab.cc"

#line 469 "parser.yy"
