#include "musicfmradiosongsrequest.h"

MusicFMRadioSongsRequest::MusicFMRadioSongsRequest(QObject *parent)
    : MusicAbstractFMRadioRequest(parent)
{

}

void MusicFMRadioSongsRequest::startRequest(const QString &id)
{
    m_cachedID = id;
    m_songInfo = TTK::MusicSongInformation();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(FM_PLAYLIST_URL, false).arg(id));
    MusicFMInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicFMRadioSongsRequest::downLoadFinished()
{
    MusicAbstractFMRadioRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();

            const QVariantList &datas = value["song"].toList();
            if(datas.isEmpty())
            {
                TTK_ERROR_STREAM("The fm radio song is empty");
                deleteAll();
                startRequest(m_cachedID);
                return;
            }

            for(const QVariant &var : qAsConst(datas))
            {
                value = var.toMap();
                TTK_NETWORK_QUERY_CHECK();

                if(value.isEmpty() || value["url"].toString().isEmpty())
                {
                    continue;
                }

                TTK::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_bitrate = value["kbps"].toInt();
                prop.m_format = value["file_ext"].toString();

                const QVariantMap &formats = value["available_formats"].toMap();
                const QString &bitrate = QString::number(prop.m_bitrate);
                if(formats.contains(bitrate))
                {
                    prop.m_size = TTK::Number::sizeByteToLabel(formats[bitrate].toInt() * 1000);
                }

                m_songInfo.m_songProps << prop;
                m_songInfo.m_songName = TTK::String::charactersReplaced(value["title"].toString());
                m_songInfo.m_singerName = TTK::String::charactersReplaced(value["artist"].toString());
                m_songInfo.m_coverUrl = value["picture"].toString();
                m_songInfo.m_albumName = TTK::String::charactersReplaced(value["albumtitle"].toString());
                m_songInfo.m_lrcUrl = TTK::Algorithm::mdII(FM_LRC_URL, false).arg(value["sid"].toString(), value["ssid"].toString());

                if(!m_songInfo.m_songProps.isEmpty())
                {
                    break;
                }
            }

        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
