#include "musicqqdiscoverlistrequest.h"
#include "musicqqqueryinterface.h"

MusicQQDiscoverListRequest::MusicQQDiscoverListRequest(QObject *parent)
    : MusicDiscoverListRequest(parent)
{

}

void MusicQQDiscoverListRequest::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch").arg(getClassName()));
    deleteAll();

    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(QQ_TOPLIST_URL, false).arg(4);
    m_toplistInfo.clear();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(QQ_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicQQDiscoverListRequest::downLoadFinished()
{
    if(!m_reply)
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
            if(value["code"].toInt() == 0 && value.contains("songlist"))
            {
                const QVariantList &datas = value["songlist"].toList();
                int where = datas.count();
                where = (where > 0) ? MusicTime::random(where) : 0;

                int counter = 0;
                for(const QVariant &var : qAsConst(datas))
                {
                    if(m_interrupt) return;

                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    value = value["data"].toMap();

                    const QVariantList &artistsArray = value["singer"].toList();
                    for(const QVariant &artistValue : qAsConst(artistsArray))
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        const QVariantMap &artistMap = artistValue.toMap();
                        m_toplistInfo = artistMap["name"].toString();
                    }

                    m_toplistInfo += " - " + value["songname"].toString();
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(m_toplistInfo);
    deleteAll();
}
