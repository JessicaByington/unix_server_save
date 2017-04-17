#pragma once
//**********************************
// cVarExprNode.h
//
// Defines AST node for variable expressions
//
// Inherites from cStmtNode
//
// Author: Jessica Maxey
//
// Date: Jan. 29, 2015
//

#include "cAstNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
    public: 
        cVarExprNode(cSymbol *sym) : cExprNode()
        {
            AddChild(sym);
        }      

        void Insert(cSymbol *sym)
        {
            AddChild(sym);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
