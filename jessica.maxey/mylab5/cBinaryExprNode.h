#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines AST node for binary expressions.
// Inherits from cExprNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey

#include "cAstNode.h"
#include "cExprNode.h"
#include "cOpNode.h"
#include "cSymbol.h"
#include "cDeclNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // params are the left and right expressions and the operation.
        // The operation is a char: '+', '-', etc.
        cBinaryExprNode(cExprNode *left, int op, cExprNode *right)
            : cExprNode()
        {
            AddChild(left);
            AddChild(new cOpNode(op));
            AddChild(right);

        }

        cDeclNode * GetType()
        {
            cDeclNode *left = dynamic_cast<cExprNode *>(GetChild(0))->GetType();
            cDeclNode *right = dynamic_cast<cExprNode *>(GetChild(2))->GetType();

            //promition rules
            if (left->IsFloat())
            {
                return left;
            }
            else if (right->IsFloat())
            {
                return right;
            }
            else if (left->GetSize() > right->GetSize())
            {
                return left;
            }
            else
            {
                return right;
            }
        }        

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

};
