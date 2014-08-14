#ifndef CT_STRINGTONUMERICCONVERSION_H
#define CT_STRINGTONUMERICCONVERSION_H

#include <QLocale>

class CT_StringToNumericConversion
{
public:

    static double stringToDouble(const QString &s, bool *ok = NULL)
    {
        QLocale locale;
        return locale.toDouble(s, ok);
    }

    static float stringToFloat(const QString &s, bool *ok = NULL)
    {
        QLocale locale;
        return locale.toFloat(s, ok);
    }
};

#endif // CT_STRINGTONUMERICCONVERSION_H
