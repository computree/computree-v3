#include "batchpluginmanager.h"

#include <QDir>

BatchPluginManager::BatchPluginManager()
{
    _defaultPluginDirPath = "./bplugins";
}

BatchPluginManager::~BatchPluginManager()
{
    clearPlugins();
}

bool BatchPluginManager::load()
{
    _errorWhenLoad = "";

    clearPlugins();

    QDir dir(_defaultPluginDirPath);

    //qDebug() << "Recherche des plugins dans : " << dir.absolutePath();

    QStringList nameFilters;

    #if defined(_WIN32) || defined(_WIN64)
        nameFilters << "bplug_*.dll";
    #elif defined(__linux__)
        nameFilters << "libbplug_*.so*";
    #elif defined(TARGET_OS_MAC)
        nameFilters << "libbplug_*.dylib";
    #endif

    foreach (QString fileName, dir.entryList(nameFilters, QDir::Files))
    {
        QString filePath = dir.absoluteFilePath(fileName);
        QFileInfo fileInfo(fileName);

        QPluginLoader *loader = new QPluginLoader(filePath);

        QObject *plugin = loader->instance();

        if(plugin)
        {
            BatchPluginInterface *myPlugin = qobject_cast<BatchPluginInterface *>(plugin);

            if(myPlugin)
            {
                QString plugName = fileInfo.completeBaseName();
                plugName = plugName.mid(plugName.indexOf("bplug_", 0, Qt::CaseInsensitive));

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
        _errorWhenLoad = QString("Aucun plugin dans le dossier : %1").arg(_defaultPluginDirPath);
    }

    return _errorWhenLoad.isEmpty();
}

QString BatchPluginManager::getError() const
{
    return _errorWhenLoad;
}

QString BatchPluginManager::getSearchPath() const
{
    return _defaultPluginDirPath;
}

void BatchPluginManager::setSearchPath(QString path)
{
    _defaultPluginDirPath = path;
}

bool BatchPluginManager::isAPluginLoaded() const
{
    return !_plugins.isEmpty();
}

BatchPluginInterface* BatchPluginManager::getPlugin(int i) const
{
    return qobject_cast<BatchPluginInterface *>((_plugins.constBegin()+i).value()->instance());
}

QString BatchPluginManager::getPluginName(int i) const
{
    return (_plugins.constBegin()+i).key();
}

int BatchPluginManager::nPlugins() const
{
    return _plugins.size();
}

void BatchPluginManager::clearPlugins()
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
