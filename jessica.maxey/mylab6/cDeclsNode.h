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
// Edited by Jessica Maxey
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

        int GetSize()
        {
            return m_size;
        }

        void SetSize(int size)
        {
            m_size = size;
        }

        // return the sum of the sizes of the decls
        virtual int Sizeof()
        {
            int size = 0;

            cAstNode::iterator it;

            for (it = FirstChild(); it != LastChild(); it++)
            {
                size += dynamic_cast<cDeclNode *>(*it)->Sizeof();
            }
            return size;
        }

        // return the XML node name
        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
       
        virtual string AttributesToString ()
        {
            string attString = "";

            if (m_size > 0)
            {
                attString = " size = \"";
                attString += std::to_string(m_size);
                attString += "\"";
                return attString;
            }
        
            return attString;
        }

    protected:
        int m_size;
};
