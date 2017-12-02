#include "musicdownloadwyinterface.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musicalgorithmutils.h"
#///QJson import
#include "qjson/parser.h"
#include "qaes/qaeswrap.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicDownLoadWYInterface::makeTokenQueryQequest(QNetworkRequest *request)
{
    request->setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request->setRawHeader("Cookie", MusicUtils::Algorithm::mdII(WY_COOKIE_URL, ALG_UA_KEY, false).toUtf8() + " appver=2.0.3.131777;");
    request->setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WY_UA_URL_1, ALG_UA_KEY, false).toUtf8());
}

QByteArray MusicDownLoadWYInterface::makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type)
{
    QByteArray parameter = QAesWrap("0CoJUm6Qyw8W8jud", "0102030405060708", QAesWrap::AES_128)
                           .encrypt(type.toUtf8(), QAesWrap::AES_CBC, QAesWrap::PKCS7);

    parameter = QAesWrap("a44e542eaac91dce", "0102030405060708", QAesWrap::AES_128)
                           .encrypt(parameter, QAesWrap::AES_CBC, QAesWrap::PKCS7);
    MusicUtils::Algorithm::urlEncode(parameter);

    request->setUrl(QUrl(query));
    makeTokenQueryQequest(request);

    return "params=" + parameter + "&encSecKey=" + WY_SECKRY.toUtf8();
}

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, int bitrate)
{
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_SONG_INFO_URL, false).arg(bitrate*1000).arg(info->m_songId);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    makeTokenQueryQequest(&request);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200 && value.contains("data"))
        {
            value = value["data"].toMap();
            if(value.isEmpty())
            {
                return;
            }

            MusicObject::MusicSongAttribute attr;
            attr.m_url = value["url"].toString();
            attr.m_bitrate = value["br"].toInt()/1000;

            if(attr.m_url.isEmpty())
            {
                readFromMusicSongAttributeNew(info, bitrate);
                return;
            }

            if(info->m_songAttrs.contains(attr))
            {
                return;
            }

            attr.m_size = MusicUtils::Number::size2Label(value["size"].toInt());
            attr.m_format = value["type"].toString();
            info->m_songAttrs.append(attr);
        }
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                                          const QVariantMap &key, int bitrate)
{
    qlonglong dfsId = key.value("dfsId").toLongLong();
    if(key.isEmpty() || dfsId == 0)
    {
        readFromMusicSongAttribute(info, bitrate);
    }
    else
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = key.value("extension").toString();
        attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
        attr.m_url = MusicUtils::Algorithm::mdII(WY_SONG_PATH_URL, false).arg(encryptedId(dfsId)).arg(dfsId);
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info,
                                                          const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128);
        readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192);
        readFromMusicSongAttribute(info, key["hMusic"].toMap(), MB_320);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttribute(info, key["hMusic"].toMap(), MB_320);
        }
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, int bitrate)
{
    QNetworkRequest request;
    QByteArray parameter = makeTokenQueryUrl(&request,
               MusicUtils::Algorithm::mdII(WY_SONG_INFO_N_URL, false),
               MusicUtils::Algorithm::mdII(WY_SONG_INFO_NDT_URL, false).arg(info->m_songId).arg(bitrate*1000));
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkAccessManager manager;
    MusicSemaphoreLoop loop;
    QNetworkReply *reply = manager.post(request, parameter);
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    loop.exec();

    if(!reply || reply->error() != QNetworkReply::NoError)
    {
        return;
    }

    QJson::Parser parser;
    bool ok;
    QVariant data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200 && value.contains("data"))
        {
            QVariantList datas = value["data"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["url"].toString();
                attr.m_bitrate = value["br"].toInt()/1000;
                if(attr.m_url.isEmpty() || info->m_songAttrs.contains(attr))
                {
                    break;
                }

                attr.m_size = MusicUtils::Number::size2Label(value["size"].toInt());
                attr.m_format = value["type"].toString();
                info->m_songAttrs.append(attr);
            }
        }
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info,
                                                             const QVariantMap &key, int bitrate)
{
    qlonglong fid = key.value("fid").toLongLong();
    if(key.isEmpty() || fid == 0)
    {
        readFromMusicSongAttribute(info, bitrate);
    }
    else
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = key.value("extension").toString();
        attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
        attr.m_url = MusicUtils::Algorithm::mdII(WY_SONG_PATH_URL, false).arg(encryptedId(fid)).arg(fid);
        info->m_songAttrs.append(attr);
    }
}

void MusicDownLoadWYInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info,
                                                             const QVariantMap &key, const QString &quality, bool all)
{
    if(all)
    {
        readFromMusicSongAttributeNew(info, key["l"].toMap(), MB_128);
        readFromMusicSongAttributeNew(info, key["m"].toMap(), MB_192);
        readFromMusicSongAttributeNew(info, key["h"].toMap(), MB_320);
    }
    else
    {
        if(quality == QObject::tr("SD"))
        {
            readFromMusicSongAttributeNew(info, key["l"].toMap(), MB_128);
        }
        else if(quality == QObject::tr("HQ"))
        {
            readFromMusicSongAttributeNew(info, key["m"].toMap(), MB_192);
        }
        else if(quality == QObject::tr("SQ"))
        {
            readFromMusicSongAttributeNew(info, key["h"].toMap(), MB_320);
        }
    }
}

QString MusicDownLoadWYInterface::encryptedId(qlonglong id)
{
    return encryptedId(QString::number(id));
}

QString MusicDownLoadWYInterface::encryptedId(const QString &string)
{
    QByteArray array1(WY_ENCRYPT_STRING.toUtf8());
    QByteArray array2 = string.toUtf8();
    int length = array1.length();
    for(int i=0; i<array2.length(); ++i)
    {
        array2[i] = array2[i]^array1[i%length];
    }

    QByteArray encodedData = MusicUtils::Algorithm::md5(array2);
#if (defined MUSIC_GREATER_NEW && !defined MUSIC_NO_WINEXTRAS)
    encodedData = encodedData.toBase64(QByteArray::Base64UrlEncoding);
#else
    encodedData = encodedData.toBase64();
    encodedData.replace('+', '-');
    encodedData.replace('/', '_');
#endif
    return encodedData;
}
