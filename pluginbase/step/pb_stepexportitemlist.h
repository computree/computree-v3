#ifndef PB_STEPEXPORTITEMLIST_H
#define PB_STEPEXPORTITEMLIST_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_exporter/abstract/ct_abstractexporter.h"

/*!
 * \class PB_StepExportItemList
 * \ingroup Steps_PB
 * \brief <b>Exporter une liste d'items à l'aide d'un exporter.</b>
 *
 * No detailled description for this step
 *
 *
 */

class PB_StepExportItemList: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepExportItemList(CT_StepInitializeData &dataInit);

    /*! \brief Step description
     * 
     * Return a description of the step function
     */
    QString getStepDescription() const;

    /*! \brief Step detailled description
     * 
     * Return a detailled description of the step function
     */
    QString getStepDetailledDescription() const;

    /*! \brief Step URL
     * 
     * Return a URL of a wiki for this step
     */
    QString getStepURL() const;

    /*! \brief Step copy
     * 
     * Step copy, used when a step is added by step contextual menu
     */
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    /*! \brief Input results specification
     * 
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected();

    /*! \brief Parameters DialogBox
     * 
     * DialogBox asking for step parameters
     */
    void createPostConfigurationDialog();

    /*! \brief Output results specification
     * 
     * Specification of output results models created by the step (OUT)
     */
    void createOutResultModelListProtected();

    /*! \brief Algorithm of the step
     * 
     * Step computation, using input results, and creating output results
     */
    void compute();

private:

    // Step parameters
    QStringList _dir;
    QString _suffixFileName;

    QMap<QString, QPair<CT_AbstractExporter*, int> >  _exportersMap;
    QList<CT_AbstractExporter*>                       _exportersInstancesList;

};

#endif // PB_STEPEXPORTITEMLIST_H
