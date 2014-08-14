#include "cdm_tools.h"

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
        PluginInterface *plugin = m_pm->getPlugin(i);
        QList<IExporterSeparator*> separators = plugin->getExportersAvailable();

        if(!separators.isEmpty())
        {
            QMenu *sepMenu = new QMenu(m_pm->getPluginName(i));

            QListIterator<IExporterSeparator*> itS(separators);

            while(itS.hasNext())
            {
                IExporterSeparator *sep = itS.next();

                if(!sep->exporters().isEmpty())
                {
                    QListIterator<IExporter*> itE(sep->exporters());

                    while(itE.hasNext())
                    {
                        IExporter *exp = itE.next();

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

QString CDM_Tools::createFileExtensionForExporter(IExporter *exporter) const
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

QString CDM_Tools::createNameForExporter(IExporter *exporter) const
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
