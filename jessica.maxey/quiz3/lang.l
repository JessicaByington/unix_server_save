%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Feb. 20, 2016
//
// Edited by Jessica Maxey

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "langparse.h"

// Macros that can be used to create debug output from the scanner
// The first one does NOT create debug output, the second does.
// Uncomment one or the other then use DO_RETURN(val) for all your return 
// statements
#define DO_RETURN(a) return (a);
//#define DO_RETURN(a) return Return(a);

int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. period is an example. */
period          ['.']
whitespace      '[\t\n\r ]'
%%

    // token definitions 
[\t\n\t ]*                  ;   /* Ignore whitespace */
[{}()\[\],;\.+\-*/%=]       {
                                yylval.int_val = yytext[0];
                                DO_RETURN(yytext[0]);
                            }
"print"                     DO_RETURN(PRINT);
"end"                       DO_RETURN(END);
[a-zA-Z_][a-zA-Z0-9_]*      {
                                yylval.symbol = g_SymbolTable.Find(yytext);
                                if (yylval.symbol == nullptr)
                                {
                                    yylval.symbol = new cSymbol(yytext);
                                }
                                DO_RETURN(IDENTIFIER);
                            }
[-]?[0-9]+                  { 
                                yylval.int_val = atoi(yytext);
                                DO_RETURN(INT_VAL); 
                            }
"//".*"\n"                  ;   /* Ignore comments */

%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

