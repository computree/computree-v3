#ifndef CT_READER_PTX_H
#define CT_READER_PTX_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/ct_scanner.h"
#include "ct_itemdrawable/ct_scene.h"

#include "ctlibio/ctlibio_global.h"

/**
 * @brief Reader that can load a ptx file (*.ptx) that represent a PointCloud
 */
class CTLIBIO_EXPORT CT_Reader_PTX : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_PTX();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString GetReaderName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Set the filepath to the reader. The file will be opened to check is validity.
     */
    bool setFilePath(const QString &filepath);

    /**
     * @brief Configure the reader
     */
    bool configure();

    void setApplyTransformationMatrix(bool enable);
    bool applyTransformationMatrix() const;

    SettingsNodeGroup* getAllSettings() const;
    bool setAllSettings(const SettingsNodeGroup *settings);

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_PTX)

private:
    bool    m_applyTransformation;

    bool readHeaderValues(QTextStream &stream, int &nColumn, int &nRow, Eigen::Matrix4d &matrix, bool &hasColors) const;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

#endif // CT_READER_PTX_H
