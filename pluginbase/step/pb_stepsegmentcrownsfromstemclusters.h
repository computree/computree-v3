#ifndef PB_STEPSEGMENTCROWNSFROMSTEMCLUSTERS_H
#define PB_STEPSEGMENTCROWNSFROMSTEMCLUSTERS_H

#ifdef USE_OPENCV
#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_itemdrawable/ct_standarditemgroup.h"

// Inclusion of auto-indexation system
#include "ct_tools/model/ct_autorenamemodels.h"


class PB_StepSegmentCrownsFromStemClusters: public CT_AbstractStep
{
    Q_OBJECT

public:

    /*! \brief Step constructor
     * 
     * Create a new instance of the step
     * 
     * \param dataInit Step parameters object
     */
    PB_StepSegmentCrownsFromStemClusters(CT_StepInitializeData &dataInit);

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

    double      _distMax;

    // Declaration of autoRenames Variables (groups or items added to In models copies)
    CT_AutoRenameModels    _outScene_ModelName;
    CT_AutoRenameModels    _outAttZmax_ModelName;


    struct PointData
    {
        PointData(size_t index, double x, double y, double z)
        {
            _index = index;
            _x = x;
            _y = y;
            _z = z;
        }

        size_t  _index;
        double  _x;
        double  _y;
        double  _z;
    };

    struct StemData
    {
        StemData(CT_StandardItemGroup* group, double distMax)
        {
            _group = group;
            _distMax1 = distMax;
            _distMax2 = distMax*distMax;
        }

        ~StemData()
        {
            qDeleteAll(_points);
        }

        int size() {return _points.size();}

        double maxZ()
        {
            if (_points.size() > 0)
            {
                return _points.last()->_z;
            } else {
                return 0.0;
            }
        }

        bool addPoint(PointData* point)
        {
            if (point == NULL) {return false;}
            if (_points.size() == 0 || point->_z < _points.first()->_z)
            {
                _points.push_front(point);
            } else {
                bool inserted = false;
                for (int i = 0 ; !inserted && i < _points.size(); i++)
                {
                    if (point->_z < _points.at(i)->_z)
                    {
                        _points.insert(i, point);
                        inserted = true;
                    }
                }
                if (!inserted) {_points.append(point);}
            }
            return true;
        }

        double getDistance(PointData* point, bool &alreadyInserted)
        {
            alreadyInserted = false;
            double zMin = point->_z - _distMax1;
            double distance = _distMax2;

            bool stop = false;
            for (int i = _points.size() - 1 ; !stop && i >= 0 ; i--)
            {
                PointData* pt = _points.at(i);
                if (point->_index == pt->_index) {alreadyInserted = true; return 0.0;}

                if (pt->_z >= zMin)
                {
                    double dist2 = pow(pt->_x - point->_x, 2) + pow(pt->_y - point->_y, 2) + pow(pt->_z - point->_z, 2);
                    if (dist2 < distance)
                    {
                        distance = dist2;
                    }
                } else {
                    stop = true;
                }
            }

            return sqrt(distance);
        }


        double                  _distMax1;
        double                  _distMax2;
        CT_StandardItemGroup*   _group;
        QList<PointData*>       _points;
    };

    static bool lessThan(PB_StepSegmentCrownsFromStemClusters::PointData *p1, PB_StepSegmentCrownsFromStemClusters::PointData *p2)
    {
        return p1->_z < p2->_z;
    }

};
#endif

#endif // PB_STEPSEGMENTCROWNSFROMSTEMCLUSTERS_H
