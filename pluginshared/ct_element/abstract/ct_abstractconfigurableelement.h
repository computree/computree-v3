#ifndef CT_ABSTRACTCONFIGURABLEELEMENT_H
#define CT_ABSTRACTCONFIGURABLEELEMENT_H

#include "pluginShared_global.h"
#include "ct_view/ct_stepconfigurabledialog.h"

class PLUGINSHAREDSHARED_EXPORT CT_AbstractConfigurableElement : public QObject
{
    Q_OBJECT
public:
    CT_AbstractConfigurableElement();
    virtual ~CT_AbstractConfigurableElement();

    /**
     * @brief Returns the unique name of this object (class name)
     */
    QString getUniqueName() const;

    /**
     * @brief Returns a short displayable name
     */
    virtual QString getShortDisplayableName() const;

    /**
     * @brief Returns a detailled displayable name
     */
    virtual QString getDetailledDisplayableName() const;

    /**
     * @brief Returns a short description
     */
    virtual QString getShortDescription() const = 0;

    /**
     * @brief Returns a detailled description
     */
    virtual QString getDetailledDescription() const;

    /**
     * @brief Return parameters of dialogs and other configuration to save in a script.
     *
     * @warning If you inherit this method call first the method of the superclass and add your own settings to
     *          the "SettingsNodeGroup" returned by the superclass.
     *
     *          Example :
     *
     *          SettingsNodeGroup *root = CT_AbstractConfigurableElement::getAllSettings();
     *          SettingsNodeGroup *myGroup = new SettingsNodeGroup("MyClass");
     *          myGroup->addValue(new SettingsNodeValue("MyFirstSetting", ....));
     *          root->addGroup(myGroup);
     *          return root;
     *
     * @return parameters to save
     */
    virtual SettingsNodeGroup* getAllSettings() const = 0;

    /**
     * @brief Load parameters from a script
     *
     * @warning If you inherit this method call first the method of the superclass
     *
     *          Example :
     *
     *          CT_AbstractConfigurableElement::setAllSettings(settings);
     *          SettingsNodeGroup *myGroup = settings->groupsByTagName("MyClass").first();
     *
     *          QList<SettingsNodeValue*> values = myGroup->valuesByTagName("MyFirstSetting");
     *
     *          if(values.isEmpty())
     *              return false;
     *
     *          QString mSettings = values.first()->toString();
     *
     *          return true;
     *
     * @return false if it was an error when loaded. The error message is recovered by a call to the method "getErrorMessage( getErrorCode() )"
     */
    virtual bool setAllSettings(const SettingsNodeGroup *settings) = 0;

    /**
     * @brief Returns a copy of this object
     */
    virtual CT_AbstractConfigurableElement* copy() const = 0;

    /**
     * @brief Create and return a widget that configure this object
     * @return Can be NULL if nothing must be configured
     */
    virtual CT_AbstractConfigurableWidget* createConfigurationWidget() { return NULL; }

    /**
     * @brief Called after configuration
     */
    virtual void postConfigure() = 0;

};

#endif // CT_ABSTRACTCONFIGURABLEELEMENT_H
