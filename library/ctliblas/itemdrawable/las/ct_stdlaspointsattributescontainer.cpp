#include "ct_stdlaspointsattributescontainer.h"

#include <QDebug>
CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer() : CT_AbstractPointsAttributesContainer()
{
}

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                                                       const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(model,
                                                                                                                                               result)
{

}

CT_StdLASPointsAttributesContainer::CT_StdLASPointsAttributesContainer(const QString &modelName,
                                                                       const CT_AbstractResult *result) : CT_AbstractPointsAttributesContainer(modelName,
                                                                                                                                               result)
{

}

QString CT_StdLASPointsAttributesContainer::name() const
{
    return tr("LAS point attributes");
}

void CT_StdLASPointsAttributesContainer::insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att)
{
    if(att != NULL)
        internalInsertAttributes(key, att);
}

QList<CT_LasDefine::LASPointAttributesType> CT_StdLASPointsAttributesContainer::lasPointAttributesTypes() const
{
    QList<CT_LasDefine::LASPointAttributesType> l;
    QList<int> la = internalAttributes().uniqueKeys();

    foreach (int i, la) {
        l.append((CT_LasDefine::LASPointAttributesType)i);
    }

    return l;
}

void CT_StdLASPointsAttributesContainer::getLASDataAt(const size_t i, CT_LASData &data) const
{
    CT_AbstractPointsAttributes* attribute;

    if ((attribute = pointsAttributesAt(CT_LasDefine::Intensity)) != NULL) {data._Intensity = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Intensity = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Return_Number)) != NULL) {data._Return_Number = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Return_Number = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Number_of_Returns)) != NULL) {data._Number_of_Returns = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Number_of_Returns = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Classification_Flags)) != NULL) {data._Classification_Flags = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Classification_Flags = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Scanner_Channel)) != NULL) {data._Scanner_Channel = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Scanner_Channel = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Scan_Direction_Flag)) != NULL) {data._Scan_Direction_Flag = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Scan_Direction_Flag = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Edge_of_Flight_Line)) != NULL) {data._Edge_of_Flight_Line = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Edge_of_Flight_Line = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Classification)) != NULL) {data._Classification = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Classification = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Scan_Angle_Rank)) != NULL) {data._Scan_Angle_Rank = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Scan_Angle_Rank = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::User_Data)) != NULL) {data._User_Data = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._User_Data = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Point_Source_ID)) != NULL) {data._Point_Source_ID = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Point_Source_ID = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::GPS_Time)) != NULL) {data._GPS_Time = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._GPS_Time = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Red)) != NULL) {data._Red = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Red = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Green)) != NULL) {data._Green = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Green = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Blue)) != NULL) {data._Blue = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Blue = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::NIR)) != NULL) {data._NIR = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._NIR = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Wave_Packet_Descriptor_Index)) != NULL) {data._Wave_Packet_Descriptor_Index = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Wave_Packet_Descriptor_Index = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Byte_offset_to_waveform_data)) != NULL) {data._Byte_offset_to_waveform_data = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Byte_offset_to_waveform_data = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Waveform_packet_size_in_bytes)) != NULL) {data._Waveform_packet_size_in_bytes = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Waveform_packet_size_in_bytes = 0;}
    if ((attribute = pointsAttributesAt(CT_LasDefine::Return_Point_Waveform_Location)) != NULL) {data._Return_Point_Waveform_Location = ((CT_AbstractPointAttributesScalar*) attribute)->dValueAt(i);} else {data._Return_Point_Waveform_Location = 0;}
}

QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> CT_StdLASPointsAttributesContainer::lasPointsAttributes() const
{
    QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> h;
    QHash<int, CT_AbstractPointsAttributes *> ha = internalAttributes();

    QHashIterator<int, CT_AbstractPointsAttributes *> it(ha);

    while(it.hasNext()) {
        it.next();
        h.insert((CT_LasDefine::LASPointAttributesType)it.key(), (CT_AbstractPointAttributesScalar*)it.value());
    }

    return h;
}

CT_AbstractItemDrawable *CT_StdLASPointsAttributesContainer::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    return new CT_StdLASPointsAttributesContainer((CT_OutAbstractSingularItemModel*)model, result);
}
