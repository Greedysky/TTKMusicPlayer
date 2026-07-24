#include "musicunityqueryinterface.h"

// GET
static constexpr const char *QUERY_MODULE__ = "*";
static constexpr const char *QUERY_MODULE_A = "A";
static constexpr const char *QUERY_MODULE_B = "B";
static constexpr const char *QUERY_MODULE_C = "C";
static constexpr const char *QUERY_MODULE_D = "D";
// POST
static constexpr const char *QUERY_MODULE_Y = "Y";
static constexpr const char *QUERY_MODULE_Z = "Z";
//
static constexpr const char *QUERY_PLUGINS_URL = "plugins";

struct Data
{
    QString m_ua;
    QString m_url;
    QString m_body;
    QString m_quality;
    QString m_module;
};

static bool checkModule(const QString &module)
{
    return module == QUERY_MODULE__ || module == QUERY_MODULE_A || module == QUERY_MODULE_B ||
           module == QUERY_MODULE_C || module == QUERY_MODULE_D ||
           module == QUERY_MODULE_Y || module == QUERY_MODULE_Z;
}

static void parseSongFileSize(const QVariantMap &value, TTK::MusicSongProperty &prop)
{
    if(value.contains("size"))
    {
        const QVariant &object = value["size"];
        switch(QtVariantType(object))
        {
            case QMetaType::QString: prop.m_size = object.toString(); break;
            default: prop.m_size = TTK::Number::sizeByteToLabel(object.toInt()); break;
        }
    }
}

static void parseSongProperty(const QVariantMap &value, TTK::MusicSongProperty &prop)
{
    prop.m_url = value["url"].toString();

    parseSongFileSize(value, prop);

    if(prop.isEmpty())
    {
        const QVariant &object = value["data"];
        switch(QtVariantType(object))
        {
            case QMetaType::QString:
            {
                prop.m_url = object.toString();
                break;
            }
            case QMetaType::QVariantMap:
            {
                const QVariantMap &dataObject = object.toMap();
                prop.m_url = dataObject["url"].toString();

                parseSongFileSize(value, prop);

                if(prop.isEmpty())
                {
                    prop.m_url = dataObject["data"].toString();
                }
                break;
            }
            default: break;
        }
    }
}


