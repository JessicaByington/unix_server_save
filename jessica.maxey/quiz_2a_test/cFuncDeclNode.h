#pragma once

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cParamsNode.h"
#include "cDeclsNode.h"
#include "cStmtsNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
    cFuncDeclNode(cSymbol *type, cSymbol *name) : cDeclNode()
    {
        if((g_SymbolTable.Find(name->GetName())) != nullptr)
        {
            name = new cSymbol(name->GetName());
        }

        g_SymbolTable.Insert(name);
        AddChild(type);
        AddChild(name);
    }

    void AddStmts (cStmtsNode *stmts)
    {
        AddChild(stmts);    
    }

    void AddDecls (cDeclsNode *locals)
    {
        AddChild(locals);
    }

    void AddParams(cParamsNode *params)
    {
        AddChild(params);
    }

    virtual string NodeType() { return string("func"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

};
