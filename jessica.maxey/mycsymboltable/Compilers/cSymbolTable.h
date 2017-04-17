//**************************************************
//cSymbolTable.h
//
//A symbol table definition file for CST 320 project
//
//Author: Jessica Maxey
//
//Date: 1/19/2016
//
#include "cSymbol.h"
#include "tokens.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <unordered_map>
using std::unordered_map;

#include <string>
using std::string;

#include <vector>
using std::vector;

class cSymbolTable
{
    public:
    //create a ctor
    
    cSymbolTable ()
    {
        mystack.push_back(unordered_map<string, cSymbol *>());
    } 

    void IncreaseScope()
    {
        //create new symbol table
        //push to stack of symbol tables
        mystack.push_back(unordered_map<string, cSymbol *>());
    }


    void DecreaseScope()
    {
        //pop current symbol table off stack of symbol tables
        //next one down becomes current symbol table
        mystack.pop_back();
    }

    cSymbol * InsertIdentifier(string identifier)
    {
        //searchs scopes to see if the identifier is already
        //being used. 
        //Will return nullptr if not found
        cSymbol * foundid = DefaultLookUpIdentifier(identifier);

        //if lookup does not contain the identifier
        //then add one to the symbol table
        if (foundid == nullptr)
        {
            //symboltable[identifier] = new cSymbol(identifier);
            //adds the new identifier to the current(last) symboltable on the
            //stack.
            mystack[mystack.size() - 1][identifier] = new cSymbol(identifier);
        }

        
        return mystack[mystack.size() - 1][identifier];
    }


    cSymbol * LookUpIdentifier(string identifier)
    {
        //looks at the most recently added symbol table
        //and if it was not found, will go onto the next
        //scope, starting from bottom up since this is a
        //push back vector.
        for (int i = mystack.size() - 1; i >= 0; i--)
        {
            //if (symboltable.count(identifier > 0))
            //seg fault
            if (mystack[i].count(identifier) > 0)
            {
                //identifier found, return true
                //return symboltable[identifier];
                return mystack[i][identifier];
            }
        } 
        
        //identifier not found, return false
        return nullptr;
    }

    cSymbol * DefaultLookUpIdentifier(string identifier)
    {
        //Looks at the inner most scope (the last one added to the stack)
        //which in the case is the last element in the vector
        if (mystack[mystack.size() - 1].count(identifier) > 0)
        {
            //if the identifier was found, return true
            return mystack[mystack.size() - 1][identifier];
        }

        //the identifier was not found, return false
        return nullptr;
    }




    private:
        vector<unordered_map<string, cSymbol *>> mystack;
        //unordered_map<string, cSymbol *> symboltable;
};

