#include "musicblqueryinterface.h"
#include "musicabstractnetwork.h"

static constexpr const char *BL_MOVIE_DATA_URL = "ak5VTmcwemNhNXNpU1NCcXFTb28rWmo5TEdmMmtHNkh1ZUdHWWEveHFCWG80MnRyVkpHWHVNQ2VIN0dhUDJvOGRGVmdDei9paTFNPQ==";
static constexpr const char *BL_MOVIE_PLAY_URL = "REMzSnh1WU1mZ0dkTnIvUzFpZTZDOHJ1QjRFb3IxSituTHkrMGRmRlJHRERwTHd1bFJsOEJCR2JYcnNlMDdCQk1VR2JlMzJNVUhJYWxtNS84TTFzY2h4T0w1a3pReXJXVk50WTBzQlBMSjhZZGlyU3Z6aElLbTdhVUhrMmc0RnM=";
static constexpr const char *BL_COOKIE_URL = "R0FJc1BoY0MzL0ZtdlUzbE5UcXNWY0xqZCszUE9ReDJEZ254bDkxaTdnOFQ1REllUEpFUkJUV21ac0xNNitkYXZudDc5RTNaTHdPSzFrQ25OZFYvc2RrQVpOdVU1dDRDa3VJeEp3aStuV2hwazZoNVdYSFAxeGJ4L2MrV3hqSzd1RVJSZlZwbUdQazNIUmt3TlMvT0xSQkFUUXBicUp2aDg0OU5YS0ZCcFQvMk8xTGtiZzFsc2paeEI3WnpTZkx3QUJvai82VytwYmQrelpWbUljbTdEaVZXejg0UmlHUWZNM0xmMjc4QzlUZ2VSRi9mM1FEZzQwUTFPcjRyWDU0L0t5cWl3QmZ6K0s5SEtmeW44R1l5RHRUOXZnZHNXVlpvc09nNEJ3V0tTUk1ZQzFhQWZ4N2tHS1k4Skw0PQ==";

void ReqBLInterface::makeRequestRawHeader(QNetworkRequest *request) noexcept
{
    request->setRawHeader("Cookie", TTK::Algorithm::mdII(BL_COOKIE_URL, false).toUtf8());

    TTK::setSslConfiguration(request);
    TTK::setContentTypeHeader(request);
}

void ReqBLInterface::parseFromMovieInfo(TTK::MusicSongInformation *info, QString &cid)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(BL_MOVIE_DATA_URL, false).arg(info->m_songId));
    ReqBLInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            const QVariantList &datas = value["data"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();

                cid = value["cid"].toString();
                break;
            }
        }
    }
}

void ReqBLInterface::parseFromMovieProperty(TTK::MusicSongInformation *info, const QString &cid)
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(BL_MOVIE_PLAY_URL, false).arg(info->m_songId, cid).arg(32));
    ReqBLInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJsonParseError ok;
    const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
    if(QJsonParseError::NoError == ok.error)
    {
        QVariantMap value = json.toVariant().toMap();
        if(value["code"].toInt() == 0 && value.contains("data"))
        {
            value = value["data"].toMap();

            const QVariantList &datas = value["durl"].toList();
            for(const QVariant &var : qAsConst(datas))
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();

                TTK::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_url.replace(HTTPS_PROTOCOL, HTTP_PROTOCOL);
                prop.m_size = TTK::Number::sizeByteToLabel(value["size"].toInt());
                prop.m_format = MP4_FILE_SUFFIX;
                prop.m_bitrate = TTK_BN_250;
                info->m_songProps.append(prop);
                info->m_duration = TTKTime::formatDuration(value["length"].toInt());
                break;
            }
        }
    }
}
