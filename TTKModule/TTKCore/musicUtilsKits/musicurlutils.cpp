#include "musicurlutils.h"

#include <QUrl>
#include <QProcess>
#include <QDesktopServices>
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <shellapi.h>
#endif

const MString ASCII_MAP(R"("%<>[\]^_`{|})");
const MString HEX_NUM_MAP("0123456789ABCDEF");

unsigned char number2Char(const char h, const char l)
{
  unsigned char hh = std::find(std::begin(HEX_NUM_MAP), std::end(HEX_NUM_MAP), h) - std::begin(HEX_NUM_MAP);
  unsigned char ll = std::find(std::begin(HEX_NUM_MAP), std::end(HEX_NUM_MAP), l) - std::begin(HEX_NUM_MAP);
  return (hh << 4) + ll;
}

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
        p.replace('/', "\\");
        p = "/select," + p;
        const HINSTANCE value = ShellExecuteW(0, L"open", L"explorer.exe", p.toStdWString().c_str(), nullptr, SW_SHOWNORMAL);
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

void MusicUtils::Url::urlPrettyEncode(QString &data)
{
    const MString &url = data.toStdString();
    data.clear();

    for(auto it = url.begin(); it != url.end(); ++it)
    {
      if(((*it >> 7) & 1) || (std::count(std::begin(ASCII_MAP), std::end(ASCII_MAP), *it)))
      {
        data.push_back('%');
        data.push_back(HEX_NUM_MAP[(*it >> 4) & 0x0F]);
        data.push_back(HEX_NUM_MAP[*it & 0x0F]);
      }
      else
      {
        data.push_back(*it);
      }
    }
}

void MusicUtils::Url::urlPrettyDecode(QString &data)
{
    const MString &url = data.toStdString();
    data.clear();

    for(auto it = url.begin(); it != url.end(); ++it)
    {
      if(*it == '%')
      {
        if(std::next(it++) == url.end())
        {
          M_LOGGER_ERROR("Url is invalid");
          return;
        }

        data.push_back(number2Char(*it, *std::next(it)));
        if(std::next(it++) == url.end())
        {
          M_LOGGER_ERROR("Url is invalid");
          return;
        }
      }
      else
      {
        data.push_back(*it);
      }
    }
}

void MusicUtils::Url::urlPrettyEncode(QByteArray &data)
{
    const MString url(data.constData(), data.length());
    data.clear();

    for(auto it = url.begin(); it != url.end(); ++it)
    {
      if(((*it >> 7) & 1) || (std::count(std::begin(ASCII_MAP), std::end(ASCII_MAP), *it)))
      {
        data.push_back('%');
        data.push_back(HEX_NUM_MAP[(*it >> 4) & 0x0F]);
        data.push_back(HEX_NUM_MAP[*it & 0x0F]);
      }
      else
      {
        data.push_back(*it);
      }
    }
}

void MusicUtils::Url::urlPrettyDecode(QByteArray &data)
{
    const MString url(data.constData(), data.length());
    data.clear();

    for(auto it = url.begin(); it != url.end(); ++it)
    {
      if(*it == '%')
      {
        if(std::next(it++) == url.end())
        {
          M_LOGGER_ERROR("Url is invalid");
          return;
        }

        data.push_back(number2Char(*it, *std::next(it)));
        if(std::next(it++) == url.end())
        {
          M_LOGGER_ERROR("Url is invalid");
          return;
        }
      }
      else
      {
        data.push_back(*it);
      }
    }
}
