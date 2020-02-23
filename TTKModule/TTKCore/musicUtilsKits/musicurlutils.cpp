#include "musicurlutils.h"

#include <QUrl>
#include <QProcess>
#include <QDesktopServices>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <shellapi.h>
#endif

bool MusicUtils::Url::openUrl(const QString &exe, const QString &path)
{
#ifdef Q_OS_WIN
    const HINSTANCE value = ShellExecuteW(0, exe.toStdWString().c_str(), path.toStdWString().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
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
        p.replace("/", "\\");
        p = "/select," + p;
        const HINSTANCE value = ShellExecuteW(0, L"open", L"explorer.exe", p.toStdWString().c_str(), nullptr, SW_SHOWNORMAL);
        return (int)value >= 32;
    }
#endif
    return QDesktopServices::openUrl(local ? QUrl::fromLocalFile(path) : QUrl(path, QUrl::TolerantMode));
}

void MusicUtils::Url::urlEncode(QString &data)
{
    data.replace("+", "%2B");
    data.replace("/", "%2F");
    data.replace("=", "%3D");
}

void MusicUtils::Url::urlDecode(QString &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
}

void MusicUtils::Url::urlEncode(QByteArray &data)
{
    data.replace("+", "%2B");
    data.replace("/", "%2F");
    data.replace("=", "%3D");
}

void MusicUtils::Url::urlDecode(QByteArray &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
}

void MusicUtils::Url::urlPrettyEncode(QString &data)
{
#ifdef TTK_GREATER_NEW
    data = QUrl(data).toString(QUrl::FullyEncoded);
#else
    data = QUrl(data).toEncoded();
#endif
}

void MusicUtils::Url::urlPrettyDecode(QString &data)
{
#ifdef TTK_GREATER_NEW
    data = QUrl(data).toString();
#else
    data = QByteArray::fromPercentEncoding(data.toUtf8());
#endif
}

void MusicUtils::Url::urlPrettyEncode(QByteArray &data)
{
#ifdef TTK_GREATER_NEW
    data = QUrl(data).toString(QUrl::FullyEncoded).toUtf8();
#else
    data = QUrl(data).toEncoded();
#endif
}

void MusicUtils::Url::urlPrettyDecode(QByteArray &data)
{
#ifdef TTK_GREATER_NEW
    data = QUrl::fromEncoded(data).toString(QUrl::FullyDecoded).toUtf8();
#else
    data = QByteArray::fromPercentEncoding(data);
#endif
}
