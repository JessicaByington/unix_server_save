#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines AST node for a variable declaration
//
// Inherits from cDeclNode so variables can be added to lists of decls
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cVarDeclNode : public cDeclNode
{
    public:
        // params are the symbol for the type and the name
        cVarDeclNode(cSymbol *type_id, cSymbol *var_id)
        {
            AddChild(type_id);


            // If the symbol exists in an outer scope, we need to create
            // a new one instead of reusing the outer scope's symbol.
            if (g_SymbolTable.FindLocal(var_id->GetName()) != nullptr)
            {
                SemanticError("Symbol " + var_id->GetName() + " already defined in current scope");
            }
            else if (g_SymbolTable.Find(var_id->GetName()) != nullptr)
            {
                var_id = new cSymbol(var_id->GetName());
            }

            // Insert into the global symbol table
            g_SymbolTable.Insert(var_id);

            var_id->SetDecl(this);

            AddChild(var_id);
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
            cSymbol * size = dynamic_cast<cSymbol *>(m_children.back());
            return size->GetDecl()->GetType()->GetSize();
        }

        virtual bool IsVar()
        {
            return true;
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
