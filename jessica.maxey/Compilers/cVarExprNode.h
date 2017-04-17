#pragma once
//**************************************
// cVarExprNode.h
//
// Defines AST node for a variable reference
//
// Inherits from cExprNode so variable refs can be used in expressions`
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//

#include <assert.h>

#include "cSymbol.h"
#include "cAstNode.h"
#include "cExprNode.h"

class cVarExprNode : public cExprNode
{
    public:
        // param is the symbol for the variable
        cVarExprNode(cSymbol *name)
            : cExprNode()
        {
            cSymbol *sym = g_SymbolTable.Find(name->GetName());
            if (sym == nullptr)
            {
                string error("");
                error += "Symbol " + name->GetName() + " not defined";
                SemanticError(error);
            }

            AddChild(name);
        }

        // called for the fields in struct refs
        void AddElement(cSymbol *name)
        {
            cSymbol* sym;
            sym = dynamic_cast<cSymbol*>(m_children.back());

            if (sym->GetDecl()->GetType()->IsStruct())
            {
                cStructDeclNode *decl;
                decl = dynamic_cast<cStructDeclNode *>(sym->GetDecl()->GetType());
                assert(decl != nullptr);

                cSymbol* field = decl->GetField(name->GetName());
                if (field != nullptr)
                {
                    AddChild(field);
                }
                else
                {
                    SemanticError( name->GetName() + " is not a field of " +
                            GetName());
                }
            }
            else
            {
                SemanticError( GetName() + " is not a struct" );
            }
        }

        // return a string representation of the name of the var
        virtual string GetName()
        {
            string name("");
            cSymbol* sym;
            cAstNode::iterator it = FirstChild();

            sym = dynamic_cast<cSymbol*>(*it);

            name += sym->GetName();

            for (it++; it != LastChild(); it++)
            {
                sym = dynamic_cast<cSymbol*>(*it);
                name += "." + sym->GetName();
            }

            return name;
        }
            
        // Return the type of the var
        virtual cDeclNode* GetType()
        {
            cSymbol* sym;

            sym = dynamic_cast<cSymbol*>(m_children.back());

            return sym->GetDecl()->GetType();
        }

        // Return the declaration of the var
        virtual cDeclNode* GetDecl()
        {
            cSymbol* sym;

            sym = dynamic_cast<cSymbol*>(m_children.back());

            return sym->GetDecl();
        }

        void SetOffset(int offset) { m_offset = offset; }
        void SetSize(int size)     { m_size = size; }
        int GetOffset()            { return m_offset; }
        int GetSize()              { return m_size; }

        cSymbol* GetElement(int index)
        {
            return (cSymbol*)GetChild(index);
        }

        // return a string representation of the node
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        virtual string AttributesToString()
        {
            if (m_size != 0 || m_offset != 0)
            {
                return " size=\"" + std::to_string(m_size) + "\" offset=\"" +
                    std::to_string(m_offset) + "\"";
            }
            else
            {
                return "";
            }
        }
    protected:
        int m_offset;
        int m_size;
};
