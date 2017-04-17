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

        virtual void SetSize(int size) { m_size = size; }
        virtual int  GetSize()         { return m_size; }

        // return the XML name of the node
        virtual string NodeType() { return string("block"); }
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
