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
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey

#include "cSymbol.h"
#include "cAstNode.h"
#include "cExprNode.h"
#include "cDeclNode.h"

class cVarExprNode : public cExprNode
{
    public:
        // param is the symbol for the variable
        cVarExprNode(cSymbol *name) : cExprNode()
        {
            if (g_SymbolTable.Find(name->GetName()) == nullptr)
            {
                SemanticError("Symbol " + name->GetName() + " not defined ");
            }
            
            AddChild(name);
        }
        
        string GetName()
        {
           //iterate through the vector and compose a string for the error
           string errorString = "";

           for (int i = 0; i < (int)m_children.size(); i++)
           {
                cSymbol * temp = dynamic_cast<cSymbol *>(m_children[i]);

                errorString += temp->GetName();
            
                if((i + 1) < (int)m_children.size())
                {
                    errorString += ".";
                }
           } 

           return errorString;
        }

        // called for the fields in struct refs
        void AddElement(cSymbol *name)
        {
            //Get the last element in the vector
            cSymbol *sym = dynamic_cast<cSymbol*>(m_children.back());
            

            //if it does not equal a struct, then ERROR
            if(sym->GetDecl()->GetType()->IsStruct() == false)
            {
                SemanticError(GetName() + " is not a struct");
            }
            else
            {
                //get the type
                cStructDeclNode *str = dynamic_cast<cStructDeclNode*>(sym->GetDecl()->GetType());

                //Go to cStructNode, and see if it is in the symboltable
                sym = str->GetElement(name->GetName());

                //if it is not, ERROR
                if (sym == nullptr)
                {
                    SemanticError(name->GetName() + " is not a field of " + GetName());
                }
            }

            //Actually add the new element
            AddChild(sym);
        }

        cDeclNode * GetType()
        {
            cSymbol * sym = dynamic_cast<cSymbol *>(m_children.back());

            return sym->GetDecl()->GetType();
        }

        cDeclNode * GetDecl()
        {
            cSymbol * sym = dynamic_cast<cSymbol *>(m_children.back());
            
            return sym->GetDecl();
        }


        int GetSize()
        {
            cSymbol * size = dynamic_cast<cSymbol *>(m_children.back());

            return size->GetDecl()->GetSize();
        }
        
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        
};
