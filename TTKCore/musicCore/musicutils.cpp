#include "musicutils.h"

#include <QComboBox>
#include <QBitmap>
#include <QPainter>
#include <QUrl>
#include <QDesktopServices>
#include <QTextCodec>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <shellapi.h>
#endif

void MusicUtils::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void MusicUtils::checkTheDirectoryExist()
{
    dirIsExist(LRC_DOWNLOAD_AL);
    dirIsExist(MUSIC_DOWNLOAD_AL);
    dirIsExist(MOVIE_DOWNLOAD_AL);
    dirIsExist(DATA_CACHED_AL);
    dirIsExist(THEME_DOWNLOAD_AL);
    dirIsExist(ART_DOWNLOAD_AL);
    dirIsExist(TRANS_PLUGINS_AL);
    dirIsExist(ART_BG_AL);
    dirIsExist(TR_LANGUAGE_AL);
}

bool MusicUtils::checkTheFileExist()
{
    return QFile::exists(DOWNLOADINFO_AL) &&
           QFile::exists(DARABASEPATH_AL) &&
           QFile::exists(USERPATH_AL) &&
           QFile::exists(COFIGPATH_AL) &&
           QFile::exists(MUSICPATH_AL) &&
           QFile::exists(MUSICSEARCH_AL) &&
           QFile::exists(NETDADIOPATH_AL);
}

QString MusicUtils::getLanguageName(int index)
{
    QString lan(TR_LANGUAGE_AL);
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

void MusicUtils::setTransparent(QWidget *widget, int alpha)
{
    QPalette pal = widget->palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, alpha)));
    widget->setPalette(pal);
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

QBitmap MusicUtils::getBitmapMask(const QRect &rect, int ratioX, int ratioY)
{
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, ratioX, ratioY);
    return mask;
}

void MusicUtils::widgetToRound(QWidget *w, int ratioX, int ratioY)
{
    w->setMask( getBitmapMask(w->rect(), ratioX, ratioY) );
}

QPixmap MusicUtils::pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY)
{
    return pixmapToRound(src, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap MusicUtils::pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY)
{
    if(src.isNull())
    {
        return QPixmap();
    }

    QPixmap image = src.scaled(rect.size());
    image.setMask( getBitmapMask(rect, ratioX, ratioY) );
    return image;
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

QString MusicUtils::size2NumberInt(qint64 size)
{
    QString label = size2Number(size);
    if(label.contains("."))
    {
        label = label.split(".").front();
    }
    return label;
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
    return speed2LabelFromLabel(size, size2Number(size));
}

QString MusicUtils::speed2LabelInt(qint64 size)
{
    return speed2LabelFromLabel(size, size2NumberInt(size));
}

QString MusicUtils::speed2LabelFromLabel(qint64 size, const QString &label)
{
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
        return QString("%1 MB/s").arg(label);
    }
    else if( pow(1024, 2) <= size && size < pow(1024, 4))
    {
        return QString("%1 TB/s").arg(label);
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

bool MusicUtils::openUrl(const QString &path)
{
#ifdef Q_OS_WIN
    if(path.isEmpty())
    {
        return false;
    }

    QString p = path;
    p.replace('/', "\\");
    p = " /select," + p;
    HINSTANCE value = ShellExecuteA(0, "open", "explorer.exe", p.toLocal8Bit().constData(), nullptr, true);
    return (int)value >= 32;
#else
    return QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
#endif
}

QString MusicUtils::toUnicode(const char *chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QString MusicUtils::toUnicode(const QByteArray &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QByteArray MusicUtils::fromUnicode(const QString &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->fromUnicode(chars);
}

void MusicUtils::setLocalCodec(const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    QTextCodec::setCodecForLocale(codec);
#ifndef MUSIC_QT_5
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}
