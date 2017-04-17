#pragma once
//**************************************
// cDeclNode
//
// Defines virtual base class for all declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//
// Edited by Jessica Maxey

#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        virtual void ComputeSizeOffset()
        {
        }

        virtual int GetSize()
        {
            return m_size;
        }

        virtual int GetOffset()
        {
            return m_offset;
        }

        virtual void SetSize(int size)
        {
            m_size = size;
        }

        virtual void SetOffset(int offset)
        {
            m_offset = offset;
        }        

        // return the decl for the type of the thing this node represents
        virtual cDeclNode *GetType() = 0;

        // return the name of the item that is declared
        virtual string  GetName() = 0;

        // virtual functions to define what kind of decl this is.
        // subclasses should override the appropriate functions.
        virtual bool IsStruct() { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual int  Sizeof()   = 0;

        virtual string NodeType() { return string("decl"); }
        bool IsCompatibleWith(cDeclNode *decl)
        {
            if (IsFunc()) return false;
            if (this == decl) return true;
            if (IsStruct() || decl->IsStruct()) return false;
            if (IsFloat() && ! decl->IsStruct()) return true;
            if (Sizeof() >= decl->Sizeof()) return true;
            return false;
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    
        virtual string AttributesToString() 
        {   
            string attString = "";

            if (m_size > 0 || m_offset > 0)
            {
                attString = " size = \""; 
                attString += std::to_string(m_size); 
                attString += "\" offset = \"";
                attString += std::to_string(m_offset); 
                attString += "\"";
                return attString; 
            }    

            return attString;
        }
    
    protected:
        int m_size;
        int m_offset;
};
