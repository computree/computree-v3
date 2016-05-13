#include "ct_xybheader.h"


CT_DEFAULT_IA_INIT(CT_XYBHeader)

CT_XYBHeader::CT_XYBHeader() : CT_FileHeader()
{

}

CT_XYBHeader::CT_XYBHeader(const CT_OutAbstractSingularItemModel *model, const CT_AbstractResult *result) : CT_FileHeader(model, result)
{
    m_scanCenterX = 0;
    m_scanCenterY = 0;
    m_scanCenterZ = 0;
    m_nrows = 0;
    m_ncols = 0;
}

CT_XYBHeader::CT_XYBHeader(const QString &modelName, const CT_AbstractResult *result) : CT_FileHeader(modelName, result)
{
    m_scanCenterX = 0;
    m_scanCenterY = 0;
    m_scanCenterZ = 0;
    m_nrows = 0;
    m_ncols = 0;
}

CT_AbstractItemDrawable *CT_XYBHeader::copy(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result, CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);

    CT_XYBHeader *cpy =  new CT_XYBHeader((CT_OutAbstractSingularItemModel*) model, result);
    cpy->setFile(_fileInfo.filePath());
    cpy->m_scanCenterX = m_scanCenterX;
    cpy->m_scanCenterY = m_scanCenterY;
    cpy->m_scanCenterZ = m_scanCenterZ;
    cpy->m_nrows = m_nrows;
    cpy->m_ncols = m_ncols;

    return cpy;
}
