#include "qmmp.h"

#include <QDir>
#include <QDebug>
#include <QTextCodec>
#include <QCoreApplication>

QString Qmmp::configFile()
{
    return configDir() + "/qmmprc";
}

QString Qmmp::configDir()
{
    QString path;
    bool portable = false;
    const QStringList filters{"./", "../"};

    for(const QString &filter : qAsConst(filters))
    {
        if(QFile::exists(filter + "ttk_portable"))
        {
            path = filter;
            portable = true;
            break;
        }
    }
#ifdef Q_OS_WIN
    return (portable ? path : QString::fromLocal8Bit(getenv("APPDATA")) + "/") + "ttkmp/";
#else
    return (portable ? path : QDir::homePath() + "/") + ".config/ttkmp/";
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
    qDebug() << __FUNCTION__ << "Qmmp plugin path by QMMP_PLUGINS is: " << path.data();
    if(!path.isEmpty())
    {
        return path;
    }
		
    QDir dir(QCoreApplication::applicationDirPath() + "/plugins");
    qDebug() << __FUNCTION__ << "Qmmp plugin path by canonical path is: " << dir.canonicalPath();
    qDebug() << __FUNCTION__ << "Qmmp plugin path by absolute path is: " << dir.absolutePath();
    return dir.canonicalPath();
}

QStringList Qmmp::findPlugins(const QString &prefix)
{
    QDir pluginDir(pluginPath() + "/" + prefix);
    qDebug() << __FUNCTION__ << "Qmmp plugin dir absolute path is: " << pluginDir.absolutePath();
    QStringList paths;
    for(const QFileInfo &fin : pluginDir.entryInfoList({"*.dll", "*.so", "*.dylib"}, QDir::Files))
    {
        qDebug() << __FUNCTION__ << "Qmmp plugin canonical path: " << fin.canonicalFilePath();
        qDebug() << __FUNCTION__ << "Qmmp plugin absolute path: " << fin.absolutePath();
        qDebug() << __FUNCTION__ << "Qmmp plugin absolute file path: " << fin.absoluteFilePath();
        paths << fin.canonicalFilePath();
    }
    qDebug() << __FUNCTION__ << "Qmmp plugins path: " << paths;
    return paths;
}

QString Qmmp::ttkPluginPath()
{
    QDir dir(QCoreApplication::applicationDirPath() + "/GPlugins");
    return dir.canonicalPath();
}

QByteArray Qmmp::generatePrintable(const QString &text)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0) && defined Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    return codec ? codec->fromUnicode(text) : QByteArray();
#else
    return text.toLocal8Bit();
#endif
}
