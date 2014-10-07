#include "cdm_tools.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"

CDM_Tools::CDM_Tools(const CDM_PluginManager *pm)
{
    m_pm = (CDM_PluginManager*)pm;
}

QMenu* CDM_Tools::createMenuForAllExporters(const QObject *receiver, const char *slot) const
{
    QMenu *menu = new QMenu();

    int s = m_pm->countPluginLoaded();

    for(int i=0; i<s; ++i)
    {
        CT_AbstractStepPlugin *plugin = m_pm->getPlugin(i);
        QList<CT_StandardExporterSeparator*> separators = plugin->getExportersAvailable();

        if(!separators.isEmpty())
        {
            QString pluginName = m_pm->getPluginName(i);
            if (pluginName.left(5) == "plug_")
            {
                pluginName.remove(0, 5);
            }

            QMenu *sepMenu = new QMenu(pluginName);

            QListIterator<CT_StandardExporterSeparator*> itS(separators);

            while(itS.hasNext())
            {
                CT_StandardExporterSeparator *sep = itS.next();

                if(!sep->exporters().isEmpty())
                {
                    QListIterator<CT_AbstractExporter*> itE(sep->exporters());

                    while(itE.hasNext())
                    {
                        CT_AbstractExporter *exp = itE.next();

                        QString name = createNameForExporter(exp);

                        if(!name.isEmpty())
                        {
                            QAction *expAction = new QAction(name, exp);

                            QObject::connect(expAction, SIGNAL(triggered()), receiver, slot);

                            sepMenu->addAction(expAction);
                        }
                    }
                }
            }

            if(sepMenu->isEmpty())
                delete sepMenu;
            else
                menu->addMenu(sepMenu);
        }
    }

    if(menu->isEmpty())
    {
        delete menu;
        menu = NULL;
    }

    return menu;
}

QString CDM_Tools::createFileExtensionForExporter(CT_AbstractExporter *exporter) const
{
    QString fileExtension;

    fileExtension += QObject::tr("All Valid Files (");

    // VALID FILES

    QListIterator<FileFormat> itFormat(exporter->exportFormats());

    if(itFormat.hasNext())
    {
        const FileFormat &format = itFormat.next();

        QListIterator<QString> it(format.suffixes());

        if(it.hasNext())
        {
            fileExtension += "*." + it.next();

            while(it.hasNext())
            {
                fileExtension += " *." + it.next();
            }
        }
    }

    fileExtension += ");;";

    return fileExtension;
}

QString CDM_Tools::createNameForExporter(CT_AbstractExporter *exporter) const
{
//    QString ret;

//    int n = 0;

//    QListIterator<FileFormat> it(exporter->exportFormats());

//    while(it.hasNext())
//    {
//        const FileFormat &format = it.next();

//        ret += "/";

//        QListIterator<QString> itS(format.suffixes());

//        if((n>0) && (itS.hasNext()))
//            ret += "/";

//        while(itS.hasNext())
//        {
//            ret += itS.next();

//            if(itS.hasNext())
//                ret += "/";
//        }

//        ++n;
//    }

    return exporter->getExporterCustomName();
}
