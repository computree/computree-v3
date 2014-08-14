#ifndef DM_ITEMDRAWABLETYPEBUILDER_H
#define DM_ITEMDRAWABLETYPEBUILDER_H

#include "interfaces.h"

#include <QMutex>
#include <QSet>

class DM_ItemDrawableTypeBackup
{
public:
    QMultiHash<ItemDrawable*, int>  m_itemsWithLevel;

    void add(const ItemDrawable &item, const int &level);
    void remove(const ItemDrawable &item, const int &level);
    bool isEmpty() const;
    int size() const;
};

class DM_ItemDrawableType
{
public:
    DM_ItemDrawableType() { m_backup = NULL; }
    DM_ItemDrawableType(const QString &type);
    DM_ItemDrawableType(const QString &type, const DM_ItemDrawableTypeBackup *backup);

    const QString& type() const;
    QList<ItemDrawable*> itemDrawableCollection() const;
    bool isEmpty() const;

private:
    QString                     m_type;
    DM_ItemDrawableTypeBackup    *m_backup;
};

Q_DECLARE_METATYPE(DM_ItemDrawableType)

/**
 * @brief Class tools to create a list of differents types of ItemDrawable [item.getType()] added to the builder.
 *        A signal [listChanged] is emitted when the list changed.
 */
class DM_ItemDrawableTypeBuilder : public QObject
{
    Q_OBJECT

public:
    DM_ItemDrawableTypeBuilder();
    ~DM_ItemDrawableTypeBuilder();

    bool tryLock();
    void lock();
    void unlock();

    /**
     * @brief Set how many levels the builder must go to get types (default : 1)
     */
    void setNLevel(const int &nLevel);

    /**
     * @brief Return true it a type with the name 'name' exist
     */
    bool existTypeWithName(const QString &name) const;

    /**
     * @brief Return the type with the name 'name'
     */
    DM_ItemDrawableType getTypeWithName(const QString &name) const;

    /**
     * @brief Return the differents types added to the builder
     */
    QList<DM_ItemDrawableType> types() const;

    /**
     * @brief Return true if types is empty
     */
    bool isEmpty() const;

    /**
     * @brief Return the text used in the builder to create the map
     */
    inline QString getTypeForItemDrawable(const ItemDrawable &item) const
    {
        return item.getType();
    }

public slots:

    void addItemDrawable(const ItemDrawable &item);
    void removeItemDrawable(const ItemDrawable &item);

private:
    QHash<QString, DM_ItemDrawableTypeBackup*>  m_types;
    QMutex                                      *m_mutex;
    int                                         m_nLevel;

    void recursiveAddItemDrawable(const ItemDrawable &item, const int &level);
    void recursiveRemoveItemDrawable(const ItemDrawable &item, const int &level);

signals:
    void listChanged();
    void added(DM_ItemDrawableType type);
    void removed(DM_ItemDrawableType type);
};

#endif // DM_ITEMDRAWABLETYPEBUILDER_H
