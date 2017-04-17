#pragma once

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cStructDeclNode : public cDeclNode
{
    public:
    cStructDeclNode(cDeclsNode *decls, cSymbol *name) : cDeclNode()
    {
        if ((g_SymbolTable.Find(name->GetName())) != nullptr)
        {
            name = new cSymbol(name->GetName());
        }

        g_SymbolTable.Insert(name);
        AddChild(decls);
        AddChild(name);        
    }
    
    virtual string NodeType() { return string("struct_decl"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
