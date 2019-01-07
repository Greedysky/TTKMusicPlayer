#include "musicdownloadxminterface.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"
#include "musicalgorithmutils.h"
#include "musicnetworkabstract.h"
#///QJson import
#include "qjson/parser.h"

#include <QNetworkCookie>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

bool MusicDownLoadXMInterface::makeTokenQueryCookies(QString &tk, QString &tke)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(XM_COOKIE_URL, false)));
    const QString &time = QString::number(MusicTime::timeStamp());
    const QString &appkey = "12574478";
    const QString &sign = MusicUtils::Algorithm::md5((time + "&" + appkey).toUtf8()).toHex();
    const QString &base = MusicUtils::Algorithm::mdII(XM_COOKIE_DATA_URL, false);

    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(XM_QUERY_URL, false).arg(base).arg(time).arg(appkey).arg(sign)));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(XM_UA_URL_1, ALG_UA_KEY, false).toUtf8());

    QNetworkReply *reply = manager.get(request);
    MusicSemaphoreLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return false;
    }

    const QList<QNetworkCookie> &cookies = QNetworkCookie::parseCookies(reply->rawHeader("Set-Cookie"));
    if(cookies.count() >= 2)
    {
        tk = cookies[0].value();
        tke = cookies[1].value();
        return true;
    }
    return false;
}

void MusicDownLoadXMInterface::makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type)
{
    QString tk, tke;
    if(!makeTokenQueryCookies(tk, tke))
    {
        return;
    }

    const QString time = QString::number(MusicTime::timeStamp());
    const QString appkey = "12574478";
    const QString token = tk.split("_").front();
    const QString data = MusicUtils::Algorithm::mdII(XM_QUERY_DATA_URL, false).arg(query);
    const QString encode = QString("%1&%2&%3&%4").arg(token).arg(time).arg(appkey).arg(data);
    const QString sign = MusicUtils::Algorithm::md5(encode.toUtf8()).toHex();

    request->setUrl(QUrl(MusicUtils::Algorithm::mdII(XM_QUERY_URL, false).arg(type).arg(time).arg(appkey).arg(sign).arg(data)));
    request->setRawHeader("Cookie", QString("_m_h5_tk=%1; _m_h5_tk_enc=%2").arg(tk).arg(tke).toUtf8());
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(XM_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(request);
}

void MusicDownLoadXMInterface::readFromMusicSongLrc(MusicObject::MusicSongInformation *info)
{
    QNetworkAccessManager manager;
    QNetworkRequest request;
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_LRC_DATA_URL, false).arg(info->m_songId),
                      MusicUtils::Algorithm::mdII(XM_LRC_URL, false));

    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    const QByteArray &bytes = reply->readAll();

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            value = value["data"].toMap();
            const QVariantList &datas = value["lyrics"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                if(value["type"].toInt() == 2)
                {
                    info->m_lrcUrl = value["lyricUrl"].toString();
                    break;
                }
            }
        }
    }
}

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["listenFile"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["fileSize"].toInt());
    attr.m_format = key["format"].toString();
    attr.m_bitrate = bitrate;

    if(attr.m_url.isEmpty())
    {
        attr.m_url = key["url"].toString();
    }
    if(key["fileSize"].toInt() == 0)
    {
        attr.m_size = MusicUtils::Number::size2Label(key["filesize"].toInt());
    }

    info->m_songAttrs.append(attr);
}

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariant &key, const QString &quality, bool all)
{
    foreach(const QVariant &song, key.toList())
    {
        const QVariantMap &data = song.toMap();
        const int bitrate = MusicUtils::Number::transfromBitrateToNormal(data["quality"].toString());

        if(all)
        {
            readFromMusicSongAttribute(info, data, bitrate);
        }
        else
        {
            if(quality == QObject::tr("ST") && bitrate == MB_32)
            {
                readFromMusicSongAttribute(info, data, MB_32);
            }
            else if(quality == QObject::tr("SD") && bitrate == MB_128)
            {
                readFromMusicSongAttribute(info, data, MB_128);
            }
            else if(quality == QObject::tr("HQ") && bitrate == MB_192)
            {
                readFromMusicSongAttribute(info, data, MB_192);
            }
            else if(quality == QObject::tr("SQ") && bitrate == MB_320)
            {
                readFromMusicSongAttribute(info, data, MB_320);
            }
            else if(quality == QObject::tr("CD") && bitrate == MB_500)
            {
                readFromMusicSongAttribute(info, data, MB_500);
            }
        }
    }
}
