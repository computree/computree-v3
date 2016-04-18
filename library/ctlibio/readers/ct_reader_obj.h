#ifndef CT_READER_OBJ_H
#define CT_READER_OBJ_H

#include "ct_reader/abstract/ct_abstractreader.h"

#include "ctlibio/ctlibio_global.h"
#include "ct_reader_obj_def_models.h"

/**
 * @brief Reader that can load a obj file (*.obj) that represent a Mesh or a PointCloud
 */
class CTLIBIO_EXPORT CT_Reader_OBJ : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_OBJ();

    /**
     * @brief Returns a displayable name of the reader
     */
    QString GetReaderName() const;

    /**
     * @brief Returns the sub menu level where we can store this reader
     */
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    /**
     * @brief Set if we must load the file like a PointCloud (true) or like a Mesh (false)
     */
    void setLoadAsPointCloud(bool enable);

    /**
     * @brief Returns true if the reader load the file in a Scene (Depends if faces was found in the file when you call the method "setFilePath")
     */
    bool loadAsPointCloud() const;

    /**
     * @brief Set if we must search half edges after the mesh was loaded (this process is long)
     */
    void setSearchHalfEdges(bool enable);

    /**
     * @brief Returns true if we must search half edges
     */
    bool searchHalfEdges() const;

    /**
     * @brief Show a dialog to configure this reader
     */
    bool configure();

    SettingsNodeGroup* getAllSettings() const;
    bool setAllSettings(const SettingsNodeGroup *settings);

    CT_AbstractReader* copy() const;
    READER_COPY_FULL_IMP(CT_Reader_OBJ)

private:
    bool    m_searchHEdges;
    bool    m_loadAsPointCloud;

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();

    bool checkIsAVertex(const QString &buf) const;
    bool checkHasInfoOfVertex(const QString &buf) const;
    bool loadVertex(const QString &buf, CT_Point &point) const;

    bool checkIsAFace(const QString &buf) const;
    bool checkHasInfoOfFace(const QString &buf) const;
    bool loadFace(const QString &buf, const size_t &beginVertexIndex, CT_Mesh *mesh, CT_MutableFaceIterator &itFaces, CT_MutableEdgeIterator &itEdges) const;

    CT_Edge *findHEdgeTwin(const CT_Mesh *mesh, const size_t &p0, const size_t &p1) const;

    void updateMinMax(const CT_Point &point, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax) const;

    CT_Mesh* loadFileAsMesh(QTextStream &stream, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax);
    CT_NMPCIR loadFileAsPointCloud(QTextStream &stream, const qint64 &fileSize, Eigen::Vector3d &bboxMin, Eigen::Vector3d &bboxMax);
};

#endif // CT_READER_OBJ_H
