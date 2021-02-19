%{
    #include "node.h"
    #include <cstring>
    #include <vector>
    NBlock *programBlock; /* the top level root node of our final AST */

    extern int yylex();
    extern char* yytext;
    void yyerror(const char *s) { printf("\nGrammatical Analysis Error: %s\n", s); }

%}

/* Represents the many different ways we can access our data */
%union {
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    NFunctionDeclaration *func_decl;
    std::vector<NVariableDeclaration *> *varvec;
    std::vector<NExpression *> *exprvec;
    std::string *string;
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token  <string> TIDENTIFIER TNUMBER TINT TVOID 
%token  <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL 
%token  <token> TLPAREN TRPAREN TLBRACK TRBRACK TLBRACE TRBRACE TCOMMA TDOT TSEMI
%token  <token> TPLUS TMINUS TMUL TDIV
%token  <token> TIF TELSE TFOR TRETURN TWHILE

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type  <ident> ident typename
%type  <expr> numeric expr
%type  <varvec> func_decl_args
%type  <exprvec> call_args
%type  <block> program stmts block
%type  <stmt> stmt var_decl func_decl if_stmt for_stmt while_stmt
%type  <token> comparison


/* Operator precedence for mathematical operators */
%left  TPLUS TMINUS
%left  TMUL TDIV

%start program

%%

program : stmts { programBlock = $1; }
        ;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($1); }
      | stmts stmt { $1->statements.push_back($2); }
      ;

stmt : var_decl TSEMI | func_decl
     | expr TSEMI { $$ = new NExpressionStatement(*$1); }
     | TRETURN expr TSEMI { $$ = new NReturnStatement($2); }
     | if_stmt
     | for_stmt
     | while_stmt
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new NBlock(); }
      ;

var_decl : typename ident  
            { 
                $$ = new NVariableDeclaration(*$1, *$2); 
            }
         | typename ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
         ;

func_decl : typename ident TLPAREN func_decl_args TRPAREN block
            { 
                $$ = new NFunctionDeclaration(*$1, *$2, *$4, $6); 
            }
          ;

func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back((NVariableDeclaration*) $1); }
          | func_decl_args TCOMMA var_decl { $1->push_back((NVariableDeclaration*) $3); }
          ;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1);}
      ;

typename : TVOID { $$ = new NIdentifier(*$1); $$->isType = true;}
          | TINT { $$ = new NIdentifier(*$1); $$->isType = true;}

numeric : TNUMBER { $$ = new NInteger(atol($1->c_str()));}
        ;

expr : ident TEQUAL expr { $$ = new NAssignment(*$1, *$3); }
     | ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3);}
     | ident { $$ = $1; }
     | numeric
     | expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     | TLPAREN expr TRPAREN { $$ = $2; }
     | expr TMUL expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
	 | expr TDIV expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
	 | expr TPLUS expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
	 | expr TMINUS expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
     ;

call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args TCOMMA expr  { $1->push_back($3); }
          ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE
           ;

if_stmt : TIF expr block { $$ = new NIfStatement(*$2, $3); }
		| TIF expr block TELSE block { $$ = new NIfStatement(*$2, $3, $5); }

for_stmt : TFOR TLPAREN expr TSEMI expr TSEMI expr TRPAREN block { $$ = new NForStatement($9, $3, $5, $7); }

while_stmt : TWHILE TLPAREN expr TRPAREN block { $$ = new NForStatement($5, nullptr, $3, nullptr); }

%%

// tokenNode* creatTokenList ()
// {
//     struct tokenNode *head, *last;
//     int tok;
//     int first = 1;
//     while (tok = yylex())
//     {
//         if (first)
//         {
//             first = 0;
//             head = new tokenNode;
//             head->tokenType = tok;
//             strcpy(head->stringValue, yytext);
//             head->numberValue = (tok == TNUMBER) ? atoi(yytext) : 0;
//             head->next = NULL; 
//             last = head;
//         }
//         else 
//         {
//             struct tokenNode *t = new tokenNode;
//             t->tokenType = tok;
//             strcpy(t->stringValue, yytext);
//             t->numberValue = (tok == TNUMBER) ? atoi(yytext) : 0;
//             t->next = NULL; 
//             last->next = t;
//             last = t;
//         }
//     }
//     return head;
// }


// void printTokenList (struct tokenNode* head)
// {
//     struct tokenNode *p = head;
//     printf("tokenType\tstringValue\tnumberValue\n");
//     while (p)
//     {
//         printf("%d\t\t%s\t\t%d\n", p->tokenType, p->stringValue, p->numberValue);
//         p = p->next;
//     }
// }

// main () 
// {
//     // int tok;
//     // struct tokenNode* head;
//     // head = creatTokenList();
//     // printTokenList(head);
//     funcTB.tableName = "global";
//     yyparse();
//     funcTB.printTable();
// }
