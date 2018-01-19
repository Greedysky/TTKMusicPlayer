#include "musicurlutils.h"

#include <QUrl>
#include <QProcess>
#include <QDesktopServices>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <shellapi.h>
#endif

bool MusicUtils::Url::openUrl(const QString &exe, const QString &path)
{
#ifdef Q_OS_WIN
    HINSTANCE value = ShellExecuteA(0, exe.toLocal8Bit(), path.toLocal8Bit(), nullptr, nullptr, SW_SHOWNORMAL);
    return (int)value >= 32;
#else
    Q_UNUSED(exe);
    return QProcess::startDetached(path, QStringList());
#endif
}

bool MusicUtils::Url::openUrl(const QString &path, bool local)
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
        HINSTANCE value = ShellExecuteA(0, "open", "explorer.exe", p.toLocal8Bit(), nullptr, SW_SHOWNORMAL);
        return (int)value >= 32;
    }
#else
    Q_UNUSED(local);
#endif
    return QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

void MusicUtils::Url::urlEncode(QString &data)
{
    data.replace('+', "%2B");
    data.replace('/', "%2F");
    data.replace('=', "%3D");
}

void MusicUtils::Url::urlDecode(QString &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
}

void MusicUtils::Url::urlEncode(QByteArray &data)
{
    data.replace('+', "%2B");
    data.replace('/', "%2F");
    data.replace('=', "%3D");
}

void MusicUtils::Url::urlDecode(QByteArray &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
}
