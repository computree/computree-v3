#include "pb_filterbyreturntype.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_iterator/ct_pointiterator.h"

PB_FilterByReturnType::PB_FilterByReturnType() : CT_AbstractFilter_LAS()
{
    _type = PB_FilterByReturnType::All;
    _typeAsString = "All";
    _outCloud = new CT_PointCloudIndexVector();
}

PB_FilterByReturnType::PB_FilterByReturnType(const PB_FilterByReturnType *other) : CT_AbstractFilter_LAS(other)
{
    _type = other->_type;
    _typeAsString = getStringForType(_type);
    _outCloud = new CT_PointCloudIndexVector();
}

QString PB_FilterByReturnType::getName()
{
    return QString("PBF_ReturnType");
}

QString PB_FilterByReturnType::getCompleteName()
{
    return getStringForType(_type);
}

void PB_FilterByReturnType::createConfigurationDialog()
{
    CT_StepConfigurableDialog* configDialog = addConfigurationDialog();

    QStringList typesList;
    for (int type = PB_FilterByReturnType::ReturnType_begin ; type <= PB_FilterByReturnType::ReturnType_end ; type++)
    {
        typesList.append(getStringForType((PB_FilterByReturnType::ReturnType) type));
    }

    _typeAsString = getStringForType(_type);

    configDialog->addStringChoice(tr("Type de retours à conserver"), "", typesList, _typeAsString);  
}

void PB_FilterByReturnType::updateParamtersAfterConfiguration()
{
    _type = getTypeForString(_typeAsString);
}

QString PB_FilterByReturnType::getParametersAsString() const
{
    QString result = "";
    result.append(getStringForType(_type));

    return result;
}

bool PB_FilterByReturnType::setParametersFromString(QString parameters)
{
    _type = getTypeForString(parameters);
    return true;
}

QString PB_FilterByReturnType::getShortDescription() const
{
    return tr("Filter by return type");
}

QString PB_FilterByReturnType::getDetailledDescription() const
{
    return tr("Filter by return type. You can specify following types:\n"
              "- First: first returns\n"
              "- Last: last returns\n"
              "- Intermediare: Returns which are not first neither last\n"
              "- Only: first returns if no other return for the ray"
              "- All : don't filter on return type");
}

CT_AbstractConfigurableElement *PB_FilterByReturnType::copy() const
{
    return new PB_FilterByReturnType(this);
}

void PB_FilterByReturnType::validatePoint(CT_PointIterator &pointIt, CT_LASData &lasData) const
{
    // Test for the type of return
    if (_type == PB_FilterByReturnType::First && lasData._Return_Number != 1) {return;}
    if (_type == PB_FilterByReturnType::Last && lasData._Return_Number != lasData._Number_of_Returns) {return;}
    if (_type == PB_FilterByReturnType::Intermediate && (lasData._Return_Number == 1 || lasData._Return_Number == lasData._Number_of_Returns)) {return;}
    if (_type == PB_FilterByReturnType::Only && (lasData._Return_Number != 1 || lasData._Number_of_Returns != 1)) {return;}

    _outCloud->addIndex(pointIt.currentGlobalIndex());
}

QString PB_FilterByReturnType::getStringForType(PB_FilterByReturnType::ReturnType returnType) const
{
    switch (returnType)
    {
        case PB_FilterByReturnType::First : return "first";
        case PB_FilterByReturnType::Last : return "last";
        case PB_FilterByReturnType::Intermediate : return "int";
        case PB_FilterByReturnType::Only : return "only";
    }

    return "all";
}

PB_FilterByReturnType::ReturnType PB_FilterByReturnType::getTypeForString(QString returnTypeAsString) const
{
    if (returnTypeAsString == "first") {return PB_FilterByReturnType::First;}
    if (returnTypeAsString == "last") {return PB_FilterByReturnType::Last;}
    if (returnTypeAsString == "int") {return PB_FilterByReturnType::Intermediate;}
    if (returnTypeAsString == "only") {return PB_FilterByReturnType::Only;}

    return PB_FilterByReturnType::All;
}
