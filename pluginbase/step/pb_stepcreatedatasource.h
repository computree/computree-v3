#ifndef PB_STEPCREATEDATASOURCE_H
#define PB_STEPCREATEDATASOURCE_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"
#include "ct_reader/abstract/ct_abstractreader.h"

#include "ct_view/ct_filechoicebutton.h"


/*!
 * \class PB_StepCreateDataSource
 * \ingroup Steps_PB
 * \brief <b>Créée une source de données (liste de fichiers du même type).</b>
 *
 * Seuls les headers des fichiers sont chargés à ce stade.
 *
 * \param _readersList 
 * \param _filesList 
 *
 */

class PB_StepCreateDataSource: public CT_AbstractStepCanBeAddedFirst
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepCreateDataSource(CT_StepInitializeData &dataInit);

    ~PB_StepCreateDataSource();

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

public slots:
    void setFormat(QString formatName);


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
    QString                                         _readersListValue;
    QStringList                                     _filesList;

    QMap<QString, QPair<CT_AbstractReader*, int> >  _readersMap;
    QList<CT_AbstractReader*>                       _readersInstancesList;
    CT_FileChoiceButton*                            _fileChoiceButton;

};

#endif // PB_STEPCREATEDATASOURCE_H
