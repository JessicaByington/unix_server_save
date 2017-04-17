#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cDeclsNode(cDeclNode *decl) : cAstNode()
        {
            AddChild(decl);
        }

        // Add a decl to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
        }

        cDeclNode *GetDecl(int index)
        {
            return (cDeclNode *)GetChild(index);
        }

        virtual void SetSize(int size) { m_size = size; }
        virtual int  GetSize() { return m_size; }

        // return the XML node name
        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual string AttributesToString()
        {
            if (m_size != 0)
            {
                return " size=\"" + std::to_string(m_size) + "\"";
            }
            else
            {
                return "";
            }
        }
    protected:
        int m_size;
};
