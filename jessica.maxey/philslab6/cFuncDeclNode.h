#pragma once
//**************************************
// cFuncDeclNode.h
//
// Defines AST node for a function declaration
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Feb. 5, 2015
//

#include <assert.h>

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
            AddChild(type);
            AddChild(name);
            AddChild(nullptr);          // params
            AddChild(nullptr);          // decls
            AddChild(nullptr);          // stmts
            m_isDefinition = false;
            m_hasParams = false;

            cSymbol *func = g_SymbolTable.Find(name->GetName());
            cDeclNode *decl;
            cFuncDeclNode *funcDecl;

            if (func != nullptr)
            {
                decl = func->GetDecl();
                if (!decl->IsFunc())
                {
                    SemanticError(name->GetName() + 
                            " previously defined as other than a function");
                }
                else 
                {
                    funcDecl = dynamic_cast<cFuncDeclNode *>(decl);
                    if (funcDecl->GetType() != type->GetDecl())
                    {
                        SemanticError(name->GetName() + 
                            " previously defined with different return type");
                    }
                    else
                    {
                        m_children = funcDecl->m_children;
                        m_isDefinition = funcDecl->m_isDefinition;
                        m_hasParams = funcDecl->m_hasParams;
                        name->SetDecl(this);
                    }
                }
            }
            else
            {
                name->SetDecl(this);
                g_SymbolTable.Insert(name);
            }
        }

        // Add formal params to the declaration
        void AddParams(cDeclsNode *params)
        {
            cDeclsNode *old_params = GetParams();

            // If params have been set and either the old or new are
            // not NULL
            if (m_hasParams && (params != nullptr || old_params != nullptr))
            {
                if ( (params != nullptr && old_params == nullptr) ||
                     (params == nullptr && old_params != nullptr) ||
                     (params->NumChildren() != old_params->NumChildren()) )
                {
                    SemanticError(GetName() + 
                        " redeclared with a different number of parameters");
                    return;
                }

                for (int ii=0; ii<params->NumChildren(); ii++)
                {
                    cDeclNode *decl1 = params->GetDecl(ii);
                    cDeclNode *decl2 = old_params->GetDecl(ii);

                    assert(decl1 != nullptr && decl2 != nullptr);
                    if (decl1->GetType() != decl2->GetType())
                    {
                        SemanticError(GetName() +
                            " previously defined with different parameters");
                        return;
                    }
                }
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
            m_children[3] = decls;
        }

        // Add statements to the declaration.
        // Doing so means that this is a definition, not just a declaration
        // We will set the flag marking this as a definition.
        void AddStmts(cStmtsNode *stmts)
        {
            if (m_isDefinition)
            {
                SemanticError(GetName() + " already has a definition");
                return;
            }

            m_children[4] = stmts;
            m_isDefinition = true;
        }

        cDeclsNode *GetParams()
        {
            return (cDeclsNode *)GetChild(2);
        }

        cDeclsNode *GetLocals()
        {
            return (cDeclsNode *)GetChild(3);
        }

        cStmtsNode *GetStmts()
        {
            return (cStmtsNode *)GetChild(4);
        }

        virtual bool IsFunc() { return true; }

        // Return symbol of the type of declaration. 
        // Since this IS a type (in a sense), return self
        virtual cDeclNode *GetType() 
        { 
            cSymbol* type = dynamic_cast<cSymbol*>(m_children.front());

            return type->GetDecl(); 
        }

        // Return the name of the thing that is defined
        virtual string GetName() 
        { 
            cSymbol* name = dynamic_cast<cSymbol*>(GetChild(1));

            return name->GetName(); 
        }

        // Return a string representation of the node
        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        bool        m_isDefinition;     // flag indicating this is a definition,
                                        // not just a declaration
        bool        m_hasParams;        // params have been set
};
