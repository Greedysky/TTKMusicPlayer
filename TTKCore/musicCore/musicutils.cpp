#include "musicutils.h"
#include "musicsettingmanager.h"

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



void MusicUtils::Widget::setLabelFontSize(QWidget *widget, int size)
{
    QFont font = widget->font();
    font.setPointSize(size);
    widget->setFont(font);
}

void MusicUtils::Widget::setLabelFontStyle(QWidget *widget, MusicObject::FontStyleType type)
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

QString MusicUtils::Widget::elidedText(const QFont &font, const QString &text,
                                        Qt::TextElideMode mode, int width)
{
    QFontMetrics ft(font);
    return ft.elidedText(text, mode, width);
}

void MusicUtils::Widget::setTransparent(QWidget *widget, int alpha)
{
    QPalette pal = widget->palette();
    pal.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, alpha)));
    widget->setPalette(pal);
}

void MusicUtils::Widget::setComboboxText(QComboBox *object, const QString &text)
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

void MusicUtils::Widget::widgetToRound(QWidget *w, int ratioX, int ratioY)
{
    w->setMask( getBitmapMask(w->rect(), ratioX, ratioY) );
}

void MusicUtils::Widget::fusionPixmap(QPixmap &bg, const QPixmap &fg, const QPoint &pt)
{
    QPainter painter(&bg);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawPixmap(pt.x(), pt.y(), fg);
    painter.end();
}

QPixmap MusicUtils::Widget::pixmapToRound(const QPixmap &src, const QSize &size, int ratioX, int ratioY)
{
    return pixmapToRound(src, QRect(QPoint(0, 0), size), ratioX, ratioY);
}

QPixmap MusicUtils::Widget::pixmapToRound(const QPixmap &src, const QRect &rect, int ratioX, int ratioY)
{
    if(src.isNull())
    {
        return QPixmap();
    }

    QPixmap image = src.scaled(rect.size());
    image.setMask( getBitmapMask(rect, ratioX, ratioY) );
    return image;
}

QBitmap MusicUtils::Widget::getBitmapMask(const QRect &rect, int ratioX, int ratioY)
{
    QBitmap mask(rect.size());
    QPainter painter(&mask);
    painter.fillRect(rect, Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.drawRoundedRect(rect, ratioX, ratioY);
    return mask;
}



QString MusicUtils::Number::size2Number(qint64 size)
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

QString MusicUtils::Number::size2NumberInt(qint64 size)
{
    QString label = size2Number(size);
    if(label.contains("."))
    {
        label = label.split(".").front();
    }
    return label;
}

QString MusicUtils::Number::size2Label(qint64 size)
{
    QString label = size2Number(size);
    if( size < MH_KB2B)
    {
        return QString("%1 B").arg(label);
    }
    else if( MH_KB2B <= size && size < MH_MB2B)
    {
        return QString("%1 KB").arg(label);
    }
    else if( MH_MB2B <= size && size < MH_GB2B)
    {
        return QString("%1 MB").arg(label);
    }
    else if( MH_GB2B <= size && size < MH_TB2B)
    {
        return QString("%1 TB").arg(label);
    }
    else
    {
        return QString();
    }
}

QString MusicUtils::Number::speed2Label(qint64 size)
{
    return speed2LabelFromLabel(size, size2Number(size));
}

QString MusicUtils::Number::speed2LabelInt(qint64 size)
{
    return speed2LabelFromLabel(size, size2NumberInt(size));
}

qreal MusicUtils::Number::sizeByte2KByte(qint64 size)
{
    return size*1.0 / MH_KB;
}

qreal MusicUtils::Number::sizeByte2MByte(qint64 size)
{
    return sizeByte2KByte(size) / MH_MB;
}

qreal MusicUtils::Number::sizeByte2TByte(qint64 size)
{
    return sizeByte2MByte(size) / MH_GB;
}

QString MusicUtils::Number::speed2LabelFromLabel(qint64 size, const QString &label)
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



QList<QColor> MusicUtils::String::readColorConfig(const QString &value)
{
    QList<QColor> colors;
    QStringList rgbs = value.split(';', QString::SkipEmptyParts);
    foreach(const QString &rgb, rgbs)
    {
        QStringList var = rgb.split(',');
        if(var.count() != 3)
        {
            continue;
        }
        colors << QColor(var[0].toInt(), var[1].toInt(), var[2].toInt());
    }
    return colors;
}

QString MusicUtils::String::writeColorConfig(const QColor &color)
{
    QString value;
    value.append(QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue()));
    return value;
}

QString MusicUtils::String::writeColorConfig(const QList<QColor> &colors)
{
    QString value;
    foreach(const QColor &rgb, colors)
    {
        value.append(QString("%1,%2,%3").arg(rgb.red()).arg(rgb.green()).arg(rgb.blue()));
        value.append(";");
    }
    return value;
}
