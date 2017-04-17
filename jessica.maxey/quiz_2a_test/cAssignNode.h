#pragma once

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cVarExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
    cAssignNode(cVarExprNode *varexpr, cExprNode *expr) : cStmtNode()
    {
        AddChild(varexpr);
        AddChild(expr);
    }

    virtual string NodeType() { return string("assign"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); } 
};
