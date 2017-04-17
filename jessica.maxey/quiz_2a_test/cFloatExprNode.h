#pragma once
//*************************************
// cFloatExprNode.h
//
//  Defines an AST node for a float constant (literals).
//
//  Inherits from cExprNode so that float constants can be used anywhere
//
//  Author: Jessica Maxey
//
//  Date: Jan. 26, 2016
//

#include "cAstNode.h"
#include "cExprNode.h"
class cFloatExprNode : public cExprNode
{
    public:
    cFloatExprNode(double value) : cExprNode()
    {
        m_value = value;
    }

    virtual string AttributesToString()
    {
        return " value=\"" + std::to_string(m_value) + "\"";
    }
    
    virtual string NodeType() { return string("float"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        double m_value;
};
