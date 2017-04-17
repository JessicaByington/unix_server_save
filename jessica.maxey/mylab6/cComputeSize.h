#pragma once
//*********************************************
// cComputeSize.h
//
// Defines a way to compute the size and offset 
// for objects
//
// Inherits from cVisitor
//
// Author: Jessica Maxey
//
// Date: Feb 23, 2016

#include "cAstNode.h"

class cComputeSize : public cVisitor
{
    public:
    cComputeSize() : cVisitor()
    {
        m_offset = 0;
        m_highWater = 0;
    }

    virtual void VisitAllNodes(cAstNode *node)
    {
        node->Visit(this);
    }

    virtual void Visit(cBlockNode *node)
    {
        int start_offset = m_offset;
        int start_highWater = m_highWater;

        m_highWater = m_offset;

        VisitAllChildren(node);
        int size = m_highWater - start_offset;
        
        node->SetSize(size);
        m_offset = start_offset;

        if (start_highWater > m_highWater)
            m_highWater = start_highWater;
    }

    virtual void Visit(cDeclsNode *node)
    {
        int start_offset = m_offset;
        VisitAllChildren(node);
        int size = m_offset - start_offset;

        node->SetSize(size);
    }

    virtual void Visit(cStructDeclNode *node)
    {
        int start_offset = m_offset;
        m_offset = 0;
        VisitAllChildren(node);
        int size = m_offset;
        
        node->SetSize(size);
        m_offset = start_offset;

    }

    virtual void Visit(cFuncDeclNode *node)
    {
        int start_offset = m_offset;
        int start_highWater = m_highWater;
        m_offset = 0;
        m_highWater = 0;
        VisitAllChildren(node);
        m_highWater = RoundUp(m_highWater);
        int size = m_highWater;

        node->SetSize(size);
        m_offset = start_offset;
        m_highWater = start_highWater;
    }

    virtual void Visit(cParamsNode *node)
    {
        cAstNode::iterator it;
        for (it=node->FirstChild(); it!=node->LastChild(); it++)
        {
            if ((*it) != nullptr) (*it)->Visit(this);
            m_offset = RoundUp(m_offset);
            int size = m_offset;
            node->SetSize(size);

        }
    }

    virtual void Visit(cVarExprNode *node)
    {
        int node_offset = 0;
        //visist all the children, by using the code inside visitallchildren
        //and edit it do the stuff that makes it a is the right offset and 
        //then b and c will be adding to that offset
        cAstNode::iterator it;
        for (it=node->FirstChild(); it!=node->LastChild(); it++)
        {
            if ((*it) != nullptr) (*it)->Visit(this);
            //make a get element inside of var expr that does typecast
            //and returns a symbol
            //then do getdecl getoffset
            
          
            cSymbol * sym = node->GetElement(it);  
            node_offset += sym->GetDecl()->GetOffset();
            node->SetOffset(node_offset);
            node->SetSize(node->GetType()->GetSize());
        }

    }

    virtual void Visit(cVarDeclNode *node)
    {
        node->SetSize( node->GetType()->GetSize());

        if (node->GetType()->GetSize() == 1)
        {
            node->SetOffset(m_offset);
            m_offset += node->GetSize();
        }
        else
        {
            //Check to see if the offset is word aligned
            m_offset = RoundUp(m_offset);
            node->SetOffset(m_offset);
            m_offset += node->GetSize();
        }
        
        //calc new highwater mark
        if (m_offset > m_highWater)
            m_highWater = m_offset;
    
    }

    protected: 
    int m_offset;
    int m_highWater;

    int RoundUp(int offset)
    {
        while ((offset % 4) != 0)
        {
            offset += 1;
        }

        return offset;
    }
};
