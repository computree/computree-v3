#ifndef TOOLS_H
#define TOOLS_H

#include "qstring.h"

class Tools
{
public:

    inline static QString getIndentation(int n)
    {
        QString str = "";
        for (int i = 0 ; i < n ; i++)
        {
            str.append("    ");
        }
        return str;
    }


    inline static QString getSpaceSequence(int n)
    {
        QString str = "";
        for (int i = 0 ; i < n ; i++)
        {
            str.append(" ");
        }
        return str;
    }


    inline static QString getBooleanText(bool b)
    {
        return (b?"true":"false");
    }  

};

#endif // TOOLS_H
