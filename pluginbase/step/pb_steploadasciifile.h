#ifndef PB_STEPLOADASCIIFILE_H
#define PB_STEPLOADASCIIFILE_H

#include "ct_step/abstract/ct_abstractsteploadfileinscene.h"

#include "ct_view/ct_stepconfigurabledialog.h"

/*!
 * \class PB_StepLoadAsciiFile
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
class PB_StepLoadAsciiFile : public CT_AbstractStepLoadFileInScene
{
    Q_OBJECT

public:
    PB_StepLoadAsciiFile(CT_StepInitializeData &data);
    ~PB_StepLoadAsciiFile();

    virtual void init();
    virtual void aboutToBeDeleted();

    virtual SettingsNodeGroup* getAllSettings() const;
    virtual bool setAllSettings(const SettingsNodeGroup *settings);

    QString getStepDescription() const;

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);
    QList<FileFormat> getFileExtensionAccepted() const;

    bool mustShowConfigurationDialogBeforeAdd() const;
    bool showConfigurationDialogBeforeAdd();

protected:

    CT_StepConfigurableDialog   *_columnConfigDialog;
    QString                     _columnTab[7];

    static const QString COLUMN_X;
    static const QString COLUMN_Y;
    static const QString COLUMN_Z;
    static const QString COLUMN_INTENSITY;
    static const QString COLUMN_NONE;

    int readHeaderFile(QFile &f);
    void readDataFile(QFile &f, int offset, bool little_endian = false);

    void readDouble(double &outputDouble, QStringList& stringList, int nWord, bool &success, const QString &msgError );

    bool showColumnConfigurationDialog();

    void createColumnConfigurationDialog();

public:

    bool preConfigure();
};

#endif // PB_STEPLOADASCIIFILE_H
