#include "musicstringutils.h"
#include "musicsettingmanager.h"

#include <QColor>

QString MusicUtils::String::lrcPrefix()
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

QString MusicUtils::String::musicPrefix()
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

QString MusicUtils::String::StringPrefix(const QString &name)
{
    return StringPrefix(name, ".");
}

QString MusicUtils::String::StringPrefix(const QString &name, const QString &prefix)
{
    return name.left(name.indexOf(prefix));
}

QString MusicUtils::String::StringSuffix(const QString &name)
{
    return StringSuffix(name, ".");
}

QString MusicUtils::String::StringSuffix(const QString &name, const QString &suffix)
{
    return name.right(name.length() - name.lastIndexOf(suffix) - 1);
}

QString MusicUtils::String::StringSplite(const QString &name)
{
    return StringSplite(name, ".", "?");
}

QString MusicUtils::String::StringSplite(const QString &name, const QString &prefix, const QString &suffix)
{
    const QString &data = StringSuffix(name, prefix);
    return StringPrefix(data, suffix);
}

QString MusicUtils::String::splitLineKey()
{
#ifdef Q_OS_WIN
    return "\r\n";
#else
    return "\n";
#endif
}

QString MusicUtils::String::removeStringBy(const QString &value, const QString &key)
{
    QString s = value;
    s.remove(key);
    if(s.contains(key))
    {
        s = removeStringBy(key);
    }
    return s;
}

QStringList MusicUtils::String::splitString(const QString &value, const QString &key)
{
    QStringList strings = value.split(QString(" %1 ").arg(key));
    if(strings.isEmpty() || strings.count() == 1)
    {
        strings = value.split(key);
    }
    return strings;
}

bool MusicUtils::String::isChinese(const QChar &c)
{
#ifdef Q_CC_MSVC
    return '\xa9\x96' == c || (c.unicode() >= 0x4e00 && c.unicode() <= 0x9fa5);
#else
    return L'〇' == c || (c.unicode() >= 0x4e00 && c.unicode() <= 0x9fa5);
#endif
}

QString MusicUtils::String::artistName(const QString &value, const QString &key)
{
    const QStringList &s = splitString(value);
    if(s.count() >= 2)
    {
        if(M_SETTING_PTR->value(MusicSettingManager::OtherSongFormat).toInt() == 0)
        {
            const int index = value.indexOf(key);
            return value.left(index).trimmed();
        }
        else
        {
            const int index = value.indexOf(key) + 1;
            return value.right(value.length() - index).trimmed();
        }
    }
    return value;
}

QString MusicUtils::String::songName(const QString &value, const QString &key)
{
    const QStringList &s = splitString(value);
    if(s.count() >= 2)
    {
        if(M_SETTING_PTR->value(MusicSettingManager::OtherSongFormat).toInt() == 0)
        {
            const int index = value.indexOf(key) + 1;
            return value.right(value.length() - index).trimmed();
        }
        else
        {
            const int index = value.indexOf(key);
            return value.left(index).trimmed();
        }
    }
    return value;
}

QStringList MusicUtils::String::illegalCharacters()
{
    QStringList acs;
    acs << "\\" << "/" << "?"  << "*" << "\"" << ":" << "<" << ">" << "|";
    return acs;
}

bool MusicUtils::String::illegalCharactersCheck(const QString &value)
{
    const QStringList acs(illegalCharacters());

    foreach(const QString &ac, acs)
    {
        if(value.contains(ac))
        {
            return true;
        }
    }

    return false;
}

QString MusicUtils::String::illegalCharactersReplaced(const QString &value)
{
    const QStringList acs(illegalCharacters());
    QString s(value);

    foreach(const QString &ac, acs)
    {
        if(s.contains(ac))
        {
            s.replace(ac, " ");
        }
    }

    return s;
}

QList<QColor> MusicUtils::String::readColorConfig(const QString &value)
{
    QList<QColor> colors;
    const QStringList &rgbs = value.split(';', QString::SkipEmptyParts);
    foreach(const QString &rgb, rgbs)
    {
        const QStringList &var = rgb.split(',');
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
    return QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
}

QString MusicUtils::String::writeColorConfig(const QList<QColor> &colors)
{
    QString value;
    foreach(const QColor &rgb, colors)
    {
        value.append(writeColorConfig(rgb) + ";");
    }
    return value;
}
