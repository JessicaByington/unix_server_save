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
        default:
            return string(" value='") + (char)m_op + "'";
    }
}
