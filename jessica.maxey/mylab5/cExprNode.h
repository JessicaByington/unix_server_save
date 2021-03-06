#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey
//
#include "cStmtNode.h"

class cExprNode : public cStmtNode
{
    public:
        cExprNode() : cStmtNode() {}
        
        virtual cDeclNode *GetType() = 0;
        virtual int GetSize() { return 0; };
};
