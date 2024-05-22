#include "musicstringutils.h"
#include "musicsettingmanager.h"

#include <QTextDocument>

static QString makeFilePrefix(MusicSettingManager::Config type, const QString &path)
{
    QString dir = G_SETTING_PTR->value(type).toString();
    if(dir.isEmpty())
    {
        dir = QDir(path).canonicalPath();
        if(!dir.endsWith(TTK_SEPARATOR))
        {
            dir.append(TTK_SEPARATOR);
        }
    }

    if(!QDir(dir).exists())
    {
        QDir().mkpath(dir);
    }
    return dir;
}


QString TTK::String::lrcDirPrefix()
{
    return makeFilePrefix(MusicSettingManager::DownloadLrcDirPath, LRC_DIR_FULL);
}

QString TTK::String::musicDirPrefix()
{
    return makeFilePrefix(MusicSettingManager::DownloadMusicDirPath, MUSIC_DIR_FULL);
}

QString TTK::String::pefix(const QString &name)
{
    return TTK::String::pefix(name, TTK_DOT);
}

QString TTK::String::pefix(const QString &name, const QString &prefix)
{
    return name.left(name.indexOf(prefix));
}

QString TTK::String::suffix(const QString &name)
{
    return TTK::String::suffix(name, TTK_DOT);
}

QString TTK::String::suffix(const QString &name, const QString &suffix)
{
    return name.right(name.length() - name.lastIndexOf(suffix) - suffix.length());
}

QString TTK::String::splitToken(const QString &name)
{
    return TTK::String::splitToken(name, TTK_DOT, "?");
}

QString TTK::String::splitToken(const QString &name, const QString &prefix, const QString &suffix, bool revert)
{
    if(revert)
    {
        const QString &data = TTK::String::suffix(name, prefix);
        return TTK::String::pefix(data, suffix);
    }
    else
    {
        const QString &data = TTK::String::pefix(name, prefix);
        return TTK::String::suffix(data, suffix);
    }
}

QStringList TTK::String::split(const QString &value, const QString &key)
{
    QStringList strings = value.split(QString(" %1 ").arg(key));
    if(strings.isEmpty() || strings.count() == 1)
    {
        strings = value.split(key);
    }
    return strings;
}

QString TTK::String::removeToken(const QString &value, const QString &key)
{
    QString s = value;
    s.remove(key);
    if(s.contains(key))
    {
        s = TTK::String::removeToken(key);
    }
    return s;
}

bool TTK::String::isChinese(const QChar &c)
{
#ifdef Q_CC_MSVC
    return '\xa9\x96' == c || (c.unicode() >= 0x4e00 && c.unicode() <= 0x9fa5);
#else
    return L'〇' == c || (c.unicode() >= 0x4e00 && c.unicode() <= 0x9fa5);
#endif
}

bool TTK::String::isNetworkUrl(const QString &path)
{
    return path.startsWith(HTTP_PROTOCOL) || path.startsWith(HTTPS_PROTOCOL);
}

QString TTK::String::convertHtmlToPlain(const QString &value)
{
    QTextDocument text;
    text.setHtml(value);
    return text.toPlainText();
}

QStringList TTK::String::characters()
{
    static QStringList acs{"\\", "/", "?", "*", "\"", ":", "<", ">", "|"};
    return acs;
}

bool TTK::String::isCharValid(const QString &value)
{
    for(const QString &ac : TTK::String::characters())
    {
        if(value.contains(ac))
        {
            return true;
        }
    }
    return false;
}

QString TTK::String::charactersReplace(const QString &value)
{
    QString s(TTK::String::convertHtmlToPlain(value));
    // convert to html is failed, use origin value
    if(!value.isEmpty() && s.isEmpty())
    {
        s = value;
    }

    for(const QString &ac : TTK::String::characters())
    {
        if(s.contains(ac))
        {
            s.replace(ac, TTK_SPACE);
        }
    }
    return s;
}
