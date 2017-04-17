#pragma once
//**************************************
// cStructDeclNode.h
//
// Defines AST node for a struct declaration
//
// Inherits from cDeclNode because this is a type of declaration
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Jan. 18, 2016
//
// Edited by Jessica Maxey
//
#include "cAstNode.h"
#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include <unordered_map>
using std::unordered_map;

class cStructDeclNode : public cDeclNode
{
    public:
        typedef unordered_map<string, cSymbol*> symbolTable_t;
        // params are: 
        //     the symbol table for the declarations that make up this struct
        //     the declarations that make up this struct
        //     the cSymbol for the name of the struct
        cStructDeclNode(cSymbolTable::symbolTable_t *symTbl, 
                        cDeclsNode *decls, 
                        cSymbol *struct_id)
            : cDeclNode()
        {
            cSymbol *name;

            AddChild(decls);
            m_symTbl = symTbl;

            struct_id->SetDecl(this);

            // Figure out if the ID we were passed already exists in the 
            // local symbol table. 
            name = g_SymbolTable.FindLocal(struct_id->GetName());
            if (name == nullptr)
            {
                // No: this is good. A later lab will cause an error if it does
                name = struct_id;

                // If the symbol exists in an outer scope, we need to create
                // a new one instead of re-using the symbol from the outer scope
                if (g_SymbolTable.Find(struct_id->GetName()) != nullptr)
                {
                    name = new cSymbol(struct_id->GetName());
                }

                // insert the name of the struct into the global symbol table
                g_SymbolTable.Insert(name);
            }

            AddChild(name);
            m_name = name->GetName();
        }

        cDeclNode * GetType()
        {
            return this;
        }

        bool IsStruct()
        {
            return true;
        }

        string GetName()
        {
            return m_name;
        }

        cSymbol * LookUp(symbolTable_t *table, string& name)
        {
            symbolTable_t::const_iterator got = table->find(name);

            if(got == table->end())
                return nullptr;
            else
                return got->second;
        }

        cSymbol * GetElement(string name)
        {
            cSymbol * elem = LookUp(m_symTbl, name);
            
            if(LookUp(m_symTbl, name) != nullptr)
            {
                return elem;
            }
            else
                return nullptr;
        }

        int GetSize()
        {
            return 0;
        }

        virtual string NodeType() { return string("struct_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        cSymbolTable::symbolTable_t *m_symTbl; 
        string m_name;

};
