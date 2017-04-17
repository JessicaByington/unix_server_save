#pragma once
//*************************************
// cBaseTypeNode.h
//
// Is used to attach types to decls
//
// Author: Jessica Maxey
//
// Date: Feb. 11, 2016
//
//

#include "cDeclNode.h"

class cBaseTypeNode : public cDeclNode
{
    public:
        cBaseTypeNode(string name, int size, bool isFloat) : cDeclNode()
        {
            m_name = name;
            m_size = size;
            m_isFloat = isFloat;
        }

        virtual string GetName() { return m_name; }

        virtual int GetSize() { return m_size; }

        virtual bool IsFloat() { return m_isFloat; }

        virtual cDeclNode * GetType(){ return this; }

        virtual string NodeType() { return string("type"); }

        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
    string m_name;
    int m_size;
    bool m_isFloat;
};
