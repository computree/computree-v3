#ifndef PB_STEPGENERICLOADFILE_H
#define PB_STEPGENERICLOADFILE_H

#include "ct_step/abstract/ct_abstractsteploadfile.h"
#include "ct_reader/abstract/ct_abstractreader.h"

class PB_StepGenericLoadFile : public CT_AbstractStepLoadFile
{
    Q_OBJECT

public:
    PB_StepGenericLoadFile(CT_StepInitializeData &dataInit, CT_AbstractReader *reader);
    ~PB_StepGenericLoadFile();

    void init();

    /**
     * @brief Inherited to return the name of the reader
     */
    virtual QString getStepName() const;

    QString getStepDescription() const;

    QList<QString> getFileExtensionAccepted() const;

    bool setFilePath(QString filePath);

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit);

protected:

    bool preConfigure();
    bool postConfigure();
    bool protectedInitAfterConfiguration();

    void createInResultModelListProtected();
    void createOutResultModelListProtected();

    void compute();

private:
    CT_AbstractReader *m_reader;

private slots:
    void readerProgressChanged(int progress);
};

#endif // PB_STEPGENERICLOADFILE_H
