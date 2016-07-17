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

void MusicUtils::UCore::dirIsExist(const QString &name)
{
    QDir dir;
    if(!dir.exists(name))
    {
        dir.mkdir(name);
    }
}

void MusicUtils::UCore::checkTheDirectoryExist()
{
    dirIsExist(LRC_DIR_FULL);
    dirIsExist(MUSIC_DIR_FULL);
    dirIsExist(MOVIE_DIR_FULL);
    dirIsExist(CACHE_DIR_FULL);
    dirIsExist(THEME_DIR_FULL);
    dirIsExist(ART_DIR_FULL);
    dirIsExist(PLUGINS_DIR_FULL);
    dirIsExist(BACKGROUND_DIR_FULL);
    dirIsExist(LANGUAGE_DIR_FULL);
}

bool MusicUtils::UCore::checkTheFileExist()
{
    return QFile::exists(DOWNLOADINFO_FULL) &&
           QFile::exists(DARABASEPATH_FULL) &&
           QFile::exists(USERPATH_FULL) &&
           QFile::exists(COFIGPATH_FULL) &&
           QFile::exists(MUSICPATH_FULL) &&
           QFile::exists(MUSICSEARCH_FULL) &&
           QFile::exists(NETDADIOPATH_FULL);
}

QString MusicUtils::UCore::getLanguageName(int index)
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

void MusicUtils::UWidget::setLabelFontSize(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void MusicUtils::UWidget::setLabelFontStyle(QWidget *widget, MusicObject::FontStyleType type)
{
    QFont font = widget->font();
    switch(type)
    {
        case MusicObject::FT_Bold : font.setBold(true); break;
        case MusicObject::FT_Italic : font.setItalic(true); break;
        case MusicObject::FT_Underline : font.setUnderline(true); break;
        case MusicObject::FT_Overline : font.setOverline(true); break;
        case MusicObject::FT_StrikeOut : font.setStrikeOut(true); break;
        case MusicObject::FT_FixedPitch : font.setFixedPitch(true); break;
        case MusicObject::FT_Kerningt : font.setKerning(true); break;
        default: break;
    }
    widget->setFont(font);
}

QString MusicUtils::UWidget::elidedText(const QFont &font, const QString &text,
                                        Qt::TextElideMode mode, int width)
{
    QFontMetrics ft(font);
    return ft.elidedText(text, mode, width);
}

void MusicUtils::UWidget::setTransparent(QWidget *widget, int alpha)
{
    QPalette pal = widget->palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, alpha)));
    widget->setPalette(pal);
}

void MusicUtils::UWidget::setComboboxText(QComboBox *object, const QString &text)
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

QBitmap MusicUtils::UWidget::getBitmapMask(const QRect &rect, int ratioX, int ratioY)
{
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, ratioX, ratioY);
    return mask;
}

void MusicUtils::UWidget::widgetToRound(QWidget *w, int ratioX, int ratioY)
{
    w->setMask( getBitmapMask(w->rect(), ratioX, ratioY) );
}

QPixmap MusicUtils::UWidget::pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY)
{
    return pixmapToRound(src, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap MusicUtils::UWidget::pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY)
{
    if(src.isNull())
    {
        return QPixmap();
    }

    QPixmap image = src.scaled(rect.size());
    image.setMask( getBitmapMask(rect, ratioX, ratioY) );
    return image;
}

QString MusicUtils::UNumber::size2Number(qint64 size)
{
    if( size < MH_KB2B)
    {
        return QString("%1").arg(size);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1").arg((qint64)(size*1.0/MH_KB2B*100)/100.0);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1").arg((qint64)(size*1.0/MH_MB2B*100)/100.0);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1").arg((qint64)(size*1.0/MH_GB2B*100)/100.0);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::UNumber::size2NumberInt(qint64 size)
{
    QString label = size2Number(size);
    if(label.contains("."))
    {
        label = label.split(".").front();
    }
    return label;
}

QString MusicUtils::UNumber::size2Label(qint64 size)
{
    QString label = size2Number(size);
    if( size < MH_KB2B)
    {
        return QString("%1 Byte").arg(label);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1 KByte").arg(label);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1 MByte").arg(label);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1 TByte").arg(label);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::UNumber::speed2Label(qint64 size)
{
    return speed2LabelFromLabel(size, size2Number(size));
}

QString MusicUtils::UNumber::speed2LabelInt(qint64 size)
{
    return speed2LabelFromLabel(size, size2NumberInt(size));
}

QString MusicUtils::UNumber::speed2LabelFromLabel(qint64 size, const QString &label)
{
    if( size < MH_KB2B)
    {
        return QString("%1 B/s").arg(label);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1 KB/s").arg(label);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1 MB/s").arg(label);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1 TB/s").arg(label);
    }
    else
    {
        return QString();
    }
}

qreal MusicUtils::UNumber::sizeByte2KByte(qint64 size)
{
    return size*1.0 / MH_KB;
}

qreal MusicUtils::UNumber::sizeByte2MByte(qint64 size)
{
    return sizeByte2KByte(size) / MH_MB;
}

qreal MusicUtils::UNumber::sizeByte2TByte(qint64 size)
{
    return sizeByte2MByte(size) / MH_GB;
}

quint64 MusicUtils::UCore::dirSize(const QString &dirName)
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

void MusicUtils::UCore::checkCacheSize(quint64 cacheSize, bool disabled, const QString &path)
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

QFileInfoList MusicUtils::UCore::findFile(const QString &path, const QStringList &filter)
{
    ///Find the corresponding suffix name
    QDir dir(path);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach(QFileInfo folder, folderList)
    {
        fileList.append( findFile(folder.absoluteFilePath(), filter) );
    }
    return fileList;
}

bool MusicUtils::UCore::openUrl(const QString &path, bool local)
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

QString MusicUtils::UCore::toUnicode(const char *chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QString MusicUtils::UCore::toUnicode(const QByteArray &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->toUnicode(chars);
}

QByteArray MusicUtils::UCore::fromUnicode(const QString &chars, const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    return codec->fromUnicode(chars);
}

void MusicUtils::UCore::setLocalCodec(const char *format)
{
    QTextCodec *codec = QTextCodec::codecForName(format);
    QTextCodec::setCodecForLocale(codec);
#ifndef MUSIC_GREATER_NEW
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
}

const char* MusicUtils::UCore::toLocal8Bit(const QString &str)
{
    return str.toLocal8Bit().constData();
}

const char* MusicUtils::UCore::toUtf8(const QString &str)
{
    return str.toUtf8().constData();
}
