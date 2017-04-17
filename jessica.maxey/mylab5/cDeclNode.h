#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey
//
#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() 
        {}

        virtual cDeclNode * GetType() = 0;

        virtual bool IsFloat() { return false; }

        virtual bool IsVar() { return false; }

        virtual bool IsStruct() { return false; }

        virtual bool IsDef() { return false; }
            
        virtual bool IsFunc() { return false; }
        
        virtual string GetName() { return nullptr; } 

        virtual int GetSize() = 0;
};
