#pragma once
//**************************************
// cBaseTypeNode
//
// Defines virtual base class for all declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cDeclNode.h"

class cBaseTypeNode : public cDeclNode
{
    public:
        cBaseTypeNode(string name, int size, bool isFloat) 
            : cDeclNode() 
        {
            m_name = name;
            m_size = size;
            m_isFloat = isFloat;
        }

        // return the symbol for the type
        virtual cDeclNode *GetType() { return this; }

        // return the name of the item that is declared
        virtual string  GetName() { return m_name; }

        virtual string NodeType() { return "type"; }

        virtual int Sizeof() { return m_size; }

        // return a string representation of the node
        virtual string AttributeToString()
        {
            return " name=\"" + m_name + "\" size=\"" + 
                std::to_string(m_size) +
                "\" isFloat=\"" + std::to_string(m_isFloat);
        }

        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        string m_name;
        bool   m_isFloat;
};
