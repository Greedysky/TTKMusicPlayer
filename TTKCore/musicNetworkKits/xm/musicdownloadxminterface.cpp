#include "musicdownloadxminterface.h"
#include "musiccryptographichash.h"
#include "musicnumberutils.h"
#include "musicsemaphoreloop.h"
#include "musictime.h"

#///QJson import
#include "qjson/parser.h"

#include <QNetworkCookie>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadXMInterface::makeTokenQueryUrl(QNetworkAccessManager *manager, QNetworkRequest *request,
                                                 const QString &query, const QString &type)
{
    if(!manager)
    {
        return;
    }

    QNetworkRequest re;
    re.setUrl(QUrl(MusicCryptographicHash::decryptData(XM_COOKIE_URL, URL_KEY)));
    QNetworkReply *reply = manager->get(re);
    MusicSemaphoreLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QList<QNetworkCookie> cookies = QNetworkCookie::parseCookies(reply->rawHeader("Set-Cookie"));
    QString tk = cookies[0].value();
    QString tk_enc = cookies[1].value();

    QString time = QString::number(MusicTime::timeStamp());
    QString appkey = "12574478";
    QString token = tk.split("_").front();
    QString data = MusicCryptographicHash::decryptData(XM_QUERY_DATA_URL, URL_KEY).arg(query);
    QString sign = QCryptographicHash::hash((token + "&" + time + "&" + appkey + "&" + data).toUtf8(), QCryptographicHash::Md5).toHex();

    request->setUrl(QUrl(MusicCryptographicHash::decryptData(XM_QUERY_URL, URL_KEY).arg(type).arg(time).arg(appkey).arg(sign).arg(data)));
    request->setRawHeader("Cookie", QString("_m_h5_tk=%1; _m_h5_tk_enc=%2").arg(tk).arg(tk_enc).toUtf8());
}

void MusicDownLoadXMInterface::readFromMusicSongLrc(MusicObject::MusicSongInfomation *info, QNetworkAccessManager *manager,
                                                    const QString &songID)
{
    if(!manager)
    {
        return;
    }

    QNetworkRequest request;
    makeTokenQueryUrl(manager, &request,
                      MusicCryptographicHash::decryptData(XM_LRC_DATA_URL, URL_KEY).arg(songID),
                      MusicCryptographicHash::decryptData(XM_LRC_URL, URL_KEY));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager->get( request );
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    QByteArray bytes = reply->readAll();
    bytes = bytes.replace("xiami(", "");
    bytes = bytes.replace("callback(", "");
    bytes.chop(1);

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("data"))
        {
            value = value["data"].toMap();
            value = value["data"].toMap();
            QVariantList datas = value["lyrics"].toList();
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

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    attr.m_url = key["listenFile"].toString();
    attr.m_size = MusicUtils::Number::size2Label(key["fileSize"].toInt());
    attr.m_format = key["format"].toString();
    attr.m_bitrate = bitrate;
    info->m_songAttrs.append(attr);
}

void MusicDownLoadXMInterface::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                          const QVariant &key, const QString &quality, bool all)
{
    foreach(const QVariant &song, key.toList())
    {
        QVariantMap data = song.toMap();
        int bitrate = map2NormalBitrate(data["quality"].toString());

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

int MusicDownLoadXMInterface::map2NormalBitrate(const QString &bitrate)
{
    if(bitrate == "e")
        return MB_32;
    else if(bitrate == "f")
        return MB_128;
    else if(bitrate == "l")
        return MB_192;
    else if(bitrate == "h")
        return MB_320;
    else if(bitrate == "s")
        return MB_500;
    else
        return MB_128;
}
