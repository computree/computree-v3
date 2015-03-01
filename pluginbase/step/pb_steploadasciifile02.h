#ifndef PB_STEPLOADASCIIFILE02_H
#define PB_STEPLOADASCIIFILE02_H

#include "ct_step/abstract/ct_abstractsteploadfileinscene.h"

#include "ct_view/ct_stepconfigurabledialog.h"

/*!
 * \class PB_StepLoadAsciiFile02
 * \ingroup Steps_PB
 * \brief <b>Load a scene from ascii file</b>
 *
 * <b>Output Models:</b>
 *
 *  - CT_ResultGroup \n
 *      - CT_StandardItemGroup \n
 *          - CT_Scene \n
 *
 */
class PB_StepLoadAsciiFile02 : public CT_AbstractStepLoadFileInScene
{
    Q_OBJECT

public:
    PB_StepLoadAsciiFile02(CT_StepInitializeData &data);

    virtual void init();

    virtual SettingsNodeGroup* getAllSettings() const;
    virtual bool setAllSettings(const SettingsNodeGroup *settings);

    QString getStepDescription() const;

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);
    QList<QString> getFileExtensionAccepted() const;

    bool mustShowConfigurationDialogBeforeAdd() const;
    bool showConfigurationDialogBeforeAdd();


protected:


    int _linesToSkip;                        /*!< Nombre de lignes à ignorer avant les headers*/
    bool    _header;
    QString _separator;                      /*!< Séparateur de champs*/
    QString _localeName;
    int _columnX;                            /*!< Numéro de colonne où trouver X*/
    int _columnY;                            /*!< Numéro de colonne où trouver Y*/
    int _columnZ;                            /*!< Numéro de colonne où trouver Z*/
    int _columnI;                            /*!< Numéro de colonne où trouver l'intensité*/
    int _columnR;                            /*!< Numéro de colonne où trouver la couleur rouge*/
    int _columnG;                            /*!< Numéro de colonne où trouver la couleur verte*/
    int _columnB;                            /*!< Numéro de colonne où trouver la couleur bleu*/
    int _columnNX;                            /*!< Numéro de colonne où trouver la normale (X)*/
    int _columnNY;                            /*!< Numéro de colonne où trouver la normale (Y)*/
    int _columnNZ;                            /*!< Numéro de colonne où trouver la normale (Z)*/
    bool _firstCall;

    QList<CT_OutAbstractSingularItemModel*> createOtherOutResultListProtected(const QString &sceneModelName);

    int readHeaderFile(QFile &f);
    void readDataFile(QFile &f, int offset, bool little_endian = false);

    bool showColumnConfigurationDialog();


public:

    bool preConfigure();
    bool postConfigure();


};

#endif // PB_STEPLOADASCIIFILE02_H
