#ifndef INTERFACESSETTINGS_H
#define INTERFACESSETTINGS_H

#include <QVariant>

/**
  * \brief Classe représentant une valeur d'un paramètre
  *
  * On pourra écrire une valeur dans un fichier sous le format suivant (par exemple) :
  *
  *     <NAME value="VALUE"></NAME>
  */
class SettingsNodeValue
{
public:
    SettingsNodeValue(QString name) { _name = name; _description = "";}
    SettingsNodeValue(QString name, QVariant value) { _name = name; _value = value; _description = "";}
    SettingsNodeValue(QString name, QVariant value, QString description) { _name = name; _value = value; _description = description;}

    /**
      * \brief Une valeur est valide si son nom n'est pas vide et que la valeur à été définie
      */
    bool isValid() const { return (!_name.isEmpty() && _value.isValid()); }

    /**
      * \brief Une valeur est repéré par un nom (utilisé lors de l'écriture dans un fichier)
      */
    QString name() const { return _name; }

    /**
      * \brief et a comme valeur un QVariant (utilisé lors de l'écriture dans un fichier)
      */
    QVariant value() const { return _value; }

    /**
      * \brief Description (utilisé lors de l'écriture dans un fichier)
      */
    QString description() const { return _description; }

    /**
      * \brief On peut lui définir une valeur (utilisé lors de la lecture à partir d'un fichier)
      */
    void setValue(const QVariant &value) { _value = value; }

private:

    QString     _name;
    QVariant    _value;
    QString     _description;
};

/**
  * \brief Classe représentant un groupe de paramètres
  *
  * On pourra écrire un groupe dans un fichier sous le format suivant par exemple :
  *
  *     <GROUP_NAME>
  *         <CHILD_GROUP_NAME>
  *             <VALUE_NAME value="VALUE"></VALUE_NAME>
  *         </CHILD_GROUP_NAME>
  *     </GROUP_NAME>
  */
class SettingsNodeGroup
{
public:
    SettingsNodeGroup(QString name) { _name = name; }
    ~SettingsNodeGroup()
    {
        qDeleteAll(_groups.begin(), _groups.end());
        qDeleteAll(_values.begin(), _values.end());
    }

    /**
      * \brief Un groupe est valide si son nom n'est pas vide
      */
    bool isValid() const { return !_name.isEmpty(); }

    /**
      * \brief Un groupe est repéré par un nom (utilisé lors de l'écriture dans un fichier)
      */
    QString name() const { return _name; }

    /**
      * \brief Il contient 0 ou plusieurs groupes (utilisé lors de l'écriture dans un fichier)
      */
    const QList<SettingsNodeGroup*>& groups() const { return _groups; }

    /**
      * \brief Il contient 0 ou plusieurs valeurs (utilisé lors de l'écriture dans un fichier)
      */
    const QList<SettingsNodeValue*>& values() const { return _values; }

    /**
      * \brief On peut lui ajouter un nouveau groupe (utilisé lors de la lecture à partir d'un fichier)
      */
    bool addGroup(SettingsNodeGroup *group) { _groups.append(group); return true; }

    /**
      * \brief On peut lui ajouter une nouvelle valeur (utilisé lors de la lecture à partir d'un fichier)
      */
    bool addValue(SettingsNodeValue *value) { _values.append(value); return true; }

    /**
      * \brief Retourne tous les groupes dont le nom est égual à celui passé en paramètre
      */
    QList<SettingsNodeGroup*> groupsByTagName(QString name) const
    {
        QList<SettingsNodeGroup*> list;

        QListIterator<SettingsNodeGroup*> it(_groups);

        while(it.hasNext())
        {
            SettingsNodeGroup *group = it.next();

            if(group->name() == name)
                list.append(group);
        }

        return list;
    }

    /**
     * @brief Returns the first group with the name passed in parameter. NULL if it was not found.
     * @param name : the name of the group to search
     * @return  NULL if it was not found
     */
    SettingsNodeGroup* firstGroupByTagName(QString name) const
    {
        QListIterator<SettingsNodeGroup*> it(_groups);

        while(it.hasNext())
        {
            SettingsNodeGroup *group = it.next();

            if(group->name() == name)
                return group;
        }

        return NULL;
    }

    /**
      * \brief Retourne toutes les valeurs dont le nom est égual à celui passé en paramètre
      */
    QList<SettingsNodeValue*> valuesByTagName(QString name) const
    {
        QList<SettingsNodeValue*> list;

        QListIterator<SettingsNodeValue*> it(_values);

        while(it.hasNext())
        {
            SettingsNodeValue *value = it.next();

            if(value->name() == name)
                list.append(value);
        }

        return list;
    }

    /**
     * @brief Returns the first value with the name passed in parameter. NULL if it was not found.
     * @param name : the name of the value to search
     * @return  NULL if it was not found
     */
    SettingsNodeValue* firstValueByTagName(QString name) const
    {
        QListIterator<SettingsNodeValue*> it(_values);

        while(it.hasNext())
        {
            SettingsNodeValue *value = it.next();

            if(value->name() == name)
                return value;
        }

        return NULL;
    }

private:

    QString                     _name;
    QList<SettingsNodeGroup*>   _groups;
    QList<SettingsNodeValue*>   _values;
};

#endif // INTERFACESSETTINGS_H
