#include "musickwartistsimilarrequest.h"
#include "musickwqueryinterface.h"

MusicKWArtistSimilarRequest::MusicKWArtistSimilarRequest(QObject *parent)
    : MusicSimilarRequest(parent)
{

}

void MusicKWArtistSimilarRequest::startToSearch(const QString &value)
{
    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(className(), value));

    deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_SIMILAR_URL, false).arg(artistNameById(value)));
    MusicKWInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicKWArtistSimilarRequest::downLoadFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(className()));

    MusicSimilarRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QString html(m_reply->readAll());
        QRegExp regx("<li><a href=.*data-src=\"([^\"]+).*<span>(.*)</span></a></li>");
        regx.setMinimal(true);

        int pos = html.indexOf(regx);
        while(pos != -1)
        {
            TTK_NETWORK_QUERY_CHECK();

            MusicResultsItem result;
            TTK_NETWORK_QUERY_CHECK();
            result.m_id = artistIdName(regx.cap(2));
            TTK_NETWORK_QUERY_CHECK();

            result.m_coverUrl = regx.cap(1);
            result.m_name = regx.cap(2);
            result.m_updateTime.clear();
            Q_EMIT createSimilarItem(result);

            pos += regx.matchedLength();
            pos = regx.indexIn(html, pos);
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

QString MusicKWArtistSimilarRequest::artistNameById(const QString &id) const
{
    QString name;
    if(id.isEmpty())
    {
        return name;
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_INFO_URL, false).arg(id));
    MusicKWInterface::makeRequestRawHeader(&request);

    QByteArray bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return name;
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes.replace("'", "\""), &ok);
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        name = value["name"].toString();
    }

    return name;
}

QString MusicKWArtistSimilarRequest::artistIdName(const QString &name) const
{
    if(name.isEmpty())
    {
        return QString();
    }

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KW_ARTIST_INFO_NAME_URL, false).arg(name));
    MusicKWInterface::makeRequestRawHeader(&request);

    QByteArray bytes = MusicObject::syncNetworkQueryForGet(&request);
    if(bytes.isEmpty())
    {
        return QString();
    }

    QJson::Parser json;
    bool ok;
    const QVariant &data = json.parse(bytes.replace("'", "\""), &ok);

    QString id;
    if(ok)
    {
        const QVariantMap &value = data.toMap();
        id = value["id"].toString();
    }

    return id;
}
