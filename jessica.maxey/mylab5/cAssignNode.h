#pragma once
//**************************************
// cAssignNode.h
//
// Defines AST node for assignment statments
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey
//
#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"
#include "cVarExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        // params are the lval and the expression
        cAssignNode(cVarExprNode *lval, cExprNode *expr)
            : cStmtNode()
        {
            if(lval->GetType() == expr->GetType())
            {
                AddChild(lval);
                AddChild(expr);
            }
            else
            {
                //check if struct at all, no
                if (lval->GetType()->IsStruct() || expr->GetType()->IsStruct())
                    SemanticError("Cannot assign " + expr->GetType()->GetName() + " to " + lval->GetType()->GetName() );
                if(lval->GetDecl()->IsVar() != true)
                     SemanticError(dynamic_cast<cSymbol*>(lval->GetDecl()->GetChild(1))->GetName() + " is not an lval" );
                
                //left is a int and right is a float, no
                else if((lval->GetType()->IsFloat() != true) && (expr->GetType()->IsFloat()))
                    SemanticError("Cannot assign " + expr->GetType()->GetName() + " to " + lval->GetType()->GetName() );
                //if left is a char, no
                else if(lval->GetSize() == 1)
                    SemanticError("Cannot assign " + expr->GetType()->GetName() + " to " + lval->GetType()->GetName() );
                //make sure the left is a var
                else
                {
                    AddChild(lval);
                    AddChild(expr);
                }
            }
        }

        

        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
