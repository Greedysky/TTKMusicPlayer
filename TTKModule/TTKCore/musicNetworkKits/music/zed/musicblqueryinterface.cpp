#include "musicblqueryinterface.h"
#include "musicabstractnetwork.h"

void ReqBLInterface::makeRequestRawHeader(QNetworkRequest *request)
{
    TTK::setSslConfiguration(request);
    TTK::makeContentTypeHeader(request);
    request->setRawHeader("Cookie", TTK::Algorithm::mdII(BL_BUVID_URL, false).toUtf8());
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

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
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
    request.setUrl(TTK::Algorithm::mdII(BL_MOVIE_PLAY_URL, false).arg(info->m_songId, cid).arg(16));
    ReqBLInterface::makeRequestRawHeader(&request);

    const QByteArray &bytes = TTK::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return;
    }

    QJson::Parser json;
    bool ok = false;
    const QVariant &data = json.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
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
