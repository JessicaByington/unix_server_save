#pragma once
//**************************************
// cFuncDeclNode.h
//
// Defines AST node for a function declaration
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey
//
#include "cAstNode.h"
#include "cStmtsNode.h"
#include "cDeclsNode.h"
#include "cDeclNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // params are the return type and name of function
        cFuncDeclNode(cSymbol *type, cSymbol *name)
            : cDeclNode()
        {
        //local lookup, if it wasn't a function, error,
        //if they are not the same(the cSymbol*type coming in) return, error

            cSymbol *func = g_SymbolTable.FindLocal(name->GetName());

            if(func == nullptr)
            {
                name->SetDecl(this);
                m_isDefinition = false;
                g_SymbolTable.Insert(name);

                AddChild(type);
                AddChild(name);
            }
            else 
            {
                cFuncDeclNode * decl = (cFuncDeclNode*)func->GetDecl();
                
                if(func->GetDecl()->IsFunc() == false)
                {
                    SemanticError(name->GetName() + " previously defined as other than a function");
                    AddChild(type);
                    AddChild(name);
                }
                else if(func->GetDecl()->GetType() != type->GetDecl()->GetType())
                {
                    SemanticError(name->GetName() + " previously defined with different return type");
                    AddChild(type);
                    AddChild(name);
                }
                else
                {
                    m_isDefinition = decl->m_isDefinition;
                    g_SymbolTable.Insert(name);

                    name->SetDecl(this);
                    AddChild(type);
                    AddChild(name);
                    
                    if(decl->m_hasParams)
                    {
                        AddChild(decl->GetChild(2));
                        m_hasParams = true;
                    }

                    if (decl->m_isDefinition)
                    {
                        AddChild(decl->GetChild(3));
                        AddChild(decl->GetChild(4));
                        m_isDefinition = true;
                    }
                }
            }
        }

        // Add formal params to the declaration
        void AddParams(cDeclsNode *params)
        {
            if (!m_hasParams) 
            {
                AddChild(params);
            }
            else
            {
                //check to see if they have the same number, num_children
                //then do a iterator, then do getdecl->gettype and see if they are the same    
                if(params != nullptr && GetChild(2) != nullptr)
                {
                    cDeclsNode * decls = (cDeclsNode*)GetChild(2);
                    if(decls->NumChildren() != params->NumChildren())
                    {
                        SemanticError(dynamic_cast<cSymbol*>(GetChild(1))->GetName() + " redeclared with a different number of parameters ");
                    }
                    else
                    {
                        for (int i = 0; i < decls->NumChildren(); i++)
                        {
                            if (decls->GetDecl(i)->GetType() != params->GetDecl(i)->GetType())
                            {
                                SemanticError(dynamic_cast<cSymbol*>(GetChild(1))->GetName() + "previously defined with different parameters ");
                            }
    
                        }

                    }
                }
                else if (params == nullptr && GetChild(2) != nullptr)
                    SemanticError(dynamic_cast<cSymbol*>(GetChild(1))->GetName() + " is declared with params, but has none when defined");
                else if (params != nullptr && GetChild(2) == nullptr)
                    SemanticError(dynamic_cast<cSymbol*>(GetChild(1))->GetName() + " is declared without params, but has then when defined");
            }
            
            m_children[2] = params;
            m_hasParams = true;
        }

        // Add declarations to the declaration.
        // Doing so means this is a definition, not just a declaration
        // BUT, we will wait for AddStmts to mark it as a definition
        // because our grammar requires all functions to have statements.
        void AddDecls(cDeclsNode *decls)
        {
            AddChild(decls);
        }

        // Add statements to the declaration.
        // Doing so means that this is a definition, not just a declaration
        // We will set the flag marking this as a definition.
        void AddStmts(cStmtsNode *stmts)
        {
            //cSymbol * name = dynamic_cast<cSymbol*>(m_children.back());

            if (m_isDefinition == true)
                SemanticError(dynamic_cast<cSymbol*>(GetChild(1))->GetName() +  " already has a definition");
            else
            {
                AddChild(stmts);
                m_isDefinition = true;
            }
        }

        cDeclNode * GetType()
        {
            cSymbol * type = dynamic_cast<cSymbol*>(m_children.front());
            return type->GetDecl();
        }

        cDeclNode * GetDecl()
        {
            return this;
        }

        int GetSize()
        {   
            cSymbol * size = dynamic_cast<cSymbol*>(m_children.back());
            
            return size->GetDecl()->GetSize();
        }

        virtual bool IsFunc()
        {
            return true;
        }

        bool IsDef()
        {
            return m_isDefinition;
        }

        
        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        bool        m_isDefinition;     // flag indicating this is a definition,
                                        // not just a declaration
        bool        m_hasParams;
};
