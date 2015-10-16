#ifndef PB_STEPLOADOBJFILE_H
#define PB_STEPLOADOBJFILE_H

#include "ct_step/abstract/ct_abstractsteploadfile.h"

/*!
 * \class PB_StepLoadObjFile
 * \ingroup Steps_PB
 * \brief <b>Load OBJ File .obj</b>
 *
 * <b>Output Models:</b>
 *
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_MeshModel \n
 *
 */
class PB_StepLoadObjFile : public CT_AbstractStepLoadFile
{
    Q_OBJECT

public:
    PB_StepLoadObjFile(CT_StepInitializeData &data);

    QString getStepDescription() const;

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);
    QList<FileFormat> getFileExtensionAccepted() const;

protected:

    void createPreConfigurationDialog();
    void createInResultModelListProtected();
    void createOutResultModelListProtected();

    void compute();

private:
    bool    m_searchHEdges;
};

#endif // PB_STEPLOADOBJFILE_H
