#include "musicwyqueryartistlistrequest.h"

MusicWYQueryArtistListRequest::MusicWYQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = TTK_HIGH_LEVEL;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistListRequest::startToPage(int offset)
{
    TTK_INFO_STREAM(QString("%1 startToPage %2").arg(className()).arg(offset));

    deleteAll();
    m_totalSize = TTK_HIGH_LEVEL;
    QString catId = "1001", initial = "-1";
    const QStringList &dds = m_queryValue.split(TTK_SPLITER);

    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "1001";
        }

        int id = dds[1].toInt();
        if(id <= 0)
        {
            id = -1;
        }
        else if(id > 0 && id <= 26)
        {
            id += 65 - 1;
        }
        else if(id > 26)
        {
            id = 0;
        }

        initial = QString::number(id);
    }

    QNetworkRequest request;
    const QByteArray &parameter = MusicWYInterface::makeTokenRequest(&request,
                      TTK::Algorithm::mdII(WY_ARTIST_LIST_URL, false),
                      TTK::Algorithm::mdII(WY_ARTIST_LIST_DATA_URL, false).arg(catId).arg(0).arg(100).arg(initial));

    m_reply = m_manager.post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYQueryArtistListRequest::startToSearch(const QString &value)
{
    m_queryValue = value;
    startToPage(0);
}

void MusicWYQueryArtistListRequest::downLoadFinished()
{
    TTK_INFO_STREAM(QString("%1 downLoadFinished").arg(className()));

    MusicQueryArtistListRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("artists"))
            {
                const QVariantList &datas = value["artists"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicResultDataItem result;
                    result.m_id = value["id"].toString();
                    result.m_name = value["name"].toString();
                    Q_EMIT createArtistListItem(result);
                }
            }
        }
    }

//    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
