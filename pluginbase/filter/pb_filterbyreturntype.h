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
    PB_FilterByReturnType(const PB_FilterByReturnType* other);

    QString getName();
    QString getCompleteName();

    void createConfigurationDialog();
    void updateParamtersAfterConfiguration();

    QString getShortDescription() const;
    QString getDetailledDescription() const;

    QString getParametersAsString() const;
    virtual bool setParametersFromString(QString parameters);

    CT_AbstractConfigurableElement* copy() const;

    void validatePoint(CT_PointIterator& pointIt, CT_LASData &LADData) const;


private:

    PB_FilterByReturnType::ReturnType   _type;
    QString                             _typeAsString;

    QSet<quint8>   _classifToKeep;
    bool        _filterByClassif;
    bool        _keepVegetation;
    bool        _keepGround;
    bool        _keepNotClassified;
    bool        _keepBuldings;
    bool        _keepWater;
    QString     _keepValues;


    QString getStringForType(PB_FilterByReturnType::ReturnType returnType) const;
    PB_FilterByReturnType::ReturnType getTypeForString(QString returnTypeAsString) const;
};

#endif // PB_FILTERBYRETURNTYPE_H