/*
Request: HTTP GET XXXXX/%1/%2/%3
Response:
{
  "code": 0, // or "code": 200
  "data": "xx", // or "url": "xx"
  ("size"): "xx"
}
or
{
  "code": 0, // or "code": 200
  "data": {
    "data": "xx", // or "url": "xx"
    ("size"): "xx"
  }
}
*/
static void parseSongProperties(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            parseSongProperty(value, prop);

            if(!prop.isEmpty())
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

/*
Request: HTTP GET XXXXX/%1/%2/%3
Response:
{
  "code": 0, // or "code": 200
  "data": "xx", // or "url": "xx"
  ("size"): "xx",
  "extra": {
    "quality": {
      "target": "xx",
      "result": "xx"
    }
  }
}
or
{
  "code": 0, // or "code": 200
  "data": "xx", // or "url": "xx"
  ("size"): "xx",
  "quality": {
    "target": "xx",
    "result": "xx"
  }
}
or
{
  "code": 0, // or "code": 200
  "data": {
    "data": "xx", // or "url": "xx"
    ("size"): "xx"
  }
  "extra": {
    "quality": {
      "target": "xx",
      "result": "xx"
    }
  }
}
or
{
  "code": 0, // or "code": 200
  "data": {
    "data": "xx", // or "url": "xx"
    ("size"): "xx"
  }
  "quality": {
    "target": "xx",
    "result": "xx"
  }
}
*/
static void parseSongPropertyA(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate, const QString &quality)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            parseSongProperty(value, prop);

            const QVariantMap &extra = value["extra"].toMap();
            if(!extra.isEmpty())
            {
                value = extra;
            }

            const QVariantMap &q = value["quality"].toMap();
            if(q["target"].toString().contains(quality, Qt::CaseInsensitive) &&
               q["result"].toString().contains(quality, Qt::CaseInsensitive) && !prop.isEmpty())
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

/*
Request: HTTP GET XXXXX/%1/%2/%3
Response:
{
  "data": "xx", // or "url": "xx"
  ("size"): "xx"
}
or
{
  "data": {
    "data": "xx", // or "url": "xx"
    ("size"): "xx"
  }
}
*/
static void parseSongPropertyB(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value.contains("url") || value.contains("data"))
        {
            TTK::MusicSongProperty prop;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            parseSongProperty(value, prop);

            if(!prop.isEmpty())
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

/*
Request: HTTP GET XXXXX/%1/%2/%3
Response:
url
*/
static void parseSongPropertyC(QNetworkRequest *request, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForGet(request);
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = true;
    TTK::MusicSongProperty prop;
    prop.m_url = bytes;
    prop.m_size = TTK_DEFAULT_STR;
    prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
    prop.m_bitrate = bitrate;
    info->m_songProps.append(prop);

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

/*
Request: HTTP POST XXXXX
              BODY /%1/%2/%3
Response:
{
  "code": 0, // or "code": 200
  "data": "xx", // or "url": "xx"
  ("size"): "xx"
}
or
{
  "code": 0, // or "code": 200
  "data": {
    "data": "xx", // or "url": "xx"
    ("size"): "xx"
  }
}
*/
static void parseSongPropertyZ(QNetworkRequest *request, const QString &body, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForPost(request, body.toUtf8());
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            parseSongProperty(value, prop);

            if(!prop.isEmpty())
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}

/*
Request: HTTP POST XXXXX/%1
              BODY /%1/%2/%3
Response:
{
  "code": 0, // or "code": 200
  "data": "xx", // or "url": "xx"
  ("size"): "xx"
}
or
{
  "code": 0, // or "code": 200
  "data": {
    "data": "xx", // or "url": "xx"
    ("size"): "xx"
  }
}
*/
static void parseSongPropertyY(QNetworkRequest *request, const QString &body, TTK::MusicSongInformation *info, int bitrate)
{
    const QByteArray &bytes = TTK::syncNetworkQueryForPost(request, body.toUtf8());
    if(bytes.isEmpty())
    {
        return;
    }

    bool found = false;
    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 0 || value["code"].toInt() == 200)
        {
            TTK::MusicSongProperty prop;
            prop.m_size = TTK_DEFAULT_STR;
            prop.m_format = bitrate > TTK_BN_320 ? FLAC_FILE_SUFFIX : MP3_FILE_SUFFIX;
            prop.m_bitrate = bitrate;

            parseSongProperty(value, prop);

            if(!prop.isEmpty())
            {
                found = true;
                info->m_songProps.append(prop);
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found metainfo, buffer is" << bytes);
    }
}


void ReqUnityInterface::parseFromSongProperty(TTK::MusicSongInformation *info, const QString &type, const QString &id, int bitrate)
{
    QByteArray bytes;
    QFile file(RESOURCE_DIR_FULL + QUERY_PLUGINS_URL);
    if(file.open(QIODevice::ReadOnly))
    {
        bytes = file.readAll();
        file.close();
    }
    else
    {
        TTK_ERROR_STREAM("Load server unity plugins resource config failed");
        return;
    }

    bool found = false;
    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        const QVariantList &servers = json.toVariant().toList();
        for(const QVariant &var : qAsConst(servers))
        {
            if(var.isNull())
            {
                continue;
            }

            const QVariantMap &server = var.toMap();
            const bool option = server["option"].toBool();

            if(!option)
            {
                continue;
            }

            found = false;
            for(const TTK::MusicSongProperty &prop : qAsConst(info->m_songProps))
            {
                if(prop.m_bitrate == bitrate)
                {
                    found = true;
                    break;
                }
            }

            if(found)
            {
                break;
            }

            const QVariantList &modules = server["modules"].toList();
            for(const QVariant &var : qAsConst(modules))
            {
                if(var.isNull())
                {
                    continue;
                }

                const QVariantMap &module = var.toMap();
                const QVariantMap &serverMap = module["server"].toMap();
                const QVariantMap &qualityMap = module["quality"].toMap();

                if(!serverMap.contains(type) || !qualityMap.contains(QString::number(bitrate)))
                {
                    continue;
                }

                Data v;
                v.m_ua = module["ua"].toString();
                v.m_quality = qualityMap[QString::number(bitrate)].toString();
                v.m_module = module["module"].toString();

                QNetworkRequest request;
                if(module.contains("body"))
                {
                    TTK::setContentTypeHeader(&request, "application/json");
                    v.m_url = TTK::Algorithm::mdII(module["base"].toString(), false);
                    v.m_body = TTK::Algorithm::mdII(module["body"].toString(), false).arg(serverMap[type].toString(), id, v.m_quality);
                }
                else
                {
                    v.m_url = TTK::Algorithm::mdII(module["base"].toString(), false).arg(serverMap[type].toString(), id, v.m_quality);
                }

                if(v.m_url.isEmpty())
                {
                    break;
                }

                request.setUrl(v.m_url);
                TTK::setUserAgentHeader(&request);
                TTK::setSslConfiguration(&request);

                const QVariantMap &headerMap = module["headers"].toMap();
                if(!headerMap.isEmpty())
                {
                    for(const QString &header : headerMap.keys())
                    {
                        request.setRawHeader(header.toUtf8(), TTK::Algorithm::mdII(headerMap[header].toString(), false).toUtf8());
                    }
                }

                found = true;
                if(!v.m_ua.isEmpty())
                {
                    TTK::setUserAgentHeader(&request, TTK::Algorithm::mdII(v.m_ua, false).toUtf8());
                }

                if(v.m_module == QUERY_MODULE__)
                {
                    TTK_INFO_STREAM("Parse song in * module, url:" << v.m_url);
                    parseSongProperties(&request, info, bitrate);
                }
                else if(v.m_module == QUERY_MODULE_A)
                {
                    TTK_INFO_STREAM("Parse song in A module, url:" << v.m_url);
                    parseSongPropertyA(&request, info, bitrate, v.m_quality);
                }
                else if(v.m_module == QUERY_MODULE_B)
                {
                    TTK_INFO_STREAM("Parse song in B module, url:" << v.m_url);
                    parseSongPropertyB(&request, info, bitrate);
                }
                else if(v.m_module == QUERY_MODULE_C)
                {
                    TTK_INFO_STREAM("Parse song in C module, url:" << v.m_url);
                    parseSongPropertyC(&request, info, bitrate);
                }
                else if(v.m_module == QUERY_MODULE_Z)
                {
                    TTK_INFO_STREAM("Parse song in Z module, url:" << v.m_url << v.m_body);
                    parseSongPropertyZ(&request, v.m_body, info, bitrate);
                }
                else if(v.m_module == QUERY_MODULE_Y)
                {
                    v.m_url = v.m_url.arg(serverMap[type].toString());
                    request.setUrl(v.m_url);

                    TTK_INFO_STREAM("Parse song in Y module, url:" << v.m_url << v.m_body);
                    parseSongPropertyY(&request, v.m_body, info, bitrate);
                }
            }
        }
    }

    if(!found)
    {
        TTK_INFO_STREAM("Not found available server unity plugins");
    }
}
