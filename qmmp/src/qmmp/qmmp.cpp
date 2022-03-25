#include "qmmp.h"

#include <QDir>
#include <QTextCodec>
#include <QCoreApplication>

QString Qmmp::configFile()
{
    return configDir() + "/qmmprc";
}

QString Qmmp::configDir()
{
    const bool portable = QFile::exists("../ttk_portable");
#ifdef Q_OS_WIN
    return (portable ? QString("../") : QString::fromLocal8Bit(getenv("APPDATA")) + "/") + "ttkmp/";
#else
    return (portable ? QString("../") : QDir::homePath() + "/") + ".config/ttkmp/";
#endif
}

QString Qmmp::cacheDir()
{
    return configDir();
}

QString Qmmp::strVersion()
{
    return QString("%1.%2.%3")
            .arg(QMMP_VERSION_MAJOR)
            .arg(QMMP_VERSION_MINOR)
            .arg(QMMP_VERSION_PATCH);
}

QString Qmmp::pluginPath()
{
    const QByteArray &path = qgetenv("QMMP_PLUGINS");
    if(!path.isEmpty())
    {
        return path;
    }
		
    QDir dir(qApp->applicationDirPath() + "/plugins");
    return dir.canonicalPath();
}

QStringList Qmmp::findPlugins(const QString &prefix)
{
    QDir pluginDir(pluginPath() + "/" + prefix);
    QStringList paths;
    for(const QFileInfo &fin : pluginDir.entryInfoList(QStringList() << "*.dll" << "*.so", QDir::Files))
    {
        paths << fin.canonicalFilePath();
    }
    return paths;
}

QString Qmmp::ttkPluginPath()
{
    return Qmmp::pluginPath() + "/../GPlugins";
}

QByteArray Qmmp::generatePrintable(const QString &text)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0) && defined Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    return codec->fromUnicode(text);
#else
    return text.toLocal8Bit();
#endif
}
