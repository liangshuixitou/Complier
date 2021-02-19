%option noyywrap

%{
#include <string>
#include <cstring>
#include "node.h"
#include "grammar.tab.h"

#define SAVE_TOKEN yylval.string = new std::string(yytext)
#define TOKEN(t) ( yylval.token = t)


%}

LETTER  		        [a-zA-Z]
IDENTIFIER              {LETTER}+
DIGIT   		        [0-9]
NUMBER  		        [1-9]{DIGIT}*|[0]	

SPACE                   [ \t\n]
MULTILINE_COMMENT       "/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"
INLINE_COMMENT          \/\/.*

%%

{SPACE}                 ;
{MULTILINE_COMMENT}     ;
{INLINE_COMMENT}        ;

"int"                   SAVE_TOKEN; return TINT;
"void"                  SAVE_TOKEN; return TVOID;
"else"                  return TOKEN(TELSE);
"if"                    return TOKEN(TIF);
"return"                return TOKEN(TRETURN);
"for"                   return TOKEN(TFOR);
"while"                 return TOKEN(TWHILE);

{IDENTIFIER}            SAVE_TOKEN; return TIDENTIFIER;
{NUMBER}                SAVE_TOKEN; return TNUMBER;

"="                     return TOKEN(TEQUAL);
"=="                    return TOKEN(TCEQ);
"!="                    return TOKEN(TCNE);
"<"                     return TOKEN(TCLT);
"<="                    return TOKEN(TCLE);
">"                     return TOKEN(TCGT);
">="                    return TOKEN(TCGE);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"["                     return TOKEN(TLBRACK);
"]"                     return TOKEN(TRBRACK);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
";"                     return TOKEN(TSEMI);
.                       printf("Unknown token!\n"); yyterminate();

%%


