#include "cVisitor.h"

class cComputeSize : public cVisitor
{
  public:
    cComputeSize() : cVisitor()
    {
        m_offset = 0;
        m_highwater = 0;
    }

    void VisitAllNodes(cAstNode *node) { node->Visit(this); }

    virtual void Visit(cBlockNode *node)
    {
        int start_offset = m_offset;
        int start_highwater = m_highwater;

        m_highwater = m_offset;
        VisitAllChildren(node);

        if (m_offset > m_highwater) m_highwater = m_offset;

        node->SetSize(m_highwater - start_offset);

        if (start_highwater > m_highwater) m_highwater = start_highwater;
        m_offset = start_offset;
    }

    virtual void Visit(cDeclsNode *node)
    {
        int start_offset = m_offset;

        VisitAllChildren(node);

        node->SetSize(m_offset - start_offset);
    }

    virtual void Visit(cFuncDeclNode *node)
    {
        int start_offset = m_offset;
        int start_highwater = m_highwater;

        m_offset = 0;
        m_highwater = 0;

        VisitAllChildren(node);

        node->SetSize( RoundUp(m_highwater) );

        m_highwater = start_highwater;
        m_offset = start_offset;
    }

    virtual void Visit(cParamsNode *node)
    {
        cAstNode::iterator it;
        for (it=node->FirstChild(); it!=node->LastChild(); it++)
        {
            if ((*it) != nullptr) (*it)->Visit(this);
            m_offset = RoundUp(m_offset);
        }

        node->SetSize(m_offset);
        if (m_offset > m_highwater) m_highwater = m_offset;
    }

    virtual void Visit(cStructDeclNode *node)
    {
        int start_offset = m_offset;

        m_offset = 0;

        VisitAllChildren(node);

        node->SetSize(m_offset);
        m_offset = start_offset;
    }

    virtual void Visit(cVarDeclNode *node)
    {
        node->SetSize(node->GetType()->GetSize());

        if (node->GetSize() != 1) m_offset = RoundUp(m_offset);

        node->SetOffset(m_offset);

        m_offset += node->GetSize();

        if (m_offset > m_highwater) m_highwater = m_offset;
    }

    virtual void Visit(cVarExprNode *node)
    {
        int offset = 0;
        cDeclNode *decl;

        for (int ii=0; ii<node->NumChildren(); ii++)
        {
            decl = node->GetElement(ii)->GetDecl();
            offset += decl->GetOffset();
        }
        node->SetOffset(offset);

        // decl is the last in the list, which is correct for size
        node->SetSize(decl->GetSize());
    }
  protected:
    int m_offset;
    int m_highwater;

    int RoundUp(int value)
    {
        if (value % WORD_SIZE == 0) return value;
        return value + WORD_SIZE - value%WORD_SIZE;
    }
};

