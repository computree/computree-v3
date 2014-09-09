#ifndef CT_ABSTRACTEXPORTER_P_H
#define CT_ABSTRACTEXPORTER_P_H

#include "interfaces.h"

class CT_AbstractCloudIndex;

class CT_AbstractExporterPrivate
{
public:
    CT_AbstractExporterPrivate() { _filepath = "."; _progress = 0; m_dm = NULL; m_myd = NULL; m_myStep = NULL; _eItems = true; _ePoints = false; _eFaces = false; _eEdges = false; _eOnlyGroup = false; }
    ~CT_AbstractExporterPrivate() {}

    QList<FileFormat>                   _formats;
    QString                             _errMsg;
    QList<CT_AbstractItemDrawable*>     _items;
    QList<CT_AbstractCloudIndex*>       _points;
    QList<CT_AbstractCloudIndex*>       _faces;
    QList<CT_AbstractCloudIndex*>       _edges;
    QString                             _filepath;
    int                                 _progress;
    DocumentManagerInterface            *m_dm;
    DocumentInterface                   *m_myd;
    CT_VirtualAbstractStep              *m_myStep;
    bool                                _eItems;
    bool                                _ePoints;
    bool                                _eFaces;
    bool                                _eEdges;
    bool                                _eOnlyGroup;
};

#endif // CT_ABSTRACTEXPORTER_P_H
