#include "cOpNode.h"
#include "cSymbolTable.h"
#include "astnodes.h"
#include "langparse.h"

string cOpNode::AttributesToString()
{
    switch(m_op)
    {
        case EQUALS:
            return string(" value='") + "=='";
        case CARROTS:
            return string(" value='") + "^^'";
        case CARROT:
            return string(" value='") + "^'";
        case PLUS:
            return string(" value='") + "+'";
        case MINUS:
            return string(" value='") + "-'";
        case MULT:
            return string(" value='") + "*'";
        case DIV:
            return string(" value='") + "/'";
        case MOD:
            return string(" value='") + "%'";
        default:
            return string(" value='") + (char)m_op + "'";
    }
}
