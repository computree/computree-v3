#include "cdm_pluginmanager.h"

#include "cdm_configfile.h"

#include <QDir>
#include <QDebug>

CDM_PluginManager::CDM_PluginManager()
{
    _defaultPluginDirPath = "./plugins";
    m_guiManager = NULL;

    loadConfiguration();
}

CDM_PluginManager::~CDM_PluginManager()
{
    clearPlugins();

    writeConfiguration();
}

void CDM_PluginManager::setGuiManager(const GuiManagerInterface *gManager)
{
    m_guiManager = (GuiManagerInterface*)gManager;
}

LogInterface* CDM_PluginManager::log() const
{
    return const_cast<LogInterface*>((const LogInterface*)&m_log);
}

PluginManagerInterface* CDM_PluginManager::pluginManager() const
{
    return const_cast<PluginManagerInterface*>((const PluginManagerInterface*)this);
}

ContextInterface* CDM_PluginManager::getPluginsContext() const
{
    if(_plugins.isEmpty())
        return NULL;

    return getPlugin(0)->contextInterface();
}

void CDM_PluginManager::loadConfiguration()
{
    CONFIG_FILE->beginGroup("PluginManager");

    _defaultPluginDirPath = CONFIG_FILE->value("defaultPluginDirPath", _defaultPluginDirPath).toString();

    CONFIG_FILE->endGroup();
}

void CDM_PluginManager::writeConfiguration()
{
    CONFIG_FILE->beginGroup("PluginManager");

    CONFIG_FILE->setValue("defaultPluginDirPath", _defaultPluginDirPath);

    CONFIG_FILE->endGroup();
}

bool CDM_PluginManager::load()
{
    bool mustCancel = false;

    emit startLoading(mustCancel);

    if(!mustCancel)
    {
        _errorWhenLoad = "";

        clearPlugins();

        QDir dir(_defaultPluginDirPath);

        //qDebug() << "Recherche des plugins dans : " << dir.absolutePath();

        QStringList nameFilters;

        #if defined(_WIN32) || defined(_WIN64)
            nameFilters << "plug_*.dll";
        #elif defined(__linux__)
            nameFilters << "libplug_*.so*";
        #elif defined(TARGET_OS_MAC)
            nameFilters << "libplug_*.dylib";
        #endif

        foreach (QString fileName, dir.entryList(nameFilters, QDir::Files))
        {
            QString filePath = dir.absoluteFilePath(fileName);
            QFileInfo fileInfo(fileName);

            QPluginLoader *loader = new QPluginLoader(filePath);

            QObject *plugin = loader->instance();

            if(plugin)
            {
                PluginEntryInterface *myPlugin = qobject_cast<PluginEntryInterface*>(plugin);

                if(myPlugin)
                {
                    QString plugName = fileInfo.completeBaseName();
                    plugName = plugName.mid(plugName.indexOf("plug_", 0, Qt::CaseInsensitive));

                    myPlugin->getPlugin()->setCoreInterface(this);
                    myPlugin->getPlugin()->setGuiManager(m_guiManager);
                    myPlugin->getPlugin()->init();

                    _plugins.insert(plugName, loader);
                }
                else
                {
                    _errorWhenLoad += loader->errorString() + "\r\n\r\n";
                    loader->unload();
                    delete loader;
                }
            }
            else
            {
                _errorWhenLoad += loader->errorString() + "\r\n\r\n";
                delete loader;
            }
        }

        if(_plugins.isEmpty()
                && _errorWhenLoad.isEmpty())
        {
            _errorWhenLoad = tr("Aucun plugin dans le dossier : %1").arg(_defaultPluginDirPath);
        }

        if(!_plugins.isEmpty())
        {
            int s = countPluginLoaded();

            for(int i=0; i<s; ++i)
                getPlugin(i)->initAfterAllPluginsLoaded();
        }
    }
    else
    {
        _errorWhenLoad = tr("Chargement annulé");
    }

    emit finishLoading();

    return _errorWhenLoad.isEmpty();
}

QString CDM_PluginManager::getError() const
{
    return _errorWhenLoad;
}

bool CDM_PluginManager::isAPluginLoaded() const
{
    return !_plugins.isEmpty();
}

int CDM_PluginManager::countPluginLoaded() const
{
    return _plugins.size();
}

QString CDM_PluginManager::getPluginName(int i) const
{
    if(isAPluginLoaded())
    {
        QMap<QString, QPluginLoader*>::const_iterator it = _plugins.constBegin();

        while((i>=0)
                && (it != _plugins.constEnd()))
        {
            if(i == 0)
            {
                return it.key();
            }

            ++it;
            --i;
        }
    }

    return "";
}

QString CDM_PluginManager::getPluginName(PluginInterface *stepPluginManager) const
{
    if(isAPluginLoaded())
    {
        QMap<QString, QPluginLoader*>::const_iterator it = _plugins.constBegin();

        while(it != _plugins.constEnd())
        {
            if(qobject_cast<PluginEntryInterface*>(it.value()->instance())->getPlugin() == stepPluginManager)
            {
                return it.key();
            }

            ++it;
        }
    }

    return "";
}

QString CDM_PluginManager::getSearchPath() const
{
    return _defaultPluginDirPath;
}

void CDM_PluginManager::setSearchPath(QString path)
{
    _defaultPluginDirPath = path;
}

PluginInterface* CDM_PluginManager::getPlugin(int i) const
{
    if(isAPluginLoaded())
    {
        QMap<QString, QPluginLoader*>::const_iterator it = _plugins.constBegin();

        while((i>=0)
                && (it != _plugins.constEnd()))
        {
            if(i == 0)
            {
                return qobject_cast<PluginEntryInterface*>(it.value()->instance())->getPlugin();
            }

            ++it;
            --i;
        }
    }

    return NULL;
}

PluginInterface* CDM_PluginManager::getPlugin(QString pluginName) const
{
    if(isAPluginLoaded())
    {
        QMap<QString, QPluginLoader*>::const_iterator it = _plugins.constBegin();

        while(it != _plugins.constEnd())
        {
            if(it.key() == pluginName)
            {
                return qobject_cast<PluginEntryInterface*>(it.value()->instance())->getPlugin();
            }

            ++it;
        }
    }

    return NULL;
}

void CDM_PluginManager::clearPlugins()
{
    QMapIterator<QString, QPluginLoader*> it(_plugins);

    while(it.hasNext())
    {
        it.next();
        it.value()->unload();
    }

    qDeleteAll(_plugins.begin(), _plugins.end());
    _plugins.clear();
}
