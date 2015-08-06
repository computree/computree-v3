#ifndef PB_STEPLOADPLOTAREAS_H
#define PB_STEPLOADPLOTAREAS_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_view/tools/ct_textfileconfigurationdialog.h"
#include "ct_tools/model/ct_autorenamemodels.h"


class PB_StepLoadPlotAreas: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepLoadPlotAreas(CT_StepInitializeData &dataInit);

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

    void createOutResultModelListProtected();

    void compute();


private:

    // Step parameters
    QList<CT_TextFileConfigurationFields> _neededFields;

    QString _refFileName;
    QString _plotID;
    bool _refHeader;
    QString _refSeparator;
    QString _refDecimal;
    QLocale _refLocale;
    int _refSkip;
    QMap<QString, int> _refColumns;

    CT_AutoRenameModels _areaModelName;
};

#endif // PB_STEPLOADPLOTAREAS_H
