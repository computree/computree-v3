#ifndef CT_READER_OPF_H
#define CT_READER_OPF_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_itemdrawable/abstract/ct_abstractmetric.h"
#include "ct_point.h"

#define DEF_CT_Reader_OPF_topologyOut     "ct_reader_opf_topology"

class CT_OPF_Attribute
{
public:
    CT_OPF_Attribute() {}

    CT_OPF_Attribute(const QString &name, const QString &type)
    {
        m_name = name;
        m_type = type;
    }

    QString                 m_name;
    QString                 m_type;

    bool operator ==(const CT_OPF_Attribute &o)
    {
        return (o.m_name == m_name) && (o.m_type == m_type);
    }
};

class CT_OPF_Type
{
public:
    CT_OPF_Type() {}

    CT_OPF_Type(const QString &name, const quint8 &level, const size_t &id)
    {
        m_name = name;
        m_level = level;
        m_id = id;
    }

    QString                             m_name;
    quint8                              m_level;
    size_t                              m_id;
    QHash<QString, CT_OPF_Attribute>    m_attributes;
};

class CT_OPF_Face
{
public:
    size_t   m_pointsIndex[3];
};

class CT_OPF_Mesh
{
public:
    std::vector<CT_Point>   m_points;
    std::list<CT_OPF_Face>  m_faces;

    void getBoundingBox(QVector3D &min, QVector3D &max) const;
};

class QXmlStreamReader;
class CT_TOPFNodeGroup;

class PLUGINSHAREDSHARED_EXPORT CT_Reader_OPF : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_OPF();
    ~CT_Reader_OPF();

    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;

    static CT_AbstractMetric* staticCreateMetricForType(const QString &type, const QString &value = "");

private:
    QHash<QString, CT_OPF_Type>                 m_types;
    QHash<QString, CT_OPF_Attribute>            m_attributes;
    QHash<QString, CT_OutAbstractItemModel*>    m_models;
    QHash<int, CT_OPF_Mesh*>                    m_meshes;
    QHash<int, CT_OPF_Mesh*>                    m_shapes;

    int                                         m_totalNode;

    static const QVector<QString>               TOPOLOGY_NAMES;

    void clearOtherModels();
    void clearMeshes();
    void clearShapes();

    void readMesh(QXmlStreamReader &xml);
    void readShape(QXmlStreamReader &xml);
    void readGeometry(QXmlStreamReader &xml, CT_TOPFNodeGroup *node, const QString &typeName);
    void transformAndCreateMesh(CT_OPF_Mesh *mesh, CT_TOPFNodeGroup *node, const double &dUp, const double &dDwn, const QString &typeName);

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

#endif // CT_READER_OPF_H
