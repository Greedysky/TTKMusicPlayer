#include "musicwyqueryinterface.h"
#include "musicsemaphoreloop.h"
#include "musicnumberutils.h"
#include "musicalgorithmutils.h"
#include "musicurlutils.h"
#include "musicsettingmanager.h"
#include "musicabstractnetwork.h"

#include "qalg/qaeswrap.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

void MusicWYQueryInterface::makeTokenQueryRequest(QNetworkRequest *request)
{
    request->setRawHeader("Referer", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("Origin", MusicUtils::Algorithm::mdII(WY_BASE_URL, false).toUtf8());
    request->setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WY_UA_URL, ALG_UA_KEY, false).toUtf8());

    QString cookie = M_SETTING_PTR->value(MusicSettingManager::NetworkCookie).toString();
    cookie = cookie.isEmpty() ? MusicUtils::Algorithm::mdII(WY_COOKIE_URL, false) : cookie;
    request->setRawHeader("Cookie", QString("MUSIC_U=%1; NMTID=%2; ").arg(cookie)
                                            .arg(MusicUtils::Algorithm::mdII(WY_NMTID_URL, ALG_UA_KEY, false))
                                            .toUtf8());
    MusicObject::setSslConfiguration(request);
}

QByteArray MusicWYQueryInterface::makeTokenQueryUrl(QNetworkRequest *request, const QString &query, const QString &type)
{
    QAesWrap aes;
    QByteArray parameter = aes.encryptCBC(type.toUtf8(), "0CoJUm6Qyw8W8jud", "0102030405060708");
    parameter = aes.encryptCBC(parameter, "a44e542eaac91dce", "0102030405060708");
    MusicUtils::Url::urlEncode(parameter);

    request->setUrl(query);
    makeTokenQueryRequest(request);

    return "params=" + parameter + "&encSecKey=" + WY_SECKRY_STRING.toUtf8();
}

void MusicWYQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, int bitrate)
{
    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(WY_SONG_INFO_OLD_URL, false).arg(bitrate*1000).arg(info->m_songId));
    makeTokenQueryRequest(&request);

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
    const QVariant &data = parser.parse(reply->readAll(), &ok);
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
            attr.m_bitrate = bitrate;

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

void MusicWYQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate, const QString &suffix)
{
    const qint64 dfsId = /*key.value("dfsId").toLongLong()*/0;
    if(key.isEmpty() || dfsId == 0)
    {
        readFromMusicSongAttribute(info, bitrate);
    }
    else
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = key.value("extension").toString();
        if(attr.m_format.isEmpty())
        {
            attr.m_format = suffix;
        }
        attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
        attr.m_url = MusicUtils::Algorithm::mdII(WY_SONG_PATH_OLD_URL, false).arg(encryptedId(dfsId)).arg(dfsId);
        info->m_songAttrs.append(attr);
    }
}

void MusicWYQueryInterface::readFromMusicSongAttribute(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
{
    int maxBr = MB_1000;
    const QVariantMap &privilege = key["privilege"].toMap();
    if(!privilege.isEmpty())
    {
        const QString &brStr = privilege["maxbr"].toString();
        if(brStr == "999000")
        {
            maxBr = MB_1000;
        }
        else if(brStr == "320000" || brStr == "192000" || brStr == "190000")
        {
            maxBr = MB_320;
        }
        else if(brStr == "160000")
        {
            maxBr = MB_192;
        }
        else
        {
            maxBr = MB_128;
        }
    }

    if(all)
    {
        if(maxBr == MB_1000 || maxBr == MB_320)
        {
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128, MP3_FILE_PREFIX);
            readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192, MP3_FILE_PREFIX);
            readFromMusicSongAttribute(info, key["hMusic"].toMap(), MB_320, MP3_FILE_PREFIX);
            readFromMusicSongAttribute(info, key["fMusic"].toMap(), MB_1000, FLC_FILE_PREFIX);
        }
        else if(maxBr == MB_192)
        {
            readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192, MP3_FILE_PREFIX);
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128, MP3_FILE_PREFIX);
        }
        else
        {
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128, MP3_FILE_PREFIX);
        }
    }
    else
    {
        if(quality == QObject::tr("SD") && maxBr >= MB_128)
        {
            readFromMusicSongAttribute(info, key["bMusic"].toMap(), MB_128, MP3_FILE_PREFIX);
        }
        else if(quality == QObject::tr("HQ") && maxBr >= MB_192)
        {
            readFromMusicSongAttribute(info, key["mMusic"].toMap(), MB_192, MP3_FILE_PREFIX);
        }
        else if(quality == QObject::tr("SQ") && maxBr >= MB_320)
        {
            readFromMusicSongAttribute(info, key["hMusic"].toMap(), MB_320, MP3_FILE_PREFIX);
        }
        else if(quality == QObject::tr("CD") && maxBr >= MB_1000)
        {
            readFromMusicSongAttribute(info, key["fMusic"].toMap(), MB_1000, FLC_FILE_PREFIX);
        }
    }
}

void MusicWYQueryInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, int bitrate)
{
    QNetworkRequest request;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_SONG_PATH_URL, false),
                      MusicUtils::Algorithm::mdII(WY_SONG_PATH_DATA_URL, false).arg(info->m_songId).arg(bitrate*1000));

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
    const QVariant &data = parser.parse(reply->readAll(), &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value["code"].toInt() == 200 && value.contains("data"))
        {
            const QVariantList &datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                MusicObject::MusicSongAttribute attr;
                attr.m_url = value["url"].toString();
                attr.m_bitrate = bitrate;
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

void MusicWYQueryInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, int bitrate, const QString &suffix)
{
    const qint64 fid = /*key.value("fid").toLongLong()*/0;
    if(key.isEmpty() || fid == 0)
    {
        readFromMusicSongAttribute(info, bitrate);
    }
    else
    {
        MusicObject::MusicSongAttribute attr;
        attr.m_bitrate = bitrate;
        attr.m_format = key.value("extension").toString();
        if(attr.m_format.isEmpty())
        {
            attr.m_format = suffix;
        }
        attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
        attr.m_url = MusicUtils::Algorithm::mdII(WY_SONG_PATH_OLD_URL, false).arg(encryptedId(fid)).arg(fid);
        info->m_songAttrs.append(attr);
    }
}

void MusicWYQueryInterface::readFromMusicSongAttributeNew(MusicObject::MusicSongInformation *info, const QVariantMap &key, const QString &quality, bool all)
{
    int maxBr = MB_1000;
    const QVariantMap &privilege = key["privilege"].toMap();
    if(!privilege.isEmpty())
    {
        const QString &brStr = privilege["maxbr"].toString();
        if(brStr == "999000")
        {
            maxBr = MB_1000;
        }
        else if(brStr == "320000" || brStr == "192000" || brStr == "190000")
        {
            maxBr = MB_320;
        }
        else if(brStr == "160000")
        {
            maxBr = MB_192;
        }
        else
        {
            maxBr = MB_128;
        }
    }

    if(all)
    {
        if(maxBr == MB_1000 || maxBr == MB_320)
        {
            readFromMusicSongAttributeNew(info, key["l"].toMap(), MB_128, MP3_FILE_PREFIX);
            readFromMusicSongAttributeNew(info, key["m"].toMap(), MB_192, MP3_FILE_PREFIX);
            readFromMusicSongAttributeNew(info, key["h"].toMap(), MB_320, MP3_FILE_PREFIX);
            readFromMusicSongAttributeNew(info, key["f"].toMap(), MB_1000, FLC_FILE_PREFIX);
        }
        else if(maxBr == MB_192)
        {
            readFromMusicSongAttributeNew(info, key["m"].toMap(), MB_192, MP3_FILE_PREFIX);
            readFromMusicSongAttributeNew(info, key["l"].toMap(), MB_128, MP3_FILE_PREFIX);
        }
        else
        {
            readFromMusicSongAttributeNew(info, key["l"].toMap(), MB_128, MP3_FILE_PREFIX);
        }
    }
    else
    {
        if(quality == QObject::tr("SD") && maxBr >= MB_128)
        {
            readFromMusicSongAttributeNew(info, key["l"].toMap(), MB_128, MP3_FILE_PREFIX);
        }
        else if(quality == QObject::tr("HQ") && maxBr >= MB_192)
        {
            readFromMusicSongAttributeNew(info, key["m"].toMap(), MB_192, MP3_FILE_PREFIX);
        }
        else if(quality == QObject::tr("SQ") && maxBr >= MB_320)
        {
            readFromMusicSongAttributeNew(info, key["h"].toMap(), MB_320, MP3_FILE_PREFIX);
        }
        else if(quality == QObject::tr("CD") && maxBr >= MB_1000)
        {
            readFromMusicSongAttributeNew(info, key["f"].toMap(), MB_1000, FLC_FILE_PREFIX);
        }
    }
}

QString MusicWYQueryInterface::encryptedId(qint64 id)
{
    return encryptedId(QString::number(id));
}

QString MusicWYQueryInterface::encryptedId(const QString &string)
{
    const QByteArray array1(WY_ENCRYPT_STRING.toUtf8());
    QByteArray array2 = string.toUtf8();
    int length = array1.length();
    for(int i=0; i<array2.length(); ++i)
    {
        array2[i] = array2[i]^array1[i%length];
    }

    QByteArray encodedData = MusicUtils::Algorithm::md5(array2);
#if TTK_QT_VERSION_CHECK(5,2,0)
    encodedData = encodedData.toBase64(QByteArray::Base64UrlEncoding);
#else
    encodedData = encodedData.toBase64();
    encodedData.replace("+", "-");
    encodedData.replace("/", "_");
#endif
    return encodedData;
}
