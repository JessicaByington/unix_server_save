#pragma once
//**************************************
// cOpNode.h
//
// Class for an operator (e.g. + - etc.)
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//

#include "cAstNode.h"
//#include "langparse.h"
//make a cOpNode.cpp and include langpase.h
class cOpNode : public cAstNode
{
    public:
        cOpNode(int op) : cAstNode()
        {
            m_op = op;
        }

        virtual string NodeType()       { return "op"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string AttributesToString();
        /*{
            switch(m_op)
            {
                case EQUALS:
                    return string(" value='") + "=='";
                default:
                    return string(" value='") + (char)m_op + "'";
            }
        }*/
        
    protected:
        int m_op;      // the operand
};

