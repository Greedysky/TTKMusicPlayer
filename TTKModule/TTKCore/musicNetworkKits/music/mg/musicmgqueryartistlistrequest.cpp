#include "musicmgqueryartistlistrequest.h"

MusicMGQueryArtistListRequest::MusicMGQueryArtistListRequest(QObject *parent)
    : MusicQueryArtistListRequest(parent)
{
    m_pageSize = DEFAULT_LEVEL_HIGHER;
    m_queryServer = QUERY_MG_INTERFACE;
}

void MusicMGQueryArtistListRequest::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToPage %2").arg(getClassName()).arg(offset));

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicMGQueryArtistListRequest::startToSearch(const QString &artistlist)
{
    m_searchText = artistlist;
    startToPage(0);
}

void MusicMGQueryArtistListRequest::downLoadFinished()
{

}
