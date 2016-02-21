#include "musicutils.h"

#include <QComboBox>

void MusicUtils::dirIsExist(const QString& name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void MusicUtils::checkTheDirectoryExist()
{
    dirIsExist(MusicObject::getAppDir() + LRC_DOWNLOAD);
    dirIsExist(MusicObject::getAppDir() + MUSIC_DOWNLOAD);
    dirIsExist(MusicObject::getAppDir() + MOVIE_DOWNLOAD);
    dirIsExist(MusicObject::getAppDir() + DATA_CACHED);
    dirIsExist(MusicObject::getAppDir() + THEME_DOWNLOAD);
    dirIsExist(MusicObject::getAppDir() + ART_DOWNLOAD);
    dirIsExist(MusicObject::getAppDir() + TRANS_PLUGINS);
    dirIsExist(MusicObject::getAppDir() + ART_BG);
    dirIsExist(MusicObject::getAppDir() + TR_LANGUAGE);
}

bool MusicUtils::checkTheFileExist()
{
    return QFile::exists(MusicObject::getAppDir() + DOWNLOADINFO) &&
           QFile::exists(MusicObject::getAppDir() + DARABASEPATH) &&
           QFile::exists(MusicObject::getAppDir() + USERPATH) &&
           QFile::exists(MusicObject::getAppDir() + COFIGPATH) &&
           QFile::exists(MusicObject::getAppDir() + MUSICPATH) &&
           QFile::exists(MusicObject::getAppDir() + MUSICSEARCH) &&
           QFile::exists(MusicObject::getAppDir() + NETDADIOPATH);
}

QString MusicUtils::getLanguageName(int index)
{
    QString lan(MusicObject::getAppDir() + TR_LANGUAGE);
    switch(index)
    {
        case 0 : return lan.append("cn.ln");
        case 1 : return lan.append("cn_c.ln");
        case 2 : return lan.append("en.ln");
        default: return QString();
    }
}

void MusicUtils::setLabelFont(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void MusicUtils::setComboboxText(QComboBox *object, const QString &text)
{
    if(object->isEditable())
    {
        object->setEditText(text);
    }
    else
    {
        const int i = object->findText(text);
        if (i > -1)
        {
            object->setCurrentIndex(i);
        }
    }
}

QString MusicUtils::size2Number(qint64 size)
{
    if( size < pow(1024, 1))
    {
        return QString("%1").arg(size);
    }
    else if( pow(1024, 1) <= size && size < pow(1024, 2))
    {
        return QString("%1").arg((qint64)(size*1.0/pow(1024, 1)*100)/100.0);
    }
    else if( pow(1024, 2) <= size && size < pow(1024, 3))
    {
        return QString("%1").arg((qint64)(size*1.0/pow(1024, 2)*100)/100.0);
    }
    else if( pow(1024, 3) <= size && size < pow(1024, 4))
    {
        return QString("%1").arg((qint64)(size*1.0/pow(1024, 3)*100)/100.0);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::size2Label(qint64 size)
{
    QString label = size2Number(size);
    if( size < pow(1024, 1))
    {
        return QString("%1 Byte").arg(label);
    }
    else if( pow(1024, 1) <= size && size < pow(1024, 2))
    {
        return QString("%1 KByte").arg(label);
    }
    else if( pow(1024, 2) <= size && size < pow(1024, 3))
    {
        return QString("%1 MByte").arg(label);
    }
    else if( pow(1024, 3) <= size && size < pow(1024, 4))
    {
        return QString("%1 TByte").arg(label);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::speed2Label(qint64 size)
{
    QString label = size2Number(size);
    if( size < pow(1024, 1))
    {
        return QString("%1 B/s").arg(label);
    }
    else if( pow(1024, 1) <= size && size < pow(1024, 2))
    {
        return QString("%1 KB/s").arg(label);
    }
    else if( pow(1024, 2) <= size && size < pow(1024, 3))
    {
        return QString("%1 MByte").arg(label);
    }
    else if( pow(1024, 2) <= size && size < pow(1024, 4))
    {
        return QString("%1 TByte").arg(label);
    }
    else
    {
        return QString();
    }
}

qreal MusicUtils::sizeByte2KByte(qint64 size)
{
    return size / 1024.0;
}

qreal MusicUtils::sizeByte2MByte(qint64 size)
{
    return sizeByte2KByte(size) / 1024.0;
}

qreal MusicUtils::sizeByte2TByte(qint64 size)
{
    return sizeByte2MByte(size) / 1024.0;
}

quint64 MusicUtils::dirSize(const QString &dirName)
{
    quint64 size = 0;
    if(QFileInfo(dirName).isDir())
    {
        QDir dir(dirName);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs |  QDir::Hidden |
                                               QDir::NoSymLinks | QDir::NoDotAndDotDot);
        foreach(QFileInfo fileInfo, list)
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

void MusicUtils::checkCacheSize(quint64 cacheSize, bool disabled, const QString &path)
{
    if(disabled)
    {
        quint64 size = dirSize( path );
        if( size > cacheSize)
        {
            QFileInfoList fileList = QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
            foreach(QFileInfo fileInfo, fileList)
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
