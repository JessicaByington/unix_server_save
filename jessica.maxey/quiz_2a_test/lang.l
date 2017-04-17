%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 22, 2015
//
// Edited by: Jessica Maxey

#include <iostream>

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
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

    // Definition of lang tokens
[\t\n\t ]*                  ;   /* Ignore whitespace */
[{}()\[\],;\.=]       DO_RETURN(yytext[0]);
"if"                        DO_RETURN(IF);
"else"                      DO_RETURN(ELSE);
"endif"                     DO_RETURN(ENDIF);
"while"                     DO_RETURN(WHILE);
"print"                     DO_RETURN(PRINT);

    /* Note: The following three lines will need to be changed */
    /*change the three lines from type to there respective types*/
"int"                       {
                                yylval.symbol = g_SymbolTable.Find("int");
                                DO_RETURN(TYPE_ID);
                            }

"float"                     {
                                yylval.symbol = g_SymbolTable.Find("float");
                                DO_RETURN(TYPE_ID);
                            }
"char"                      {
                                yylval.symbol = g_SymbolTable.Find("char");
                                DO_RETURN(TYPE_ID);
                            }   

"struct"                    DO_RETURN(STRUCT);
"return"                    DO_RETURN(RETURN);
[a-zA-Z_][a-zA-Z0-9_]*      { 
                              // NOTE: this code will have to be changed
                              // as part of this lab
                        
                              //Changed to global look up
                              yylval.symbol = g_SymbolTable.Find(yytext);
                              if (yylval.symbol == nullptr)
                              {
                                  yylval.symbol = new cSymbol(yytext); 
                                  //do not insert, leave that up to the grammar
                                  //g_SymbolTable.Insert(yylval.symbol);
                              }
                              DO_RETURN(IDENTIFIER);
                            }
[0-9]+                      {
                                yylval.int_val = atoi(yytext); 
                                DO_RETURN(INT_VAL); 
                            }
[0-9]+\.[0-9]+              {
                                yylval.float_val = atof(yytext);
                                DO_RETURN(FLOAT_VAL);
                            }
"%"                         {
                                yylval.int_val = MOD;
                                DO_RETURN(MOD);
                            }
"*"                         {
                                yylval.int_val = MULT;
                                DO_RETURN(MULT);
                            }
"/"                         {
                                yylval.int_val = DIV;
                                DO_RETURN(DIV);
                            }
"-"                         {
                                yylval.int_val = MINUS;
                                DO_RETURN(MINUS);
                            }
"+"                         {
                                yylval.int_val = PLUS;
                                DO_RETURN(PLUS);
                            }
"^"                         {
                                yylval.int_val = CARROT;
                                DO_RETURN(CARROT);
                            }
"=="                        {
                                yylval.int_val = EQUALS;
                                DO_RETURN(EQUALS);
                            }
"^^"                        {
                                yylval.int_val = CARROTS;
                                DO_RETURN(CARROTS);
                            }
"&&"                        DO_RETURN(AND);
"||"                        DO_RETURN(OR);
"//".*"\n"                  ;   /* Ignore comments */
.                           DO_RETURN(JUNK_TOKEN);
%%

// This function allows us to do extra processing on each token
// Used for creating debug output from scanner
int Return(int val)
{
    std::cout << "Scanned '" << yytext << "': " << val << std::endl;
    return val;
}

