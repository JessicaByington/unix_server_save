%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//
// Edited by Jessica Maxey

#include <iostream>
using std::cout;

#include <string>
using std::to_string;

#include "lex.h"

%}

%locations

 /* union defines the type for lexical values */
%union{
    int             int_val;
    char *          string_val;
    cSymbol*        symbol;
    cSymbolTable::symbolTable_t*  sym_table;
    }

%{
    int yyerror(const char *msg);
%}

%start  program

%token <symbol>    IDENTIFIER
%token <int_val>   INT_VAL

%token PRINT
%token END

%type <symbol> stmt
%type <int_val> expr
%%

program: stmt                   { 
                                  if (yynerrs == 0)
                                    YYACCEPT;
                                  else
                                    YYABORT;
                                }

stmt: IDENTIFIER ':' expr ';'   { $$ = $1; }
    | PRINT expr ';'            { 
                                  $$ = $2; 
                                  cout << $$ << '\n';
                                }
    | END ';'                   {} 
   
expr: '+' expr expr             { $$ = $2 + $3; }                                
    | '-' expr expr             { $$ = $2 - $3; }
    | '*' expr expr             { $$ = $2 * $3; }
    | '/' expr expr             { $$ = $2 / $3; }
    | INT_VAL                   { $$ = $1; }
    | IDENTIFIER                { $$ = $1; }

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg << " at symbol "
        << yytext << " on line " << yylineno << "\n";

    return 0;
}
