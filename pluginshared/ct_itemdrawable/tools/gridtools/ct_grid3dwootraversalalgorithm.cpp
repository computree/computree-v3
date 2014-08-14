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
        _end.x() > _start.x() ? _stepAxis.setX(1) : _stepAxis.setX(-1);
        _end.y() > _start.y() ? _stepAxis.setY(1) : _stepAxis.setY(-1);
        _end.z() > _start.z() ? _stepAxis.setZ(1) : _stepAxis.setZ(-1);


        size_t currentIndex;

        // The voxel pierced by the first intersection between the ray and the grid
        if (_calibrationGrid->indexAtXYZ(_start.x(), _start.y(), _start.z(), currentIndex))
        {
            _calibrationGrid->indexToGrid(currentIndex, _currentCol, _currentLin, _currentLevz);
        } else {

            qDebug() << "Error while initializing traversal algorithm : initial voxel is not part of the grid ";
            qDebug() << "The starting point of the algorithm is not part of the grid";
            qDebug() << "Starting point of the algorithm : " << _start.x() << _start.y() << _start.z();
            qDebug() << "Bottom left corner of the calibration grid : " << _gridBottom.x() << _gridBottom.y() << _gridBottom.z();
            qDebug() << "Top right corner of the calibration grid : " << _gridTop.x() << _gridTop.y() << _gridTop.z();
            exit(1);
        }

        // Wether the ray is along the direction axis or opposite, the tMax value computation will differ (the first encountered boundary may be forward or backward)
        _stepAxis.x() > 0 ?
                    _boundary.setX(((_currentCol+1)*_gridRes) + _gridBottom.x()) :
                    _boundary.setX((_currentCol*_gridRes) + _gridBottom.x());

        _stepAxis.y() > 0 ?
                    _boundary.setY(((_currentLin+1)*_gridRes) + _gridBottom.y()) :
                    _boundary.setY((_currentLin*_gridRes) + _gridBottom.y());


        _stepAxis.z() > 0 ?
                    _boundary.setZ(((_currentLevz+1)*_gridRes) + _gridBottom.z()) :
                    _boundary.setZ((_currentLevz*_gridRes) + _gridBottom.z());

        // Initializing the tMax values
        data.getDirection().x() != 0 ?
                    _tMax.setX(fabs((_boundary.x() - _start.x()) / data.getDirection().x())) :
                    _tMax.setX(std::numeric_limits<float>::max());

        data.getDirection().y() != 0 ?
                    _tMax.setY(fabs((_boundary.y() - _start.y()) / data.getDirection().y())) :
                    _tMax.setY(std::numeric_limits<float>::max());

        data.getDirection().z() != 0 ?
                    _tMax.setZ(fabs((_boundary.z() - _start.z()) / data.getDirection().z())) :
                    _tMax.setZ(std::numeric_limits<float>::max());

        // Initializing the deltaT values
        data.getDirection().x() != 0 ?
                    _tDel.setX(fabs(_gridRes / data.getDirection().x())) :
                    _tDel.setX(std::numeric_limits<float>::max());

        data.getDirection().y() != 0 ?
                    _tDel.setY(fabs(_gridRes / data.getDirection().y())) :
                    _tDel.setY(std::numeric_limits<float>::max());

        data.getDirection().z() != 0 ?
                    _tDel.setZ(fabs(_gridRes / data.getDirection().z())) :
                    _tDel.setZ(std::numeric_limits<float>::max());

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
            int bits =	(( _tMax.x() < _tMax.y() ) << 2) +
                    (( _tMax.x() < _tMax.z() ) << 1) +
                    (( _tMax.y() < _tMax.z() ));
            _nextStepAxis = _chooseAxis[bits];

            // Going to next voxel along this direction
            if      (_nextStepAxis==0) {_currentCol = _currentCol + _stepAxis.x();}
            else if (_nextStepAxis==1) {_currentLin = _currentLin + _stepAxis.y();}
            else if (_nextStepAxis==2) {_currentLevz = _currentLevz + _stepAxis.z();}

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
            if      (_nextStepAxis==0) {_tMax.setX(_tMax.x() + _tDel.x());}
            else if (_nextStepAxis==1) {_tMax.setY(_tMax.y() + _tDel.y());}
            else if (_nextStepAxis==2) {_tMax.setZ(_tMax.z() + _tDel.z());}
        }
    }
}
