#include "qmmp.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QTextCodec>

QString Qmmp::configFile()
{
    return configDir() + "/qmmprc";
}

QString Qmmp::configDir()
{
#ifdef Q_OS_WIN
    return QString::fromLocal8Bit(getenv("APPDATA")) + "/ttkmp";
#else
    return QDir::homePath() + "/.config/ttkmp";
#endif
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

const char *Qmmp::textCodec(const QString &text)
{
#if QT_VERSION < QT_VERSION_CHECK(5,0,0) && defined Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("GB18030");
    return codec->fromUnicode(text).constData();
#else
    return qPrintable(text);
#endif
}
