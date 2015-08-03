#ifndef PB_FILTERBYRETURNTYPE_H
#define PB_FILTERBYRETURNTYPE_H

#include "ct_filter/abstract/ct_abstractfilter_las.h"

class PB_FilterByReturnType : public CT_AbstractFilter_LAS
{
    Q_OBJECT
public:

    enum ReturnType {
        First,
        Intermediate,
        Last,
        Only,
        All,
        ReturnType_begin = First,
        ReturnType_end = All
    };

    PB_FilterByReturnType();
    PB_FilterByReturnType(PB_FilterByReturnType::ReturnType type);

    void createConfigurationDialog();
    void updateParamtersAfterConfiguration();

    void updateName();

    QString getParametersAsString() const;
    QString getShortDescription() const;
    QString getDetailledDescription() const;
    CT_AbstractConfigurableElement* copy() const;

    void validatePoint(CT_PointIterator& pointIt, CT_LASData &LADData) const;


private:

    PB_FilterByReturnType::ReturnType   _type;
    QString                             _typeAsString;


    QString getStringForType(PB_FilterByReturnType::ReturnType returnType) const;
    PB_FilterByReturnType::ReturnType getTypeForString(QString returnTypeAsString) const;
};

#endif // PB_FILTERBYRETURNTYPE_H
