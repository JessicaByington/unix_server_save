//**************************************
// parse.cpp
//
// Starting point for top-down recursive-descent parser
// Used in CST 320 Lab2
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 23, 2015
//
// Edited by: Jssica Maxey
#include <iostream>
using std::cout;
using std::endl;
#include "lex.h"
#include "parse.h"
#include "utils.h"
#include "tokens.h"
const int END_OF_FILE = 0;
//*******************************************
// Find a PROG non-terminal
//

bool FindPROG()
{
    int token;
    
    if (!FindSTMTS()) return false;

    token = PeekToken();
    if (token != END) return false;
    AdvanceToken();
    
    return true;
}

bool FindSTMTS()
{  
    /*&int token;

    if (!FindSTMT()) return true;
//    cout << "Found a statement" << endl;

    token = PeekToken();
    if (token != ';' && token != END && token != END_OF_FILE)
    {
        if (!FindSTMTS()) return false;
    }

    return true;*/

    while (FindSTMT())
    {}

    return true;
}

bool FindSTMT()
{
    int token;

    if (FindEXPR())
    {
        token = PeekToken();

        if (token == ';')
        {
            //found a ;
            AdvanceToken();
            cout << "Found a statement" << endl;
            return true;
        }
        else if (token == END)
        {
            return true;
        }
        else if (token != ';')
        {
            Error("';'");
        }
        
    }

    token = PeekToken();
    
    /*if (token != ';')
    {
        cout << "'FindSTMT" << endl;
        Error("';'");
    }*/

    while (token != ';' && token != END && token != END_OF_FILE)
    {
        //syncing
        AdvanceToken();
        token = PeekToken();
    }
    
    if (token == ';')
    {
        //found a ;
        AdvanceToken(); 
        return true;
    }

    return false;
}

bool FindEXPR()
{
    int token;

    token = PeekToken();

    if (token ==  '(' )
    {
        //found a (
        AdvanceToken();

        if(!FindEXPR()) return false;
        
        token = PeekToken();

        if (token != ')')   
        { 
            Error("')'");
            return false;
        }
        
        //found a )
        AdvanceToken();


        if (!FindEXPR_P()) return false;

        return true;
    }
    else if (FindTERM()) 
    {
        return true;
    }
    
    return false; 
}

bool FindEXPR_P()
{
    int token;

    //if lamda, return true
    if (!FindPLUSOP()) return true;

    token = PeekToken();
    if (token != '(') 
    { 
        Error("'('");
        return false;
    }
    //found a (
    AdvanceToken();


    if (!FindEXPR()) return false;

    token = PeekToken();

    if (token != ')') 
    {
       Error("')'");
       return false;
    }
    
    //found a )
    AdvanceToken();

    if (!FindEXPR_P()) return false;
   
    return true;
}

bool FindPLUSOP()
{
    int token;
    token = PeekToken();

    if (token == '+' || token == '-')
    {
        //found a + or -
        AdvanceToken();

        return true;
    }
 
    return false;
}

bool FindTERM()
{
    int token;

    token = PeekToken();

    if (token == '[')
    {
        //found a [ 
        AdvanceToken();

        if(!FindEXPR()) return false; 
        
        token = PeekToken();
            
        if (token != ']') 
        {
            Error("']'");
            return false;
        }
        
        //found a ]
        AdvanceToken();

        if (!FindTERM_P()) return false;

        return true;
    }
    else if (token == INT_VAL) 
    {
        //found a INT_VAL
        AdvanceToken(); 
        return true;
    }

    return false;
}

bool FindTERM_P()
    
{
    int token;

    //if lamda, return true
    if (!FindTIMESOP()) return true;
    
    token = PeekToken();
    if (token != '[') 
    {
        Error("'['");
        return false;
    }

    //found a [
    AdvanceToken();

    if (!FindEXPR()) return false; 
   
    token = PeekToken();

    if (token != ']') 
    {
        Error("']'");
        return false;
    }
   
    //found a ] 
    AdvanceToken();


    if (!FindTERM_P()) return false;

   return true;
}

bool FindTIMESOP()
{
   int token;
   token = PeekToken();

   if (token == '*' || token == '/')
    {
        //found a * or /
        AdvanceToken();


        return true;
   }

   return false;
}

