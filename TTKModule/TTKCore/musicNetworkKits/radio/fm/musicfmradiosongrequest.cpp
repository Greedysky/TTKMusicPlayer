#include "musicfmradiosongrequest.h"
#include "musicwyqueryinterface.h"

static constexpr const char *FM_DETAIL_URL = "M1htRHNMTFJzSGJPVzFUbUV5MFQxM3E1NDhUalAzdkNUUnNNVm5UVGt6YUtMbWUwZlhwRFV5TnZUdVE9";

MusicFMRadioSongRequest::MusicFMRadioSongRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicFMRadioSongRequest::startToRequest()
{
    deleteAll();
    m_item = TTK::MusicSongInformation();

    QNetworkRequest request;
    const QByteArray &parameter = ReqWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(FM_DETAIL_URL, false),
                      "{}");

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicFMRadioSongRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
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
                    TTK_NETWORK_QUERY_CHECK();

                    TTK::MusicSongInformation info;
                    info.m_songId = value["id"].toString();
                    info.m_songName = TTK::String::charactersReplace(value["name"].toString());

                    const QVariantList &artistsArray = value["artists"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }

                        const QVariantMap &artistObject = artistValue.toMap();
                        if(info.m_artistId.isEmpty())
                        {
                            info.m_artistId = artistObject["id"].toString();
                        }

                        info.m_artistName = ReqWYInterface::makeSongArtist(info.m_artistName, artistObject["name"].toString());
                    }

                    const QVariantMap &albumObject = value["album"].toMap();
                    info.m_albumId = albumObject["id"].toString();
                    info.m_albumName = TTK::String::charactersReplace(albumObject["name"].toString());

                    info.m_coverUrl = albumObject["picUrl"].toString();
                    info.m_lrcUrl = TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(info.m_songId);
                    info.m_duration = TTKTime::formatDuration(value["duration"].toInt());
                    info.m_year.clear();
                    info.m_trackNumber = value["no"].toString();

                    TTK_NETWORK_QUERY_CHECK();
                    ReqWYInterface::parseFromSongProperty(&info, TTK_BN_128);
                    TTK_NETWORK_QUERY_CHECK();

                    if(!info.m_songProps.isEmpty())
                    {
                        m_item = info;
                        break;
                    }
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
