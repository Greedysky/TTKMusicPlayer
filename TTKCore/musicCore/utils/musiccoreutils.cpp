#include "musiccoreutils.h"
#include "musicsettingmanager.h"

#include <QUrl>
#include <QDesktopServices>
#include <QTextCodec>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <shellapi.h>
#endif

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

QStringList MusicUtils::Core::splitString(const QString &value)
{
    QStringList strings = value.split(" - ");
    if(strings.isEmpty())
    {
        strings = value.split("-");
    }
    return strings;
}

QString MusicUtils::Core::artistName(const QString &value, bool all)
{
    QString v = splitString(value).front().trimmed();

    if(!all)
    {
        QStringList splits;
        splits << "、" << "&" << "_" << "#";
        foreach(const QString &var, splits)
        {
            QStringList vs = v.split(var);
            if(!vs.isEmpty())
            {
                return vs.front().trimmed();
            }
        }
    }
    return v;
}

QString MusicUtils::Core::songName(const QString &value)
{
    return splitString(value).back().trimmed();
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
        p = " /select," + p;
        HINSTANCE value = ShellExecuteA(0, "open", "explorer.exe", toLocal8Bit(p), nullptr, true);
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
