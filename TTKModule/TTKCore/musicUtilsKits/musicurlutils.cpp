#include "musicurlutils.h"
#include "ttkplatformsystem.h"

#include <QUrl>
#include <QProcess>
#include <QDesktopServices>
#ifdef Q_OS_WIN
#  define WIN32_LEAN_AND_MEAN
#  include <qt_windows.h>
#  include <shellapi.h>
#endif

bool TTK::Url::execute(const QString &path)
{
#ifdef Q_OS_WIN
    ShellExecuteW(0, L"open", path.toStdWString().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    return true;
#else
    return QProcess::startDetached(path, {});
#endif
}

bool TTK::Url::openUrl(const QString &path, bool local)
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
        if(TTKPlatformSystem::systemName() == TTKPlatformSystem::System::LinuxUbuntu)
        {
            return QProcess::startDetached("nautilus", {path});
        }
#endif
    }
    return QDesktopServices::openUrl(local ? QUrl::fromLocalFile(path) : QUrl(path, QUrl::TolerantMode));
}

QString TTK::Url::urlEncode(QString &data)
{
    data.replace("+", "%2B");
    data.replace("/", "%2F");
    data.replace("=", "%3D");
    return data;
}

QString TTK::Url::urlDecode(QString &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
    return data;
}

QByteArray TTK::Url::urlEncode(QByteArray &data)
{
    data.replace("+", "%2B");
    data.replace("/", "%2F");
    data.replace("=", "%3D");
    return data;
}

QByteArray TTK::Url::urlDecode(QByteArray &data)
{
    data.replace("%2B", "+");
    data.replace("%2F", "/");
    data.replace("%3D", "=");
    return data;
}

QString TTK::Url::urlPrettyEncode(QString &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl(data).toString(QUrl::FullyEncoded);
#else
    data = QUrl(data).toEncoded();
#endif
    return data;
}

QString TTK::Url::urlPrettyDecode(QString &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl(data).toString();
#else
    data = QByteArray::fromPercentEncoding(data.toUtf8());
#endif
    return data;
}

QByteArray TTK::Url::urlPrettyEncode(QByteArray &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl(data).toString(QUrl::FullyEncoded).toUtf8();
#else
    data = QUrl(data).toEncoded();
#endif
    return data;
}

QByteArray TTK::Url::urlPrettyDecode(QByteArray &data)
{
#if TTK_QT_VERSION_CHECK(5,0,0)
    data = QUrl::fromEncoded(data).toString(QUrl::FullyDecoded).toUtf8();
#else
    data = QByteArray::fromPercentEncoding(data);
#endif
    return data;
}
