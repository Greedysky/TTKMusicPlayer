#include "musicdownloadquerywyartistlistthread.h"
#include "musicnumberdefine.h"

MusicDownLoadQueryWYArtistListThread::MusicDownLoadQueryWYArtistListThread(QObject *parent)
    : MusicDownLoadQueryArtistListThread(parent)
{
    m_pageSize = MU_MAX;
    m_pageTotal = MU_MAX;
    m_queryServer = "WangYi";
}

QString MusicDownLoadQueryWYArtistListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryWYArtistListThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));
    QString catId = "1001", initial = "-1";
    QStringList dds = m_searchText.split(STRING_SPLITER);
    if(dds.count() == 2)
    {
        catId = dds[0];
        if(catId.isEmpty())
        {
            catId = "1001";
        }

        int mIdx = dds[1].toInt();
        if(mIdx > -1 && mIdx < 27)
        {
            mIdx += 65;
        }
        else if(mIdx >= 27)
        {
            mIdx = 0;
        }

        initial = QString::number(mIdx);
    }
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WY_AR_LIST_URL, false).arg(catId).arg(initial);

    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    makeTokenQueryQequest(&request);
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicDownLoadQueryWYArtistListThread::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    startToPage(0);
}

void MusicDownLoadQueryWYArtistListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    emit clearAllItems();
    m_musicSongInfos.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QString html(m_reply->readAll());

        QRegExp regx("<a href=\".?/artist\\?id=(\\d+).*>(.*)</a>");
        regx.setMinimal(true);
        int pos = html.indexOf(regx);
        while(pos != -1)
        {
            if(m_interrupt) return;

            MusicResultsItem info;
            info.m_id = regx.cap(1);
            info.m_name = regx.cap(2);
            emit createArtistListItems(info);

            pos += regx.matchedLength();
            pos = regx.indexIn(html, pos);
        }
    }

//    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
