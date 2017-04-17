#pragma once
//**************************************
// cBlockNode.h
//
// Defines AST node for a block of code (stuff inside {})
//
// Since blocks can take the place of statements, this class inherits from 
// cStmtNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//
// Edited by Jessica Maxey
//
#include "cAstNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cBlockNode : public cStmtNode
{
    public:
        // params are the decls and statements contained in the block
        cBlockNode(cAstNode *decls, cStmtsNode *statements)
            : cStmtNode()
        {
            AddChild(decls);
            AddChild(statements);
        }

        virtual int GetSize()
        {
            return m_size;
        }
    
        virtual void SetSize(int size)
        {
            m_size = size;
        }

        // return the XML name of the node
        virtual string NodeType() { return string("block"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString()
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
