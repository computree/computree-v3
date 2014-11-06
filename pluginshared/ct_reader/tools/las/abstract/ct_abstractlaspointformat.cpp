#include "ct_abstractlaspointformat.h"

CT_AbstractLASPointFormat::CT_AbstractLASPointFormat()
{
    m_lasHeader = NULL;
}

CT_AbstractLASPointFormat::~CT_AbstractLASPointFormat()
{
    clearInfos();
}

void CT_AbstractLASPointFormat::setHeader(const CT_LASHeader *header)
{
    m_lasHeader = (CT_LASHeader*)header;
}

void CT_AbstractLASPointFormat::setAttributes(const QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> &attributes)
{
    m_attributes = attributes;
}

const QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> &CT_AbstractLASPointFormat::attributes() const
{
    return m_attributes;
}

bool CT_AbstractLASPointFormat::initWrite()
{
    clearInfos();

    // get type of attribute to search for point format X (from derivated class)
    QList<CT_LasDefine::LASPointAttributesType> types = typesToSearch();

    QListIterator<CT_LasDefine::LASPointAttributesType> itT(types);

    // for each type of attribute
    while(itT.hasNext())
    {
        CT_LasDefine::LASPointAttributesType type = itT.next();

        // get it in the attributes map
        CT_AbstractPointAttributesScalar *scalar = m_attributes.value(type, NULL);

        // if exist
        if(scalar != NULL)
        {
            // get the point cloud index
            const CT_AbstractPointCloudIndex *indexes = scalar->getPointCloudIndex();

            if(indexes != NULL)
            {
                size_t pIndex = 0;

                CT_AbstractPointCloudIndex::ConstIterator it = indexes->constBegin();
                CT_AbstractPointCloudIndex::ConstIterator end = indexes->constEnd();

                // for each index
                while(it != end) {

                    // get info for this global point index
                    CT_LasPointInfo *info = m_infos.value(it.cIndex(), NULL);

                    // if info doesn't already exist
                    if(info == NULL)
                    {
                        // create it
                        info = new CT_LasPointInfo();

                        // insert in the info map
                        m_infos.insert(it.cIndex(), info);
                    }

                    // and set it the attribute
                    info->setAttribute(type, scalar, pIndex);

                    ++it;
                    ++pIndex;
                }
            }
        }
    }

    return true;
}

void CT_AbstractLASPointFormat::clearInfos()
{
    qDeleteAll(m_infos.begin(), m_infos.end());
    m_infos.clear();
}
