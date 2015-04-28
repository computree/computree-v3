#ifndef PB_STEPSELECTCELLSINGRID3DBYBINARYPATTERN_H
#define PB_STEPSELECTCELLSINGRID3DBYBINARYPATTERN_H

#include "ct_step/abstract/ct_abstractstep.h"

/*!
 * \class PB_StepSelectCellsInGrid3DByBinaryPattern
 * \ingroup Steps_PB
 * \brief <b>Créée une grille booléenne de séléction à partir d'un motif binaire.</b>
 *
 * No detailled description for this step
 *
 * \param _inThreshold 
 * \param _pattern 
 * \param _trash 
 * \param _outThreshold 
 *
 */

class PB_StepSelectCellsInGrid3DByBinaryPattern: public CT_AbstractStep
{
    Q_OBJECT

    struct PatternCell
    {
        PatternCell(int r, int c, int lev, double v)
        {
            _rowRel = r;
            _colRel = c;
            _levRel = lev;
            _val = v;
        }

        int     _rowRel;
        int     _colRel;
        int     _levRel;
        double  _val;
    };

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepSelectCellsInGrid3DByBinaryPattern(CT_StepInitializeData &dataInit);

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
    double      _inThreshold;
    QString     _pattern;
    int         _outThresholdAbsolute;
    double      _outThresholdRelative;
    int         _selectMode;

    bool parsePattern(QList<PatternCell> &parsedPattern);
};

#endif // PB_STEPSELECTCELLSINGRID3DBYBINARYPATTERN_H
