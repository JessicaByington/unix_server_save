#pragma once
//**************************************
// cFuncExprNode.h
//
// Defines AST node for function calls
//
// Inherits from cExprNode so that functions can be used in expressions
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey

#include "cAstNode.h"
#include "cStmtsNode.h"
#include "cParamListNode.h"
#include "cExprNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"
class cFuncExprNode : public cExprNode
{
    public:
        // params are the symbol for the func decl, and the params
        cFuncExprNode(cSymbol *name, cParamListNode *params)
            : cExprNode()
        {
            //make sure that the name coming in is a fun
            if(name->GetDecl()->IsFunc() == true)
            {
                cSymbol *sym = g_SymbolTable.FindLocal(name->GetName());
                cFuncDeclNode *decl = (cFuncDeclNode*)sym->GetDecl();
                cDeclsNode *decls = (cDeclsNode*)decl->GetChild(2); 
                cDeclsNode *param = (cDeclsNode*)params;

                if(decls->NumChildren() != param->NumChildren())
                {
                    SemanticError(dynamic_cast<cSymbol*>(decl->GetChild(1))->GetName() + " called with wrong number of arguments");
                }
                else if(decls->NumChildren() == param->NumChildren())
                {
                    for (int i = 0; i < decls->NumChildren(); i++)
                    {
                        if (decls->GetDecl(i)->GetType() != param->GetDecl(i)->GetType())
                        {
                            SemanticError(dynamic_cast<cSymbol*>(decl->GetChild(1))->GetName() + " called with incompatible argument");
                        }
                        else
                        {
                            AddChild(name);
                            AddChild(params);
                        }
                    }
                }
                else
                {
                    AddChild(name);
                    AddChild(params);
                }
            }
            else
                SemanticError(name->GetName() + " is not a function");
        }

        

        cDeclNode * GetType()
        {
            cSymbol *sym = dynamic_cast<cSymbol *>(m_children.front());
            return sym->GetDecl()->GetType();    
        }        

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

};
