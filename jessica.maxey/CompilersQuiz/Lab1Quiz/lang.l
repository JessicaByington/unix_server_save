%{
//**************************************
// lang.l
//
// Scanner definition file for CST 320 project
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 23, 2015
//
// Edited by: Jessica Maxey

#include "tokens.h"

#define DO_RETURN(a) return (a)
//#define DO_RETURN(a) return Return(a)
int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


     /* definitions go here. period is an example. */


%%

    // token definitions go here. "if" is an example.
"my"			    DO_RETURN(MY);
"token"			    DO_RETURN(TOKEN);
"xq"+			    DO_RETURN(BLOB1);
[0-9]*"qx"+   		    DO_RETURN(BLOB2);
")"			    DO_RETURN(')');
"+" 			    DO_RETURN('+');	
[ \t\n]			    {}
.			    DO_RETURN(JUNK);

%%

// This function allows us to do extra processing on each token
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

