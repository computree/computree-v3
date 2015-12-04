#ifndef CT_READER_OPF_H
#define CT_READER_OPF_H

#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_attributes/abstract/ct_abstractitemattribute.h"
#include "ct_itemdrawable/tools/drawmanager/abstract/ct_abstractitemdrawabledrawmanager.h"
#include "ct_mesh/ct_mesh.h"
#include "ct_point.h"

#include "rapidxml/rapidxml_utils.hpp"

#include "ct_reader/ct_reader_opf_def_models.h"

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

class CT_OPF_Mesh
{
public:
    CT_Mesh                 *m_mesh;

    CT_OPF_Mesh() { m_mesh = NULL; m_bboxOK = false; }
    ~CT_OPF_Mesh() { delete m_mesh; }

    void getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const;

private:
    mutable Eigen::Vector3d   m_min;
    mutable Eigen::Vector3d   m_max;
    mutable bool              m_bboxOK;
};

class QXmlStreamReader;
class CT_TOPFNodeGroup;

/**
 * @brief Read OPF File (http://amapstudio.cirad.fr/)
 */
class PLUGINSHAREDSHARED_EXPORT CT_Reader_OPF : public CT_AbstractReader
{
    Q_OBJECT

public:
    CT_Reader_OPF();
    ~CT_Reader_OPF();

    QString GetReaderName() const;
    CT_StepsMenu::LevelPredefined getReaderSubMenuName() const;

    bool setFilePath(const QString &filepath);

    CT_AbstractReader* copy() const;

    static CT_AbstractItemAttribute* staticCreateAttributeForType(const QString &type, const QString &value = "");

private:
    QHash<QString, CT_OPF_Type>                         m_types;
    QHash<QString, CT_OPF_Attribute>                    m_attributes;
    QHash<QString, CT_OutAbstractItemAttributeModel*>   m_attributesOriginalModels;
    QHash<QString, CT_OutAbstractModel*>                m_models;
    QHash<int, CT_OPF_Mesh*>                            m_meshes;
    QHash<int, CT_OPF_Mesh*>                            m_shapes;
    QList<CT_AbstractItemDrawableDrawManager*>          m_drawManager;

    int                                                 m_totalNode;

    CT_OPF_Mesh                                         m_cylinderMesh;

    static const QVector<QString>                       TOPOLOGY_NAMES;

    void clearOtherModels();
    void clearMeshes();
    void clearShapes();
    void clearDrawManagers();

    void readMesh(rapidxml::xml_node<> *xmlNode);
    void readShape(rapidxml::xml_node<> *xmlNode);
    void readGeometry(rapidxml::xml_node<> *xmlNode, CT_TOPFNodeGroup *node, const QString &typeName);
    void transformAndCreateMesh(CT_Mesh *mesh, Eigen::Vector3d &min, Eigen::Vector3d &max, CT_TOPFNodeGroup *node, const double &dUp, const double &dDwn, const QString &typeName);

    void recursiveReadTopologyForModel(rapidxml::xml_node<> *node,
                                       int &totalNode,
                                       QHash<QString,
                                       CT_OPF_Type> &types,
                                       const QHash<QString,
                                       CT_OPF_Attribute> &attributes);

    void recursiveReadTopology(rapidxml::xml_node<> *xmlNode,
                               CT_TOPFNodeGroup *node,
                               int &nNodeReaded);

protected:
    void protectedInit();
    void protectedCreateOutItemDrawableModelList();
    bool protectedReadFile();
};

#endif // CT_READER_OPF_H
