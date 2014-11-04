#ifndef CT_STDLASPOINTSATTRIBUTESCONTAINER_H
#define CT_STDLASPOINTSATTRIBUTESCONTAINER_H

#include "ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.h"

/**
 *  This class is a standard container for LAS points attributes
 */
class CT_StdLASPointsAttributesContainer : public CT_AbstractPointsAttributesContainer
{
public:

    /**
     * @brief Use this type to set the type of the LAS point attributes
     */
    enum LASPointAttributesType {
        Intensity = 0,
        Return_Number,
        Number_of_Returns,
        Classification_Flags,
        Scanner_Channel,
        Scan_Direction_Flag,
        Edge_of_Flight_Line,
        Classification,
        Scan_Angle_Rank,
        User_Data,
        Point_Source_ID,
        GPS_Time,
        Color,
        Wave_Packet_Descriptor_Index,
        Byte_offset_to_waveform_data,
        Waveform_packet_size_in_bytes,
        Return_Point_Waveform_Location,
        NIR
    };

    CT_StdLASPointsAttributesContainer();

    CT_StdLASPointsAttributesContainer(const CT_OutAbstractSingularItemModel *model,
                                       const CT_AbstractResult *result);

    CT_StdLASPointsAttributesContainer(const QString &modelName,
                                       const CT_AbstractResult *result);

    /**
      * ATTENTION : ne pas oublier de redéfinir ces deux méthodes si vous hérité de cette classe.
      */
    QString getType() const;
    static QString staticGetType();

    /**
     * @brief Insert the CT_AbstractPointsAttributes at key 'key'. If an attribute already exist it will be replaced.
     *        The attribute is not deleted from memory, you are the owner.
     */
    void insertPointsAttributesAt(LASPointAttributesType key, const CT_AbstractPointsAttributes *att);

    CT_AbstractItemDrawable* copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList);
};

#endif // CT_STDLASPOINTSATTRIBUTESCONTAINER_H
