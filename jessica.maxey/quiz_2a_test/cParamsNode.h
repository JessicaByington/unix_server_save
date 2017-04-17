#pragma once

#include "cAstNode.h"
#include "cExprNode.h"
#include "cDeclNode.h"

class cParamsNode : public cExprNode
{
    public:
    cParamsNode(cDeclNode *decl) : cExprNode()
    {
        AddChild(decl);
    }

    void Insert(cDeclNode *decl)
    {
        AddChild(decl);
    }

    virtual string NodeType() { return string("args"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
