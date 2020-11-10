#include "musicwyqueryartistlistrequest.h"

MusicWYQueryArtistListRequest::MusicWYQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = DEFAULT_LEVEL_HIGHER;
    m_pageTotal = DEFAULT_LEVEL_HIGHER;
    m_queryServer = QUERY_WY_INTERFACE;
}

void MusicWYQueryArtistListRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
    deleteAll();

    QString catId = "1001", initial = "-1";
    const QStringList &dds = m_searchText.split(TTK_STR_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "1001";
        }

        int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 26)
        {
            mIdx += 65;
        }
        else if(mIdx >= 26)
        {
            mIdx = 0;
        }

        initial = QString::number(mIdx);
    }
    m_interrupt = true;

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    const QByteArray &parameter = makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(WY_ARTIST_LIST_URL, false),
                      MusicUtils::Algorithm::mdII(WY_ARTIST_LIST_DATA_URL, false).arg(catId).arg(0).arg(100).arg(initial));
    if(!m_manager || m_stateCode != MusicObject::NetworkQuery) return;
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, parameter);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYQueryArtistListRequest::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    startToPage(0);
}

void MusicWYQueryArtistListRequest::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    Q_EMIT clearAllItems();
    m_musicSongInfos.clear();
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
            if(value["code"].toInt() == 200 && value.contains("artists"))
            {
                const QVariantList &datas = value["artists"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    if(m_interrupt) return;

                    value = var.toMap();
                    MusicResultsItem info;
                    info.m_id = value["id"].toString();
                    info.m_name = value["name"].toString();
                    Q_EMIT createArtistListItem(info);
                }
            }
        }
    }
//    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
