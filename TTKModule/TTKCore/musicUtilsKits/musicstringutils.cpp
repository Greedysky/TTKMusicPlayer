#include "musicstringutils.h"
#include "musicsettingmanager.h"

#include <QColor>

QString MusicUtils::String::lrcPrefix()
{
    QString path = M_SETTING_PTR->value(MusicSettingManager::DownloadLrcPathDir).toString();
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
    QString path = M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDir).toString();
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

QString MusicUtils::String::stringPrefix(const QString &name)
{
    return stringPrefix(name, ".");
}

QString MusicUtils::String::stringPrefix(const QString &name, const QString &prefix)
{
    return name.left(name.indexOf(prefix));
}

QString MusicUtils::String::stringSuffix(const QString &name)
{
    return stringSuffix(name, ".");
}

QString MusicUtils::String::stringSuffix(const QString &name, const QString &suffix)
{
    return name.right(name.length() - name.lastIndexOf(suffix) - suffix.length());
}

QString MusicUtils::String::stringSplitToken(const QString &name)
{
    return stringSplitToken(name, ".", "?");
}

QString MusicUtils::String::stringSplitToken(const QString &name, const QString &prefix, const QString &suffix, bool revert)
{
    if(revert)
    {
        const QString &data = stringSuffix(name, prefix);
        return stringPrefix(data, suffix);
    }
    else
    {
        const QString &data = stringPrefix(name, prefix);
        return stringSuffix(data, suffix);
    }
}

QStringList MusicUtils::String::stringSplit(const QString &value, const QString &key)
{
    QStringList strings = value.split(QString(" %1 ").arg(key));
    if(strings.isEmpty() || strings.count() == 1)
    {
        strings = value.split(key);
    }
    return strings;
}

QString MusicUtils::String::removeStringToken(const QString &value, const QString &key)
{
    QString s = value;
    s.remove(key);
    if(s.contains(key))
    {
        s = removeStringToken(key);
    }
    return s;
}

QString MusicUtils::String::newlines()
{
#ifdef Q_OS_WIN
    return "\r\n";
#else
    return "\n";
#endif
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
    const QStringList &s = stringSplit(value);
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
    const QStringList &s = stringSplit(value);
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
    for(const QString &ac : qAsConst(acs))
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

    for(const QString &ac : qAsConst(acs))
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
#if TTK_QT_VERSION_CHECK(5,15,0)
    const QStringList &rgbs = value.split(";", Qt::SkipEmptyParts);
#else
    const QStringList &rgbs = value.split(";", QString::SkipEmptyParts);
#endif
    for(const QString &rgb : qAsConst(rgbs))
    {
        const QStringList &var = rgb.split(",");
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
    for(const QColor &rgb : qAsConst(colors))
    {
        value.append(writeColorConfig(rgb) + ";");
    }
    return value;
}
