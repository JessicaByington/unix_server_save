#pragma once
//**************************************************
// cBinaryExprNode.h
//
// Defines an AST node for a binary expr made up of two literals
// and a operator inbetween
//
// Inherits from cExprNode so that binaryexpr can be used anywhere
//
// Author: Jessica Maxey
// 
// Date: Jan. 26, 2016

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cAstNode *first_val, int op, cAstNode *second_val) : cExprNode()
        {
            AddChild(first_val);
            AddChild(new cOpNode(op));
            AddChild(second_val);
        }
        
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
