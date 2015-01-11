#include "ct_grid3dwootraversalalgorithm.h"
#include "math.h"
#include <QDebug>
#include <limits>

CT_Grid3DWooTraversalAlgorithm::CT_Grid3DWooTraversalAlgorithm()
{

}

CT_Grid3DWooTraversalAlgorithm::CT_Grid3DWooTraversalAlgorithm(const CT_AbstractGrid3D *grid, bool keepFirst, QList<CT_AbstractGrid3DBeamVisitor *> &list)
{
    _visitorList = list;
    _numberOfVisitors = list.size();
    _calibrationGrid = grid;
    _calibrationGrid->getMinCoordinates(_gridBottom);
    _calibrationGrid->getMaxCoordinates(_gridTop);
    _gridRes = _calibrationGrid->resolution();
    _keepFirst = keepFirst;

    _chooseAxis[0] = 2;
    _chooseAxis[1] = 1;
    _chooseAxis[2] = 2;
    _chooseAxis[3] = 1;
    _chooseAxis[4] = 2;
    _chooseAxis[5] = 2;
    _chooseAxis[6] = 0;
    _chooseAxis[7] = 0;
}

CT_Grid3DWooTraversalAlgorithm::~CT_Grid3DWooTraversalAlgorithm()
{
}

void CT_Grid3DWooTraversalAlgorithm::compute(CT_Beam &data)
{
    _intersects = data.intersect(_gridBottom, _gridTop, _start, _end);

    if ( _intersects )
    {
        // //////////////////////////
        // Beam initialisation phase
        // //////////////////////////

        // Direction of the ray along each axis
        for (int i = 0 ; i < 3 ; i++) {_end(i) > _start(i) ? _stepAxis(i) = 1 : _stepAxis(i) = -1;}

        size_t currentIndex;

        // The voxel pierced by the first intersection between the ray and the grid
        if (_calibrationGrid->indexAtXYZ(_start(0), _start(1), _start(2), currentIndex))
        {
            _calibrationGrid->indexToGrid(currentIndex, _currentCol, _currentLin, _currentLevz);
        } else {

            qDebug() << "Error while initializing traversal algorithm : initial voxel is not part of the grid ";
            qDebug() << "The starting point of the algorithm is not part of the grid";
            qDebug() << "Starting point of the algorithm : " << _start(0) << _start(1) << _start(2);
            qDebug() << "Bottom left corner of the calibration grid : " << _gridBottom(0) << _gridBottom(1) << _gridBottom(2);
            qDebug() << "Top right corner of the calibration grid : " << _gridTop(0) << _gridTop(1) << _gridTop(2);
            exit(1);
        }

        // Wether the ray is along the direction axis or opposite, the tMax value computation will differ (the first encountered boundary may be forward or backward)

        for (int i = 0 ; i < 3 ; i++) {
            _stepAxis(i) > 0 ?
                    _boundary(i) = ((_currentCol+1)*_gridRes) + _gridBottom(i) :
                    _boundary(i) = (_currentCol*_gridRes) + _gridBottom(i);
        }

        // Initializing the tMax values                
        for (int i = 0 ; i < 3 ; i++) {
            data.getDirection()(i) != 0 ?
                        _tMax(i) = fabs((_boundary(i) - _start(i)) / data.getDirection()(i)) :
                        _tMax(i) = std::numeric_limits<double>::max();
        }

        // Initializing the deltaT values
        for (int i = 0 ; i < 3 ; i++) {
            data.getDirection()(i) != 0 ?
                        _tDel(i) = fabs(_gridRes / data.getDirection()(i)) :
                        _tDel(i) = std::numeric_limits<double>::max();
        }

        if (_keepFirst)
        {
            size_t index;

            if (_calibrationGrid->index(_currentCol, _currentLin, _currentLevz, index))
            {
                for (int i = 0 ; i < _numberOfVisitors ; ++i)
                {
                    _visitorList.at(i)->visit(index, &data);
                }
            }
        }

        while ( 1 )
        {
            // Finds along which axis to do the next step
            int bits =	(( _tMax(0) < _tMax(1) ) << 2) +
                        (( _tMax(0) < _tMax(2) ) << 1) +
                        (( _tMax(1) < _tMax(2) ));
            _nextStepAxis = _chooseAxis[bits];

            // Going to next voxel along this direction
            if      (_nextStepAxis==0) {_currentCol = _currentCol + _stepAxis(0);}
            else if (_nextStepAxis==1) {_currentLin = _currentLin + _stepAxis(1);}
            else if (_nextStepAxis==2) {_currentLevz = _currentLevz + _stepAxis(2);}

            // Checks if the currentvoxel is outside the grid, the algorithm has finished
            if (_currentCol >= _calibrationGrid->xdim()) {return;}
            if (_currentLin >= _calibrationGrid->ydim()) {return;}
            if (_currentLevz >= _calibrationGrid->zdim()) {return;}

            // Add the index of the voxel to the list

            size_t index;

            if (_calibrationGrid->index(_currentCol, _currentLin, _currentLevz, index))
            {
                // Add the index of the voxel to the list
                for (int i = 0 ; i < _numberOfVisitors ; ++i)
                {
                    _visitorList.at(i)->visit(index, &data);
                }
            }

            // Updating tmax of this axis (increasing by deltaT)
            if      (_nextStepAxis == 0) {_tMax(0) = _tMax(0) + _tDel(0);}
            else if (_nextStepAxis == 1) {_tMax(1) = _tMax(1) + _tDel(1);}
            else if (_nextStepAxis == 2) {_tMax(2) = _tMax(2) + _tDel(2);}
        }
    }
}
