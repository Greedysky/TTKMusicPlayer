#include "musicxmartistsimilarrequest.h"

MusicXMArtistSimilarRequest::MusicXMArtistSimilarRequest(QObject *parent)
    : MusicSimilarRequest(parent)
{
    m_pageSize = 20;
}

void MusicXMArtistSimilarRequest::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    deleteAll();

    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    makeTokenQueryUrl(&request, false,
                      MusicUtils::Algorithm::mdII(XM_ARTIST_SIMILAR_DATA_URL, false).arg(text).arg(1).arg(m_pageSize),
                      MusicUtils::Algorithm::mdII(XM_ARTIST_SIMILAR_URL, false));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMArtistSimilarRequest::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

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
                const QVariantList &datas = value["artists"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicResultsItem info;
                    info.m_id = value["artistId"].toString();
                    info.m_coverUrl = value["artistLogo"].toString();
                    info.m_name = value["artistName"].toString();
                    info.m_updateTime.clear();
                    Q_EMIT createSimilarItem(info);
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
