#ifndef DM_ELEMENTINFOMANAGER_H
#define DM_ELEMENTINFOMANAGER_H

#include "ct_global/ct_repository.h"
#include "ct_cloud/registered/ct_stdcloudregisteredt.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"

/**
 * Class that manage selection or visibility, etc... of Points/Faces or Edges in 3d graphics view. This
 * class create an object that will be synchronized with the size of the global points/faces or edges cloud (depend
 * of the parameter CT_Repository::SyncCloudWith).
 *
 * You can add 8 information because a GLubyte is a 8bit variable (in x32 and x64 platform).
 *
 * Use this class with a shader !
 */
class DM_ElementInfoManager
{
public:
    DM_ElementInfoManager(CT_Repository::SyncCloudWith syncWith);
    virtual ~DM_ElementInfoManager() {}

    /**
     * @brief Call this method when you add an index to the selection
     */
    void addIDToSelection(const size_t &id);

    /**
     * @brief Call this method when you want to add multiple index to the selection
     */
    void addMultipleIDToSelection(const std::vector<size_t> &idList);

    /**
     * @brief Call this method when you want to add a list of index to the selection
     */
    void addCloudIndexToSelection(const QList<CT_AbstractCloudIndex*> &listID);

    /**
     * @brief Call this method when you remove an index from the selection
     */
    void removeIDFromSelection(const size_t &id);

    /**
     * @brief Call this method when you want to remove multiple index from the selection
     */
    void removeMultipleIDFromSelection(const std::vector<size_t> &idList);

    /**
     * @brief Call this method when you want to remove a list of index from the selection
     */
    void removeCloudIndexFromSelection(const QList<CT_AbstractCloudIndex*> &listID);

    /**
     * @brief Call this method when you want to set an element invisible
     */
    void addIDToInvisibility(const size_t &id);

    /**
     * @brief Call this method when you want to set an element visible
     */
    void removeIDFromInvisibility(const size_t &id);

    /**
     * @brief Call this method when you want to clear selection
     */
    void clearSelection();

    /**
     * @brief Set all selected elements invisible
     */
    void setAllSelectedInvisible();

    /**
     * @brief Set all elements visible
     */
    void setAllElementVisible();

    /**
     * @brief Returns the information cloud that you can use with a shader
     */
    CT_StandardCloudStdVectorT<GLubyte>* informations() const;

    /**
     * @brief Returns an GLubyte to check if the element in the information cloud is selected or not
     * @example GLubyte s = checkSelected();
     *          if(info.constTAt(i) & s) {
     *              // element is selected
     *          }
     */
    GLubyte checkSelected() const;

    /**
     * @brief Returns an GLubyte to check if the element in the information cloud is visible or not
     * @example GLubyte v = checkInvisible();
     *          if(info.constTAt(i) & v) {
     *              // element 'i' is invisible
     *          }
     */
    GLubyte checkInvisible() const;

    inline GLubyte enableSelection() const { return m_enableSelection; }
    inline GLubyte disableSelection() const { return m_disableSelection; }
    inline GLubyte enableInvisibility() const { return m_enableInvisibility; }
    inline GLubyte disableInvisibility() const { return m_disableInvisibility; }

    inline bool inlineIsSelected(const size_t &id) { return (m_infoCloud->constTAt(id) & m_enableSelection); }
    inline bool inlineIsInvisible(const size_t &id) { return (m_infoCloud->constTAt(id) & m_enableInvisibility); }

    /**
     * @brief Returns the index cloud of selected TYPE
     */
    CT_CIR selected();

private:
    QSharedPointer< CT_StdCloudRegisteredT< CT_StandardCloudStdVectorT<GLubyte> > >            m_infoR;
    CT_StandardCloudStdVectorT<GLubyte>                         *m_infoCloud;

    CT_Repository::SyncCloudWith                                m_syncWith;

    GLubyte                                                     m_enableSelection;
    GLubyte                                                     m_disableSelection;
    GLubyte                                                     m_enableInvisibility;
    GLubyte                                                     m_disableInvisibility;

    bool                                                        m_selectionChanged;

    CT_CIR                                                      m_selectedBackup;

    template<typename T>
    CT_AbstractModifiableCloudIndexT<T>* selectedT() const;
};

#endif // DM_ELEMENTINFOMANAGER_H
