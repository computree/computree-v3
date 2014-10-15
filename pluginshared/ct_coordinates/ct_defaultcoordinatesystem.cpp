#include "ct_defaultcoordinatesystem.h"

#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"
#include "ct_coordinates/view/ct_gdefaultcoordinatesystem.h"

CT_DefaultCoordinateSystem::CT_DefaultCoordinateSystem()
{
    m_xOffset = 0;
    m_yOffset = 0;
    m_zOffset = 0;
}

bool CT_DefaultCoordinateSystem::canConvertImport(const CT_AbstractCoordinateSystem::realEx &x,
                                                  const CT_AbstractCoordinateSystem::realEx &y,
                                                  const CT_AbstractCoordinateSystem::realEx &z) const
{
    CT_AbstractCoordinateSystem::realEx max = std::numeric_limits<CT_AbstractCoordinateSystem::realIm>::max();
    CT_AbstractCoordinateSystem::realEx min = -max;

    CT_AbstractCoordinateSystem::realEx tmp = x-m_xOffset;

    if((tmp > max) || (tmp < min))
        return false;

    tmp = y-m_yOffset;

    if((tmp > max) || (tmp < min))
        return false;

    tmp = z-m_zOffset;

    return ((tmp > max) || (tmp < min));
}

void CT_DefaultCoordinateSystem::convertImport(const CT_AbstractCoordinateSystem::realEx &x,
                                               const CT_AbstractCoordinateSystem::realEx &y,
                                               const CT_AbstractCoordinateSystem::realEx &z,
                                               CT_AbstractCoordinateSystem::realIm &xOut,
                                               CT_AbstractCoordinateSystem::realIm &yOut,
                                               CT_AbstractCoordinateSystem::realIm &zOut) const
{
    xOut = x-m_xOffset;
    yOut = y-m_xOffset;
    zOut = z-m_xOffset;

    informThatUsed();
}

void CT_DefaultCoordinateSystem::convertExport(const CT_AbstractCoordinateSystem::realIm &x,
                                               const CT_AbstractCoordinateSystem::realIm &y,
                                               const CT_AbstractCoordinateSystem::realIm &z,
                                               CT_AbstractCoordinateSystem::realEx &xOut,
                                               CT_AbstractCoordinateSystem::realEx &yOut,
                                               CT_AbstractCoordinateSystem::realEx &zOut) const
{
    xOut = x+m_xOffset;
    yOut = y+m_xOffset;
    zOut = z+m_xOffset;

    informThatUsed();
}

bool CT_DefaultCoordinateSystem::configure()
{
    CT_GDefaultCoordinateSystem dialog;
    dialog.setCoordinateSystem(this);
    dialog.setReadOnly(!CT_CoordinateSystemManager::staticCanChangeOrModifyCoordinateSystem());

    return (dialog.exec() == QDialog::Accepted);
}

void CT_DefaultCoordinateSystem::setOffset(CT_AbstractCoordinateSystem::realEx x,
                                           CT_AbstractCoordinateSystem::realEx y,
                                           realEx z)
{
    m_xOffset = x;
    m_yOffset = y;
    m_zOffset = z;
}
