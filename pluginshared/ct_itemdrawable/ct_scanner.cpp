/****************************************************************************

 Copyright (C) 2012-2012 Université de Sherbrooke, Québec, CANADA
                     All rights reserved.

 Contact :  richard.fournier@usherbrooke.ca
            jean-francois.cote@nrcan-rncan.gc.ca
            joris.ravaglia@gmail.com

 Developers : Joris RAVAGLIA

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/

#include "ct_scanner.h"

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#include <math.h>
#endif

CT_DEFAULT_IA_INIT(CT_Scanner)

// Initializing the draw manager
const CT_StandardScannerDrawManager CT_Scanner::CT_SCANNER_DRAW_MANAGER;

CT_Scanner::CT_Scanner(int scanID, bool clockWise) :
    CT_AbstractItemDrawableWithoutPointCloud()
{
    _scanID = scanID;
    setCenterX(0);
    setCenterY(0);
    setCenterZ(0);
    CT_ThetaPhiShootingPattern *p = new CT_ThetaPhiShootingPattern();
    p->setClockWise(clockWise);
    m_shootingPattern.reset(p);

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::CT_Scanner(const CT_OutAbstractSingularItemModel *model,
                       const CT_AbstractResult *result, int scanId, bool clockWise) :
    CT_AbstractItemDrawableWithoutPointCloud (model, result )
{
    _scanID = scanId;
    setCenterX(0);
    setCenterY(0);
    setCenterZ(0);


    CT_ThetaPhiShootingPattern *p = new CT_ThetaPhiShootingPattern();
    p->setClockWise(clockWise);
    m_shootingPattern.reset(p);

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::CT_Scanner(const CT_OutAbstractSingularItemModel *model,
                       const CT_AbstractResult *result,
                       int scanID,
                       const Eigen::Vector3d &origin,
                       const Eigen::Vector3d &zVector,
                       double hFov,
                       double vFov,
                       double hRes,
                       double vRes,
                       double initTheta,
                       double initPhi,
                       bool clockWise,
                       bool radians) :
    CT_AbstractItemDrawableWithoutPointCloud (model, result)
{
    _scanID = scanID;
    setCenterX(origin.x());
    setCenterY(origin.y());
    setCenterZ(origin.z());
    m_shootingPattern.reset(new CT_ThetaPhiShootingPattern(origin,
                                                       hFov,
                                                       vFov,
                                                       hRes,
                                                       vRes,
                                                       initTheta,
                                                       initPhi,
                                                       zVector,
                                                       clockWise,
                                                       radians));

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}



CT_Scanner::CT_Scanner(const QString &modelName, const CT_AbstractResult *result,
                       int scanId, bool clockWise) :
    CT_AbstractItemDrawableWithoutPointCloud (modelName, result)
{
    _scanID = scanId;
    setCenterX(0);
    setCenterY(0);
    setCenterZ(0);

    CT_ThetaPhiShootingPattern *p = new CT_ThetaPhiShootingPattern();
    p->setClockWise(clockWise);
    m_shootingPattern.reset(p);

    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::CT_Scanner(const QString &modelName,
                       const CT_AbstractResult *result,
                       int scanID,
                       const Eigen::Vector3d &origin,
                       const Eigen::Vector3d &zVector,
                       double hFov,
                       double vFov,
                       double hRes,
                       double vRes,
                       double initTheta,
                       double initPhi,
                       bool clockWise,
                       bool radians) :
    CT_AbstractItemDrawableWithoutPointCloud (modelName, result)
{
    _scanID = scanID;
    setCenterX(origin(0));
    setCenterY(origin(1));
    setCenterZ(origin(2));

    m_shootingPattern.reset(new CT_ThetaPhiShootingPattern(origin,
                                                       hFov,
                                                       vFov,
                                                       hRes,
                                                       vRes,
                                                       initTheta,
                                                       initPhi,
                                                       zVector,
                                                       clockWise,
                                                       radians));
    setBaseDrawManager(&CT_SCANNER_DRAW_MANAGER);
}

CT_Scanner::~CT_Scanner()
{
}

CT_ShootingPattern* CT_Scanner::getShootingPattern() const
{
    return m_shootingPattern.get();
}

void CT_Scanner::beam(int i, CT_Beam &beam) const
{
    CT_ShootingPattern *p = getShootingPattern();
    if (!p)
        return;

    const CT_Shot &shot = p->getShotAt(i);

    beam.setOrigin(shot.getOrigin());
    beam.setDirection(shot.getDirection());
}

CT_Scanner* CT_Scanner::copy(const CT_OutAbstractItemModel *model,
                             const CT_AbstractResult *result,
                             CT_ResultCopyModeList copyModeList)
{
    // TODO: copy the shooting pattern instead of the hardcoded constructor
    CT_Scanner *sc = new CT_Scanner((const CT_OutAbstractSingularItemModel *)model,
                                    result,
                                    getScanID());
    sc->setId(id());
    sc->setAlternativeDrawManager(getAlternativeDrawManager());

    return sc;
}
