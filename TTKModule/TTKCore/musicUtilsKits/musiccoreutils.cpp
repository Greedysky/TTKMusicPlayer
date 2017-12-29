#include "musiccoreutils.h"
#include "musicsettingmanager.h"
#include "musicversion.h"

#include <QUrl>
#include <QTextCodec>
#include <QSettings>
#include <QProcess>
#include <QDirIterator>
#include <QDesktopServices>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <shellapi.h>
#endif
///qmmp incldue
#include "qmmp.h"

QString MusicUtils::Core::lrcPrefix()
{
    QString path = M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDirChoiced).toString();
    if(path.isEmpty())
    {
        path = LRC_DIR_FULL;
    }
    else
    {
        if(!QDir(path).exists())
        {
            QDir().mkpath(path);
        }
    }
    return path;
}

QString MusicUtils::Core::musicPrefix()
{
    QString path = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDirChoiced).toString();
    if(path.isEmpty())
    {
        path = MUSIC_DIR_FULL;
    }
    else
    {
        if(!QDir(path).exists())
        {
            QDir().mkpath(path);
        }
    }
    return path;
}

QString MusicUtils::Core::fileSuffix(const QString &name)
{
    return name.right(name.length() - name.lastIndexOf(".") - 1);
}

quint64 MusicUtils::Core::dirSize(const QString &dirName)
{
    quint64 size = 0;
    if(QFileInfo(dirName).isDir())
    {
        QDir dir(dirName);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden |
                                               QDir::NoSymLinks | QDir::NoDotAndDotDot);
        foreach(const QFileInfo &fileInfo, list)
        {
            if(fileInfo.isDir())
            {
                size += dirSize(fileInfo.absoluteFilePath());
            }
            else
            {
                size += fileInfo.size();
            }
        }
    }
    return size;
}

void MusicUtils::Core::checkCacheSize(quint64 cacheSize, bool disabled, const QString &path)
{
    if(disabled)
    {
        quint64 size = dirSize( path );
        if( size > cacheSize)
        {
            QFileInfoList fileList = QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            foreach(const QFileInfo &fileInfo, fileList)
            {
                size -= fileInfo.size();
                QFile::remove(fileInfo.absoluteFilePath());
                if(size <= cacheSize)
                {
                    break;
                }
            }
        }
    }
}

QFileInfoList MusicUtils::Core::findFile(const QString &path, const QStringList &filter)
{
    ///Find the corresponding suffix name
    QDir dir(path);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(const QFileInfo &folder, folderList)
    {
        fileList.append( findFile(folder.absoluteFilePath(), filter) );
    }
    return fileList;
}

QString MusicUtils::Core::getLanguageName(int index)
{
    QString lan(LANGUAGE_DIR_FULL);
    switch(index)
    {
        case 0 : return lan.append("cn.ln");
        case 1 : return lan.append("cn_c.ln");
        case 2 : return lan.append("en.ln");
        default: return QString();
    }
}

bool MusicUtils::Core::removeRecursively(const QString &dir)
{
    QDir dr(dir);
    if(!dr.exists())
    {
        return true;
    }

    bool success = true;
    const QString dirPath = dr.path();
    // not empty -- we must empty it first
    QDirIterator di(dirPath, QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    while(di.hasNext())
    {
        di.next();
        const QFileInfo &fi = di.fileInfo();
        const QString &filePath = di.filePath();
        bool ok;
        if(fi.isDir() && !fi.isSymLink())
        {
            ok = MusicUtils::Core::removeRecursively(filePath); // recursive
        }
        else
        {
            ok = QFile::remove(filePath);
            if(!ok)
            {
                // Read-only files prevent directory deletion on Windows, retry with Write permission.
                const QFile::Permissions permissions = QFile::permissions(filePath);
                if(!(permissions & QFile::WriteUser))
                {
                    ok = QFile::setPermissions(filePath, permissions | QFile::WriteUser)
                      && QFile::remove(filePath);
                }
            }
        }

        if(!ok)
        {
            success = false;
        }
    }

    if(success)
    {
        success = dr.rmdir(dr.absolutePath());
    }

    return success;
}

bool MusicUtils::Core::openUrl(const QString &exe, const QString &path)
{
#ifdef Q_OS_WIN
    HINSTANCE value = ShellExecuteA(0, exe.toLocal8Bit(), path.toLocal8Bit(), nullptr, nullptr, SW_SHOWNORMAL);
    return (int)value >= 32;
#else
    Q_UNUSED(exe);
    return QProcess::startDetached(path, QStringList());
#endif
}

bool MusicUtils::Core::openUrl(const QString &path, bool local)
{
#ifdef Q_OS_WIN
    if(path.isEmpty())
    {
        return false;
    }

    if(local)
    {
        QString p = path;
        p.replace('/', "\\");
        p = "/select," + p;
        HINSTANCE value = ShellExecuteA(0, "open", "explorer.exe", toLocal8Bit(p), nullptr, SW_SHOWNORMAL);
        return (int)value >= 32;
    }
#else
    Q_UNUSED(local);
#endif
    return QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

QString MusicUtils::Core::toUnicode(const char *chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QString MusicUtils::Core::toUnicode(const QByteArray &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QByteArray MusicUtils::Core::fromUnicode(const QString &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->fromUnicode(chars);
}

void MusicUtils::Core::setLocalCodec(const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    QTextCodec::setCodecForLocale(codec);
#ifndef MUSIC_GREATER_NEW
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}

const char* MusicUtils::Core::toLocal8Bit(const QString &str)
{
    return str.toLocal8Bit().constData();
}

const char* MusicUtils::Core::toUtf8(const QString &str)
{
    return str.toUtf8().constData();
}

QString MusicUtils::Core::pluginPath(const QString &module, const QString &format)
{
    QString path = MusicObject::getAppDir();
#ifdef Q_OS_WIN
    path = path + QString("plugins/%1/%2.dll").arg(module).arg(format);
#elif defined Q_OS_UNIX
    path = path + QString("qmmp/%1/lib%2.so").arg(module).arg(format);
#endif
    return path;
}

void MusicUtils::Core::midTransferFile()
{
    QString conf_path = MAKE_CONFIG_DIR_FULL + QString("wildmidi.cfg");
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("Midi");
    settings.setValue("conf_path", conf_path);
    settings.endGroup();

    QFile file(conf_path);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();

        if(file.open(QFile::WriteOnly))
        {
            data.remove(0, data.indexOf("\r\n"));
            data.insert(0, QString("dir %1freepats/").arg(MAKE_CONFIG_DIR_FULL));
            file.write(data);
        }
    }
    file.close();
}

bool MusicUtils::Core::musicVersionCheck(const QStringList &ol, const QStringList &dl, int depth)
{
    if(depth >= ol.count())
    {
        return false;
    }

    if(dl[depth].toInt() >= ol[depth].toInt())
    {
        if(dl[depth].toInt() == ol[depth].toInt())
        {
            return musicVersionCheck(ol, dl, depth + 1);
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool MusicUtils::Core::musicVersionCheck(const QString &o, const QString &d)
{
    QStringList ol = o.split(".");
    QStringList dl = d.split(".");

    if(ol.isEmpty() || dl.isEmpty() || ol.count() != dl.count())
    {
        return false;
    }

    return musicVersionCheck(ol, dl, 0);
}
