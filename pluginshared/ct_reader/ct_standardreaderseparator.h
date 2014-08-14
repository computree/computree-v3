#ifndef CT_STANDARDREADERSEPARATOR_H
#define CT_STANDARDREADERSEPARATOR_H

#include "ct_reader/abstract/ct_abstractreader.h"

class PLUGINSHAREDSHARED_EXPORT CT_StandardReaderSeparator
{
public:
    CT_StandardReaderSeparator(QString title = "");
    virtual ~CT_StandardReaderSeparator();

    bool addReader(CT_AbstractReader *reader);

    QString getTitle() const;
    QList<CT_AbstractReader*> readers() const;

private:

    QList<CT_AbstractReader*>   m_readers;
    QString                     m_title;

};

#endif // CT_STANDARDREADERSEPARATOR_H
