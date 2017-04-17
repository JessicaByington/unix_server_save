#pragma once
//*******************************************
//
//
//
//
//
// Author: Jessica Maxey
//
// Date: March 1, 2016

#include "cVisitor.h"
#include "emit.h"

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(string filename) : cVisitor()
        {
            InitOutput(filename.c_str());
        }

        ~cCodeGen()
        {
            FinalizeOutput();
        }

        void VisitAllNodes(cAstNode *node) { node->Visit(this); }

        virtual void Visit(cIntExprNode *node)
        {
            EmitInt(node->GetValue()); 
        }

        virtual void Visit(cFloatExprNode *node)
        {
            EmitFloat(node->GetValue()); 
        }

        virtual void Visit(cPrintNode *node)
        {
            if(!node->GetExpr()->GetType()->IsFloat())
            {
                EmitString("Temp = ");
                node->GetExpr()->Visit(this);
                EmitString(";\n");
                EmitPrintTemp();
            }
            else if(node->GetExpr()->GetType()->IsFloat())
            {
                EmitString("Temp_F = ");
                node->GetExpr()->Visit(this);
                EmitString(";\n");
                EmitPrintTemp_F();
            }
        }

        virtual void Visit(cIfNode *node)
        {
            string label1 = "";
            string label2 = "";

            EmitString("if (");
            node->GetCond()->Visit(this);
            EmitString("== 0)");

            EmitString(" goto ");
            label1 = GenerateLabel();
            EmitString(label1);
            EmitString(";\n");

            node->GetIfStmt()->Visit(this);
            EmitString("goto ");
            label2 = GenerateLabel();
            EmitString(label2);
            EmitString(";\n");

            EmitString(label1);
            EmitString(":\n");
            if(node->GetElseStmt() != nullptr)
            {
                node->GetElseStmt()->Visit(this);
                EmitString("\n");
            }
            
            EmitString(label2);
            EmitString(":\n");
        }

        virtual void Visit(cWhileNode *node)
        {
            string label1 = "";
            string label2 = "";

            label1 = GenerateLabel();
            EmitString(label1);
            EmitString(":\n");
            EmitString("if (");
            node->GetCond()->Visit(this);
            EmitString("== 1)");

            EmitString(" goto ");
            label2 = GenerateLabel();
            EmitString(label2);
            EmitString(";\n");

            node->GetStmt()->Visit(this);
            EmitString("goto ");
            EmitString(label1);
            EmitString(";\n");
            
            EmitString(label2);
            EmitString(":\n");
        } 
        
        virtual void Visit(cAssignNode *node)
        {
            if(!node->GetExpr()->IsFunc())
            {
                node->GetLval()->Visit(this);
                EmitString(" = ");
                node->GetExpr()->Visit(this);
                EmitString(";\n");
            }
            else
            {
                node->GetExpr()->Visit(this);
             
                if(node->GetLval()->GetSize() != 8)
                {
                    node->GetLval()->Visit(this);
                    EmitString(" = Temp;\n"); 
                }
                else
                {
                    node->GetLval()->Visit(this);
                    EmitString(" = Temp_F;\n");
                }


            }
            
        }

        virtual void Visit(cVarExprNode *node)
        {
            int offset = node->GetOffset();
            int size = node->GetSize();

            if(size == 8)
            {
                EmitFloatRef(offset);
            }
            else if(size == 4)
            {
                EmitIntRef(offset);
            }
            else
            {
                EmitCharRef(offset);
            }
            
        }

        virtual void Visit(cBinaryExprNode *node)
        {
            node->GetLeft()->Visit(this);
            node->GetOp()->Visit(this);
            node->GetRight()->Visit(this);
        }

        virtual void Visit(cOpNode *node)
        {
            EmitString(node->GetOpAsString());
        }

        virtual void Visit(cFuncDeclNode *node)
        {
            m_Func_Rtn_Label = GenerateLabel();

            string func_name = node->GetName();
            
            StartFunctionOutput(func_name);
    
            if(node->GetLocals() != nullptr)
            {
                EmitString("Stack_Pointer += ");
                EmitInt(node->GetLocals()->GetSize());
                EmitString(";\n");
            }
            
            
            node->GetStmts()->Visit(this);

            EmitString(m_Func_Rtn_Label);
            EmitString(":\n");
           
            if(node->GetLocals() != nullptr)
            {
                EmitString("Stack_Pointer -= ");
                EmitInt(node->GetLocals()->GetSize());
                EmitString(";\n");
            }
       
            EndFunctionOutput();
        }

        virtual void Visit(cParamListNode *node)
        {
            for (int i = 0; i < node->NumChildren(); i++)
            {
                cExprNode * param = node->GetParam(i);
                if(param->GetType()->IsFloat())
                {
                    EmitString("*(double *)&Memory[Stack_Pointer] = ");
                }
                else
                {
                    EmitString("*(int *)&Memory[Stack_Pointer] = ");
                }

                param->Visit(this);
                EmitString(";\n");
                EmitString("Stack_Pointer += " + std::to_string(param->GetType()->GetSize()) + ";\n");
            }

        }

        virtual void Visit(cReturnNode *node)
        {
            if(node->GetExpr()->GetType()->IsFloat())
            {
                EmitString("Temp_F = ");
                node->GetExpr()->Visit(this);            
                EmitString(";\n");
            }
            else
            {
                EmitString("Temp = ");
                node->GetExpr()->Visit(this);            
                EmitString(";\n");
            }
            
            EmitString("goto ");
            EmitString(m_Func_Rtn_Label);
            EmitString(";\n");
        }


        virtual void Visit(cFuncExprNode *node)
        {
            EmitString("*(int *)&Memory[Stack_Pointer] = Frame_Pointer;\n");
            EmitString("Stack_Pointer += 4;\n");
            EmitString("Temp = Stack_Pointer;\n");
            

            if(node->GetParams() != nullptr)
            {
                node->GetParams()->Visit(this);
            }

            EmitString("Frame_Pointer = Temp;\n");
            
            EmitString(node->GetName());
            EmitString("();\n");

            if(node->GetParams() != nullptr)
            {
                for (int i = 0; i < node->GetParams()->NumChildren(); i++)
                {    
                    EmitString("Stack_Pointer -= " + std::to_string(node->GetType()->GetSize()));
                    EmitString(";\n");
                }
            }

            EmitString("Stack_Pointer -= 4;\n");
            EmitString("Frame_Pointer = *(int *)&Memory[Stack_Pointer];\n");
        }

        virtual void Visit(cProgramNode *node)
        {
            EmitString("Stack_Pointer += ");
            EmitInt(node->GetBlock()->GetSize());
            EmitString(";\n");
        
            node->GetBlock()->Visit(this);

            EmitString("Stack_Pointer -= ");
            EmitInt(node->GetBlock()->GetSize());
            EmitString(";\n");
        }

    protected:
        string m_Func_Rtn_Label;

        void EmitStackRef()
        {}

        void EmitStackRef_F()
        {}

        void EmitCharRef(int offset)
        {
            EmitString("Memory[Frame_Pointer +");
            EmitInt(offset);
            EmitString("]");
        }

        void EmitIntRef(int offset)
        {
            EmitString("*(int *)&Memory[Frame_Pointer +");
            EmitInt(offset);
            EmitString("]");
        }

        void EmitFloatRef(int offset)
        {
            EmitString("*(double *)&Memory[Frame_Pointer +");
            EmitInt(offset);
            EmitString("]");
        }

};
