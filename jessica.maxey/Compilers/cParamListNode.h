#pragma once
//**************************************
// cParamListNode.h
//
// Defines AST node for actual params passed to a function
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//
// Edited by Jessica Maxey

#include "cAstNode.h"
#include "cExprNode.h"

class cParamListNode : public cAstNode
{
    public:
        // param is first actual param passed to function
        cParamListNode(cExprNode *param)
            : cAstNode()
        {
            AddChild(param);
        }

        // add the next actual param
        void Insert(cExprNode *param)
        {
            AddChild(param);
        }

        cExprNode *GetParam(int index)
        {
            return (cExprNode *)GetChild(index);
        }

        virtual int GetSize()
        {
            return m_size;
        }

        virtual void SetSize(int size)
        {
            m_size = size;
        }

        virtual int GetOffset()
        {
            return m_offset;
        }

        virtual void SetOffset(int offset)
        {
            m_offset = offset;
        }

        // return a string representation of the node
        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_offset;
        int m_size;
};
