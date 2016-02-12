#ifndef CT_READER_LAS_H
#define CT_READER_LAS_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ctlibio/ctlibio_global.h"
#include "headers/ct_lasheader.h"
#include "ct_reader_las_def_models.h"

/**
 * @brief Read LAS File (http://www.asprs.org/Committee-General/LASer-LAS-File-Format-Exchange-Activities.html)
 */
class CTLIBIO_EXPORT CT_Reader_LAS : public CT_AbstractReader
{
    Q_OBJECT

public:

    struct PointCore0_5
    { 
        quint8  entire; // Edge of Flight Line (1 bit) - Scan Direction Flag (1 bit) - Number of Returns (3 bits) - Return Number (3 bits)

        typedef quint8 MASK;
    };

    struct PointCore6_10
    {
        quint16 entire; // Number of Returns (4 bits) - Return Number (4 bits)
                        // Edge of Flight Line (1 bit) - Scan Direction Flag (1 bit) - Scanner Channel (2 bits) - Classification Flags (4 bits)

        typedef quint16 MASK;
    };

    CT_Reader_LAS();
    virtual ~CT_Reader_LAS();

    QString GetReaderName() const;
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    bool setFilePath(const QString &filepath);
    bool configure();

    virtual bool hasBoundingBox() {return true;}

    /**
     * @brief Read the header and return it if it was no error. Otherwise return NULL.
     *        You are responsible to delete the header returned by this method.
     */
    CT_LASHeader* readHeader(const QString &filepath, QString &error) const;

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_LAS)

protected:

    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

CT_TYPEINFO_MACRO(CT_Reader_LAS::PointCore0_5, CT_Reader_LAS::PointCore0_5)
CT_TYPEINFO_MACRO(CT_Reader_LAS::PointCore6_10, CT_Reader_LAS::PointCore6_10)

#endif // CT_READER_LAS_H
