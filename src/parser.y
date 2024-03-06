// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

%code requires{
    #include "ast.hpp"

    extern Node *g_root;
    extern FILE *yyin;
    int yylex(void);
    void yyerror(const char *);
}

// Represents the value associated with any kind of AST node.
%union{
  Node         *node;
  List_Ptr     nodes;
  Specifier    type;

  int          number_int;
  double       number_float;
  std::string  *string;

  ExpressionRoot *expression_root;
  DeclarationRoot *declaration_root;
  StatementRoot  *statement_root;

  UnaryOp* unary_op;

  yytokentype  token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_CONSTANT CHAR_CONSTANT
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <node>   external_declaration function_definition
%type <node>   declaration

%type <nodes> translation_unit statement_list declaration_list parameter_list argument_expression_list
%type <nodes> initializer_list identifier_list init_declarator_list

%type <declaration_root>  declarator direct_declarator init_declarator parameter_declaration

%type <statement_root> statement jump_statement compound_statement selection_statement
%type <statement_root> labeled_statement expression_statement iteration_statement

%type <expression_root> expression assignment_expression unary_expression cast_expression postfix_expression initializer
%type <expression_root> additive_expression multiplicative_expression shift_expression
%type <expression_root> conditional_expression logical_or_expression logical_and_expression inclusive_or_expression
%type <expression_root> exclusive_or_expression relational_expression and_expression equality_expression
%type <expression_root> constant_expression primary_expression

%type <node> struct_specifier struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list
%type <node> struct_declarator enum_specifier enumerator_list enumerator pointer
%type <node> abstract_declarator direct_abstract_declarator


%type <number_int> INT_CONSTANT
%type <number_float> FLOAT_CONSTANT
%type <string> IDENTIFIER STRING_CONSTANT CHAR_CONSTANT
%type <string> assignment_operator storage_class_specifier
%type <type> declaration_specifiers type_name type_specifier
%type <unary_op> unary_operator


%start ROOT
%%

ROOT
  : translation_unit { g_root = new RootNode($1); }

translation_unit
    : external_declaration                  { $$ = new std::vector<const Node*>({ $1 }); }
    | translation_unit external_declaration { $1 -> push_back($2); $$ = $1;}
    ;

external_declaration
	: function_definition { $$ = $1; }
	| declaration	{ $$ = $1; }
	;

declaration_list
    : declaration                        { $$ = new std::vector<const Node*>({ $1 }); }
    | declaration_list declaration       { $1 -> push_back($2); $$ = $1;}
    ;


function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement {
		$$ = new FunctionDefinition($1, $2, $3);
	}
	| declarator declaration_list compound_statement
	| declarator compound_statement
	;


primary_expression
	: IDENTIFIER        { $$ = new Identifier(*$1); delete $1; } //memory leak?
	| INT_CONSTANT      { $$ = new IntConstant($1); }  //memory leak?
    | FLOAT_CONSTANT	{ $$ = new FloatConstant($1); }
	| STRING_CONSTANT    { $$ = new StringConstant(*$1); }
	| CHAR_CONSTANT      { $$ = new CharConstant(*$1); }
	| '(' expression ')'	{ $$ = $2; }
	;

postfix_expression
	: primary_expression                         { $$ = $1; }
	| postfix_expression '[' expression ']'      { $$ = new ArrayIndex($1, $3); }
	| postfix_expression '(' ')'                 { $$ = new FunctionCall($1, nullptr); }
	| postfix_expression '(' argument_expression_list ')' { $$ = new FunctionCall($1, $3); }
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP                   { $$ = new Increment($1); }
	| postfix_expression DEC_OP                   { $$ = new Decrement($1); }
	;

argument_expression_list
	: assignment_expression                                 { $$ = new std::vector<const Node*>({ $1 }); }
	| argument_expression_list ',' assignment_expression    { $1 -> push_back($3); $$ = $1;}
	;

unary_expression
	: postfix_expression                   { $$ = $1; }
	| INC_OP unary_expression              { $$ = new Increment($2); }
	| DEC_OP unary_expression              { $$ = new Decrement($2); }
	| unary_operator cast_expression
	| SIZEOF unary_expression              { $$ = new SizeOfExpression($2); }
	| SIZEOF '(' type_specifier ')'             { $$ = new SizeOfType($3); }
	;

unary_operator
    : '&' unary_expression 		{ $$ = new UnaryOp(UnaryOperator::AddressOf, $2); }
    | '*' unary_expression 		{ $$ = new UnaryOp(UnaryOperator::Deref, $2); }
    | '+' unary_expression 		{ $$ = new UnaryOp(UnaryOperator::Plus, $2); }
    | '-' unary_expression 		{ $$ = new UnaryOp(UnaryOperator::Minus, $2); }
    | '~' unary_expression 		{ $$ = new UnaryOp(UnaryOperator::BitwiseNot, $2); }
    | '!' unary_expression 		{ $$ = new UnaryOp(UnaryOperator::LogicalNot, $2); }
    ;

cast_expression
	: unary_expression                  { $$ = $1; }
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression                   { $$ = $1; }
	| multiplicative_expression '*' cast_expression	{ $$ = new Multiplication($1, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = new Division($1, $3); }
	| multiplicative_expression '%' cast_expression { $$ = new Modulus($1, $3); }
	;

additive_expression
	: multiplicative_expression         { $$ = $1; }
	| additive_expression '+' multiplicative_expression	{ $$ = new Addition($1, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = new Subtraction($1, $3); }
	;

shift_expression
	: additive_expression    			{ $$ = $1; }
	| shift_expression LEFT_OP additive_expression	{ $$ = new LeftShift($1, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = new RightShift($1, $3); }
	;

relational_expression
	: shift_expression	{ $$ = $1;}
	| relational_expression '<' shift_expression	{ $$ = new LessThan($1, $3);} //memory leak?
	| relational_expression '>' shift_expression	{ $$ = new GreaterThan($1, $3);}
	| relational_expression LE_OP shift_expression	{ $$ = new LessThanEqual($1, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = new GreaterThanEqual($1, $3); }
	;

equality_expression
	: relational_expression	{ $$ = $1;}
	| equality_expression EQ_OP relational_expression	{ $$ = new Equal($1, $3);}
	| equality_expression NE_OP relational_expression	{ $$ = new NotEqual($1, $3);}
	;

and_expression
	: equality_expression	{ $$ = $1;}
	| and_expression '&' equality_expression	{ $$ = new BitwiseAnd($1, $3);}
	;

exclusive_or_expression
	: and_expression	{ $$ = $1;}
	| exclusive_or_expression '^' and_expression	{ $$ = new BitwiseXor($1, $3);}
	;

inclusive_or_expression
	: exclusive_or_expression	{ $$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = new BitwiseOr($1, $3); }
	;

logical_and_expression
	: inclusive_or_expression	{ $$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = new LogicalAnd($1, $3); }
	;

logical_or_expression
	: logical_and_expression	{ $$ = $1;}
	| logical_or_expression OR_OP logical_and_expression	{ $$ = new LogicalOr($1, $3);}
	;

conditional_expression
	: logical_or_expression	{ $$ = $1;}
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression	{ $$ = $1; }
	| conditional_expression '=' assignment_expression         { $$ = new Assignment($1, $3); }
	| unary_expression ADD_ASSIGN assignment_expression        { $$ = new Assignment($1, new Addition($1, $3)); }
	| unary_expression SUB_ASSIGN assignment_expression        { $$ = new Assignment($1, new Subtraction($1, $3)); }
    | unary_expression MUL_ASSIGN assignment_expression        { $$ = new Assignment($1, new Multiplication($1, $3)); }
	| unary_expression DIV_ASSIGN assignment_expression        { $$ = new Assignment($1, new Division($1, $3)); }
	| unary_expression MOD_ASSIGN assignment_expression        { $$ = new Assignment($1, new Modulus($1, $3)); }
	| unary_expression LEFT_ASSIGN assignment_expression       { $$ = new Assignment($1, new LeftShift($1, $3)); }
	| unary_expression RIGHT_ASSIGN assignment_expression      { $$ = new Assignment($1, new RightShift($1, $3)); }
	| unary_expression AND_ASSIGN assignment_expression        { $$ = new Assignment($1, new BitwiseAnd($1, $3)); }
	| unary_expression XOR_ASSIGN assignment_expression        { $$ = new Assignment($1, new BitwiseXor($1, $3)); }
	| unary_expression OR_ASSIGN assignment_expression         { $$ = new Assignment($1, new BitwiseOr($1, $3)); }
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression                    { $$ = $1; }
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression                    { $$ = $1; }
	;

/* declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	; */

declaration
	: declaration_specifiers init_declarator ';'     { $$ = new Declaration($1, $2); }
	;

declaration_specifiers
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers
	| type_specifier { $$ = $1; }
	| type_specifier declaration_specifiers
	;

init_declarator_list
	: init_declarator                             { $$ = new std::vector<const Node*>({ $1 }); }
	| init_declarator_list ',' init_declarator    { $1 -> push_back($3); $$ = $1;}
	;

init_declarator
	: declarator	{ $$ = $1; }
	| declarator '=' initializer   {$$ = new InitDeclarator($1, $3);}
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID	{ $$ = Specifier::_void; }
	| CHAR	{ $$ = Specifier::_char; }
	| SHORT
	| INT { $$ = Specifier::_int; }
	| LONG
	| FLOAT	{ $$ = Specifier::_float; }
	| DOUBLE	{ $$ = Specifier::_double; }
	| SIGNED
	| UNSIGNED	{ $$ = Specifier::_int; }

| struct_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_specifier
	: STRUCT IDENTIFIER '{' struct_declaration_list '}'
	| STRUCT '{' struct_declaration_list '}'
	| STRUCT IDENTIFIER
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

declarator
	: pointer direct_declarator   { $$ = new PointerDeclarator($2); }
	| direct_declarator           { $$ = $1; }
	;

direct_declarator
	: IDENTIFIER                  { $$ = new Declarator(*$1); delete $1; } //memory leak?
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'    { $$ = new ArrayDeclarator($1, $3); }
	| direct_declarator '[' ']'                 {std::cerr << "Error: Array size not specified" << std::endl; exit(1);}
	| direct_declarator '(' parameter_list ')'  { $$ = new FunctionDeclarator($1, $3); }
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'    { $$ = new FunctionDeclarator($1, nullptr); }
	;

pointer
	: '*'
	| '*' pointer
	;

parameter_list
	: parameter_declaration                         { $$ = new std::vector<const Node*>({ $1 }); }
	| parameter_list ',' parameter_declaration      { $1 -> push_back($3); $$ = $1;}
    ;

parameter_declaration
	: type_specifier declarator              { $$ = new ParameterDeclaration($1, $2); }
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER                                    { $$ = initList(new Identifier(*$1)); }
	| identifier_list ',' IDENTIFIER                { $$ = concatList($1,new Identifier(*$3)); }
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_list ')'
	;

initializer
	: assignment_expression       { $$ = $1; }
	| '{' initializer_list '}'    { $$ = new ArrayInitialiser($2); }
	| '{' initializer_list ',' '}'{}
	;

initializer_list
	: initializer                       { $$ = new std::vector<const Node*>({ $1 }); }
	| initializer_list ',' initializer  { $1 -> push_back($3); $$ = $1;}
	;

statement
	: labeled_statement          { $$ = $1; }
	| compound_statement         { $$ = $1; }
	| expression_statement       { $$ = $1; }
	| selection_statement        { $$ = $1; }
	| iteration_statement        { $$ = $1; }
	| jump_statement             { $$ = $1; }
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement        { $$ = new Case($2, $4); }
	| DEFAULT ':' statement                         { $$ = new Case(nullptr, $3); }
	;

compound_statement
    : '{' statement_list '}'                        { $$ = new CompoundStatement($2, nullptr); }
    | '{' declaration_list '}'                      { $$ = new CompoundStatement(nullptr, $2); }
    | '{' declaration_list statement_list '}'       { $$ = new CompoundStatement($3, $2); }
    | '{' '}'                                       { $$ = new CompoundStatement(nullptr, nullptr);}
    ;

statement_list
	: statement                           { $$ = new std::vector<const Node*>({ $1 }); }
	| statement_list statement            { $1 -> push_back($2); $$ = $1;}
	;

expression_statement
	: ';'	                {$$ = NULL;}
	| expression ';'        { $$ = new ExpressionStatement($1); }
	;

selection_statement
	: IF '(' expression ')' statement                       { $$ = new IfElseStatement($3, $5, nullptr); }
	| IF '(' expression ')' statement ELSE statement        { $$ = new IfElseStatement($3, $5, $7); }
	| SWITCH '(' expression ')' statement                   { $$ = new SwitchStatement($3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement                                             { $$ = new WhileStatement($3, $5); }
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement                { $$ = new ForStatement($3, $4, nullptr, $6); }
	| FOR '(' expression_statement expression_statement expression ')' statement     { $$ = new ForStatement($3, $4, $5, $7); }
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| RETURN expression ';'       { $$ = new ReturnStatement($2); }
	| CONTINUE ';'                 { $$ = new ContinueStatement(); }
	| BREAK ';'                    { $$ = new BreakStatement(); }
	| RETURN ';'                   { $$ = new ReturnStatement(nullptr); }
	;


%%

Node *g_root;

Node *ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if(yyin == NULL){
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    exit(1);
  }
  g_root = nullptr;
  yyparse();
  return g_root;
}
