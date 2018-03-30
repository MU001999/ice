%{
	#include "node.h"
	BlockExprAST *programBlock;

	extern int yylex();
	void yyerror(const char *s) { printf("ERROR: %s\n", s); }
%}

%union{
	Node *node;
	BlockExprAST *block;
	ExprAST *expr;
	StmtAST *stmt;
	IdentifierExprAST *ident;
	VariableDeclarationStmtAST *var_decl;
	IfElseStmtAST *if_else;
	std::vector<IdentifierExprAST*> *varvec;
    std::vector<ExprAST*> *exprvec;
    std::string *string;
    int token;
}

%token <string> TIDENTIFIER TINTEGER TDOUBLE
%token <token> TASSIGN TCOMMA
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE
%token <token> TADD TSUB TMUL TDIV TMOD
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE
%token <token> TAT TIF TELSE TWHILE TRETURN

%type <ident> ident
%type <expr> numeric expr
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl
%type <token> comparison
%type <stmt> if_else return_stmt

%left TCEQ TCNE
%left TCLT TCLE TCGT TCGE
%left TADD TSUB
%left TMUL TDIV TMOD

%start program

%%

program	
	: stmts { programBlock = $1; }
	;

stmts
	: stmt { $$ = new BlockExprAST(); $$->statements.push_back($1); }
	| stmts stmt { $1->statements.push_back($2); }
	;

stmt
	: var_decl | func_decl | if_else
	| expr { $$ = new ExprStmtAST(*$1); }
	| return_stmt
	;

return_stmt
	: TRETURN expr { $$ = new ReturnStmtAST(*$2); }
	;

block
	: TLBRACE stmts TRBRACE { $$ = $2; }
	| TLBRACE TRBRACE { $$ = new BlockExprAST(); }
	;

var_decl
	: ident TASSIGN expr { $$ = new VariableDeclarationStmtAST(*$1, $3); }
	;

func_decl
	: TAT ident TLPAREN func_decl_args TRPAREN TASSIGN expr { BlockExprAST *b = new BlockExprAST(); b->statements.push_back(new ReturnStmtAST(*$7)); $$ = new FunctionDeclarationStmtAST(*$2, *$4, *b); }
	| TAT ident TLPAREN func_decl_args TRPAREN block { $$ = new FunctionDeclarationStmtAST(*$2, *$4, *$6); }
	;

func_decl_args
	: /*blank*/ { $$ = new VariableList(); }
	| ident { $$ = new VariableList(); $$->push_back($1); }
	| func_decl_args TCOMMA ident { $1->push_back($3); }
	;

ident
	: TIDENTIFIER { $$ = new IdentifierExprAST(*$1); delete $1;}
	;

numeric	
	: TINTEGER { $$ = new IntegerExprAST(atol($1->c_str())); delete $1; }
	| TDOUBLE { $$ = new DoubleExprAST(atof($1->c_str())); delete $1; }
	;

expr
	: ident TLPAREN call_args TRPAREN { $$ = new MethodCallExprAST($1->name, *$3); delete $3; }
	| ident { $$ = $1; }
	| numeric
	| expr TADD expr { $$ = new BinaryOperatorExprAST(*$1, $2, *$3); }
	| expr TSUB expr { $$ = new BinaryOperatorExprAST(*$1, $2, *$3); }
	| expr TMUL expr { $$ = new BinaryOperatorExprAST(*$1, $2, *$3); }
	| expr TDIV expr { $$ = new BinaryOperatorExprAST(*$1, $2, *$3); }
	| expr TMOD expr { $$ = new BinaryOperatorExprAST(*$1, $2, *$3); }
	| expr comparison expr { $$ = new BinaryOperatorExprAST(*$1, $2, *$3); }
	| TLPAREN expr TRPAREN { $$ = $2; }
	;

call_args
	: /*blank*/ { $$ = new ExpressionList(); }
	| expr { $$ = new ExpressionList(); $$->push_back($1); }
	| call_args TCOMMA expr  { $1->push_back($3); }
	;

comparison
	: TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE
	;

if_else
	: TIF expr block { BlockExprAST *_b = new BlockExprAST(); ; $$ = new IfElseStmtAST(*$2, *$3, *_b); }
	| TIF expr block TELSE block { $$ = new IfElseStmtAST(*$2, *$3, *$5); }
	;

%%