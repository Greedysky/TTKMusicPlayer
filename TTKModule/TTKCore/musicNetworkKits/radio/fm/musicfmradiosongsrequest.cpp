#include "musicfmradiosongsrequest.h"
#include "musicqqqueryinterface.h"

MusicFMRadioSongsRequest::MusicFMRadioSongsRequest(QObject *parent)
    : MusicAbstractFMRadioRequest(parent)
{

}

void MusicFMRadioSongsRequest::startToDownload(const QString &id)
{
    m_cachedID = id;
    m_songInfo = MusicObject::MusicSongInformation();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(FM_PLAYLIST_URL, false).arg(id));
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
#if TTK_QT_VERSION_CHECK(5,15,0)
    connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#else
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
#endif
}

void MusicFMRadioSongsRequest::downLoadFinished()
{
    MusicAbstractFMRadioRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            const QVariantList &datas = value["song"].toList();
            if(datas.isEmpty())
            {
                TTK_LOGGER_ERROR("The fm radio song is empty");
                deleteAll();
                startToDownload(m_cachedID);
                return;
            }

            for(const QVariant &var : qAsConst(datas))
            {
                value = var.toMap();
                if(value.isEmpty() || value["url"].toString().isEmpty())
                {
                    continue;
                }

                MusicObject::MusicSongProperty prop;
                prop.m_url = value["url"].toString();
                prop.m_bitrate = value["kbps"].toInt();
                prop.m_format = value["file_ext"].toString();

                const QVariantMap &formats = value["available_formats"].toMap();
                if(formats.contains(QString::number(prop.m_bitrate)))
                {
                    prop.m_size = MusicUtils::Number::sizeByte2Label(formats[QString::number(prop.m_bitrate)].toInt() * 1000);
                }

                m_songInfo.m_songProps << prop;
                m_songInfo.m_songName = MusicUtils::String::charactersReplaced(value["title"].toString());
                m_songInfo.m_singerName = MusicUtils::String::charactersReplaced(value["artist"].toString());
                m_songInfo.m_coverUrl = value["picture"].toString();
                m_songInfo.m_albumName = MusicUtils::String::charactersReplaced(value["albumtitle"].toString());
                m_songInfo.m_lrcUrl = MusicUtils::Algorithm::mdII(FM_LRC_URL, false).arg(value["sid"].toString(), value["ssid"].toString());

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
