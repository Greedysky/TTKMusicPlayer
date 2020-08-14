#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QByteArray>

#include "qmmp.h"

QString Qmmp::m_configDir;

QString Qmmp::configFile()
{
    return configDir() + "/qmmprc";
}

QString Qmmp::configDir()
{
    if(m_configDir.isEmpty())
#ifdef Q_OS_WIN
        return QString::fromLocal8Bit(getenv("APPDATA")) + "/ttkmp";
#else
        return QDir::homePath() + "/.config/ttkmp";
#endif
    else
        return m_configDir;
}

void Qmmp::setConfigDir(const QString &path)
{
    m_configDir = path;
}

QString Qmmp::strVersion()
{
    QString ver = QString("%1.%2.%3")
            .arg(QMMP_VERSION_MAJOR)
            .arg(QMMP_VERSION_MINOR)
            .arg(QMMP_VERSION_PATCH);
    return ver;
}

QString Qmmp::pluginPath()
{
    QByteArray path = qgetenv("QMMP_PLUGINS");
    if(!path.isEmpty())
        return path;
		
    QDir dir(qApp->applicationDirPath() + "/plugins");
    return dir.canonicalPath();
}

QStringList Qmmp::findPlugins(const QString &prefix)
{
    QDir pluginDir(pluginPath() + "/" + prefix);
    QStringList paths;
    for(const QFileInfo &info : pluginDir.entryInfoList(QStringList() << "*.dll" << "*.so", QDir::Files))
        paths << info.canonicalFilePath();
    return paths;
}
