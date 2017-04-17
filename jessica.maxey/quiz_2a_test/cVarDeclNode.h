#pragma once
//************************************************
//
// cVarDeclNode.h
//
// Defines an AST node for adding a type and name to the symboltable
//
// Inherits from cDeclNode
//
// Author: Jessica Maxey
//
// Date: Jan. 27 2016
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cVarDeclNode : public cDeclNode
{
    public:
    cVarDeclNode(cSymbol * type, cSymbol * name) : cDeclNode()
    {

        if ((g_SymbolTable.Find(name->GetName())) != nullptr)
        {
            name = new cSymbol(name->GetName());
        }

        g_SymbolTable.Insert(name);
        AddChild(type);
        AddChild(name);    
    }

    virtual string NodeType() { return string ("var_decl"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

};
