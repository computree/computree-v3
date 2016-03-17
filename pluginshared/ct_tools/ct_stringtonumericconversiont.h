#ifndef CT_STRINGTONUMERICCONVERSIONT_H
#define CT_STRINGTONUMERICCONVERSIONT_H

#include <QLocale>
#include <sstream>
#include <iomanip>

template<typename T>
class CT_StringToNumericConversionT
{
public:
    static T fromString(const QString &s, bool *ok)
    {
        QLocale locale = QLocale::system();

        QString sVal = s;
        sVal.replace(locale.decimalPoint(), '.');

        std::stringstream ss(sVal.toStdString());

        T value;

        ss >> value;

        if(ok != NULL)
            (*ok) = ss.good();

        return value;
    }
};

template<>
inline bool CT_StringToNumericConversionT<bool>::fromString(const QString &s, bool *ok)
{
    Q_UNUSED(ok)

    return (s == "true");
}

#endif // CT_STRINGTONUMERICCONVERSIONT_H
