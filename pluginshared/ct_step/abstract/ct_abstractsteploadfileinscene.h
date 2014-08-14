#ifndef CT_ABSTRACTSTEPLOADFILEINSCENE_H
#define CT_ABSTRACTSTEPLOADFILEINSCENE_H

#include "ct_abstractsteploadfile.h"

#include "ct_itemdrawable/ct_scene.h"

#include <QFile>

class CT_OutAbstractSingularItemModel;

/**
 * @brief Represent a step that load a file in a CT_Scene
 */
class PLUGINSHAREDSHARED_EXPORT CT_AbstractStepLoadFileInScene : public CT_AbstractStepLoadFile
{
public:
    CT_AbstractStepLoadFileInScene(CT_StepInitializeData &dataInit);

protected:
    /**
     * @brief Redefine this method if you want to add other OUTPUT models in the same group that the scene
     * @param sceneModelName : the unique name of the model who represent the scene. To not use it in your models.
     * @return Returns the list of other models you want to create in your compute method
     */
    virtual QList<CT_OutAbstractSingularItemModel*> createOtherOutResultListProtected(const QString &sceneModelName) { Q_UNUSED(sceneModelName) return QList<CT_OutAbstractSingularItemModel*>(); }


    /**
     * @brief Read the header file
     * @param f : the file to read
     * @return number of characters readed. -1 if it was an error.
     */
    virtual int readHeaderFile(QFile &f) = 0;

    /**
     * @brief Read the data file (after the header) and call the "createOutResult" method
     * @param f : the file to read
     * @param offset : number of characters already readed
     * @param little_endian : true if data is in littleEndian
     */
    virtual void readDataFile(QFile &f, int offset, bool little_endian = false);

    /**
     * @brief Create the OUPUT models (the scene)
     */
    void createOutResultModelListProtected();

    /**
     * @brief Read the file
     */
    virtual void compute();

    /**
     * @brief Create the scene and add it to the result.
     * @param pcir : index cloud of points readed
     * @param xmin : bounding box xmin
     * @param xmax : bounding box ymin
     * @param ymin : bounding box zmin
     * @param ymax : bounding box xmax
     * @param zmin : bounding box ymax
     * @param zmax : bounding box zmax
     * @param list : the list of other SingularItem that you have set in OUTPUT models and you want to add in the same group that the scene
     */
    virtual void createOutResult(CT_Scene::CT_AbstractCIR pcir, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, QList<CT_AbstractSingularItemDrawable*> list = QList<CT_AbstractSingularItemDrawable*>());
};

#endif // CT_ABSTRACTSTEPLOADFILEINSCENE_H
