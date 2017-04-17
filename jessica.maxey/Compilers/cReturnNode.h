// Edited by Jessica Maxey
#pragma once

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cReturnNode : public cStmtNode
{
    public:
        cReturnNode(cExprNode *expr)
        {
            AddChild(expr);
        }

        virtual cExprNode * GetExpr()
        {
            return (cExprNode*)GetChild(0);
        }

        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
