#ifndef CT_MENULEVEL_H
#define CT_MENULEVEL_H

#include "ct_abstractstepplugin.h"

#include <QList>

class CT_VirtualAbstractStep;

/**
 * @brief A level in the menu of steps that contains steps and was identified
 *        by a displayable name
 */
class PLUGINSHAREDSHARED_EXPORT CT_MenuLevel : public QObject
{
    Q_OBJECT

public:

    struct CT_NotFoundedStep {
        QString stepKey;
        QString pluginName;

        bool operator ==(const CT_NotFoundedStep &other) {
            return (other.stepKey == stepKey) && (other.pluginName == pluginName);
        }

        bool operator !=(const CT_NotFoundedStep &other) {
            return (other.stepKey != stepKey) || (other.pluginName != pluginName);
        }
    };

    /**
     * @brief Create a level with a custom displayable name and add it to
     *        the parent level passed in parameter. Parent Level must not be NULL
     *        otherwise the method returns NULL.
     */
    static CT_MenuLevel* getOrCreateLevel(const QString &customDisplayableName, CT_MenuLevel *parentLevel);

    /**
     * @brief Create a level with a predefined displayable name and add it to
     *        the parent level passed in parameter. Parent Level must not be NULL
     *        otherwise the method returns NULL.
     */
    static CT_MenuLevel* getOrCreateLevel(CT_StepsMenu::LevelPredefined predefinedLevel, CT_MenuLevel *parentLevel);

    /**
     * @brief Returns the displayable name of this level
     */
    QString displayableName() const;

    /**
     * @brief Add the type of step to this level. Use this method in your plugin
     * @example level->addStep<PB_CustomStep>(this);
     */
    template<class Step>
    bool addStep(CT_AbstractStepPlugin *stepPlugin) {
        Step *s = new Step(*stepPlugin->createNewStepInitializeData(NULL));

        return addStepToCollectionOrDeleteIt(s);
    }

    bool addStep(CT_VirtualAbstractStep *step);

    void raiseStepInMenu(CT_VirtualAbstractStep *step);

    void lowerStepInMenu(CT_VirtualAbstractStep *step);

    /**
     * @brief Add a step that was not founded (use it for favorites levels)
     */
    bool addNotFoundedStep(CT_NotFoundedStep nfs);

    /**
     * @brief Returns the list of steps contained in this level
     */
    QList<CT_VirtualAbstractStep*> steps() const;

    /**
     * @brief Returns the list of steps not founded contained in this level
     */
    QList<CT_NotFoundedStep> stepsNotFounded() const;

    /**
     * @brief Returns the list of levels contained in this level
     */
    QList<CT_MenuLevel*> levels() const;

    /**
     * @brief Returns levels that contains step or sub-level that contain step or etc...
     *        If it was not at least one step recursively the level was not added to the
     *        returned list.
     */
    QList<CT_MenuLevel*> levelsWithSteps() const;

    /**
     * @brief Returns the parent level
     */
    CT_MenuLevel* parentLevel() const;

protected:
    friend class CT_StepsMenu;
    friend class CT_StepSeparator;
    friend class CT_StepCanBeAddedFirstSeparator;
    friend class CT_StepLoadFileSeparator;
    friend class CT_AbstractStepPlugin;

    CT_MenuLevel();
    CT_MenuLevel(const QString &displayableName);

    ~CT_MenuLevel();

    /**
     * @brief Check if this level has at least one step and if not test sub-level
     */
    bool isStepsNotEmptyRecursively() const;

    /**
     * @brief Add the step to the collection if it does not already exist otherwiase delete it
     */
    bool addStepToCollectionOrDeleteIt(CT_VirtualAbstractStep *step);

    /**
     * @brief Return true if the step alread exist in the collection
     */
    bool existStepInCollection(CT_VirtualAbstractStep *step);

    /**
     * @brief Add the level passed in parameter in this level
     */
    void addLevel(CT_MenuLevel *level);

    /**
     * @brief Modify the parent of this level
     */
    void setParentLevel(CT_MenuLevel *parent);

    /**
     * @brief Returns the level that have the displayable name passed in parameter and that was in this level
     */
    CT_MenuLevel* levelFromDisplayableName(const QString &name);

    /**
     * @brief Returns true if it was a sublevel of favorite level
     */
    bool isAFavoriteSubLevel() const;

private:
    typedef QList<CT_VirtualAbstractStep*>          StepCollection;
    typedef QListIterator<CT_VirtualAbstractStep*>  StepCollectionIt;

    typedef QMap<QString, CT_VirtualAbstractStep*>  StepCollectionSorted;

    typedef QList<CT_NotFoundedStep>                StepNotFoundedCollection;
    typedef QListIterator<CT_NotFoundedStep>        StepNotFoundedCollectionIt;

    typedef QList<CT_MenuLevel*>                    LevelCollection;
    typedef QListIterator<CT_MenuLevel*>            LevelCollectionIt;

    StepCollection              m_steps;
    StepCollectionSorted        m_stepsSorted;
    StepNotFoundedCollection    m_stepsNotFounded;
    LevelCollection             m_levelsPredefined;
    LevelCollection             m_levelsCustom;
    QString                     m_displayableName;
    CT_MenuLevel                *m_parent;

private slots:
    void subLevelDeleted(QObject *level);
    void stepDeleted(QObject *step);
};

#endif // CT_MENULEVEL_H
