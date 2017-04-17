#pragma once
//**************************************
// cIfNode.h
//
// Defines an AST node for a if statement
//
// Inherites from cAstNode
//
// Author: Jessica Maxey
//
// Date: Jan. 29, 2016
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cIfNode : public cStmtNode
{
    public:
        cIfNode(cExprNode *expr, cStmtsNode *ifstmt, cStmtsNode *elsestmt) : cStmtNode()
        {
            AddChild(expr);
            AddChild(ifstmt);
            AddChild(elsestmt);
        }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
