#include "musicurlutils.h"
#include "musicplatformmanager.h"

#include <QUrl>
#include <QProcess>
#include <QDesktopServices>
#ifdef Q_OS_WIN
#  include <qt_windows.h>
#  include <shellapi.h>
#endif

bool MusicUtils::Url::execute(const QString &path)
{
#ifdef Q_OS_WIN
    ShellExecuteW(0, L"open", path.toStdWString().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    return true;
#else
    return QProcess::startDetached(path, QStringList());
#endif
}

bool MusicUtils::Url::openUrl(const QString &path, bool local)
{
    if(path.isEmpty())
    {
        return false;
    }

    if(local)
    {
#ifdef Q_OS_WIN
        QString p = path;
        p.replace(TTK_SEPARATOR, "\\");
        p = "/select," + p;
        ShellExecuteW(0, L"open", L"explorer.exe", p.toStdWString().c_str(), nullptr, SW_SHOWNORMAL);
        return true;
#elif defined Q_OS_UNIX
        MusicPlatformManager platform;
        if(platform.systemName() == MusicPlatformManager::SystemLinuxUbuntu)
        {
            return QProcess::startDetached("nautilus", QStringList() << path);
        }
#endif
    }
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
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl(data).toString(QUrl::FullyEncoded);
#else
    data = QUrl(data).toEncoded();
#endif
}

void MusicUtils::Url::urlPrettyDecode(QString &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl(data).toString();
#else
    data = QByteArray::fromPercentEncoding(data.toUtf8());
#endif
}

void MusicUtils::Url::urlPrettyEncode(QByteArray &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl(data).toString(QUrl::FullyEncoded).toUtf8();
#else
    data = QUrl(data).toEncoded();
#endif
}

void MusicUtils::Url::urlPrettyDecode(QByteArray &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl::fromEncoded(data).toString(QUrl::FullyDecoded).toUtf8();
#else
    data = QByteArray::fromPercentEncoding(data);
#endif
}
