#pragma once
//**************************************
// cSymbol.h
//
// Defines class used to represent symbols.
// Later labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//
// Edited by Jessica Maxey
//
#include <string>

using std::string;


class cSymbol
{
    public:
        // param is name of symbol
        cSymbol(string name)
        {
            m_id = ++nextId;        // get next available ID
            m_name = name;
        }

        // return name of symbol
        string GetName() { return m_name; }


        // return string representation of symbol
        string ToString()
        {
            string result("<sym id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\"";

            return result;
        }


    protected:
        static long long nextId;        // Next avail symbol ID
        long long m_id;                 // Unique ID for this symbol
        string m_name;                  // name of symbol
};
