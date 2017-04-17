#pragma once

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cParamListNode.h"

class cFuncExprNode : public cExprNode
{
    public: 
    cFuncExprNode(cSymbol *sym, cParamListNode *paramlist) : cExprNode()
    {
        AddChild(sym);
        AddChild(paramlist);
    }

    virtual string NodeType() { return string("funcCall"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
