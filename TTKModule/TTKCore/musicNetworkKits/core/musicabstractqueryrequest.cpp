#include "musicabstractqueryrequest.h"

MusicAbstractQueryRequest::MusicAbstractQueryRequest(QObject *parent)
    : MusicPageQueryRequest(parent),
      m_queryServer("Invalid"),
      m_queryQuality(TTK::QueryQuality::Standard),
      m_queryAllRecords(false),
      m_queryLite(false)
{

}

void MusicAbstractQueryRequest::startToSingleSearch(const QString &id)
{
    Q_UNUSED(id);
}

QString MusicAbstractQueryRequest::mapQueryServerString() const
{
    const QString &v = tr("Current used server from %1");
    if(m_queryServer.contains(QUERY_KG_INTERFACE))
    {
        return v.arg(tr("KG"));
    }
    else if(m_queryServer.contains(QUERY_KW_INTERFACE))
    {
        return v.arg(tr("KW"));
    }
    else if(m_queryServer.contains(QUERY_WY_INTERFACE))
    {
        return v.arg(tr("WY"));
    }
    else
    {
        return {};
    }
}

void MusicAbstractQueryRequest::downLoadFinished()
{
    Q_EMIT clearItems();
    m_songInfos.clear();
    MusicPageQueryRequest::downLoadFinished();
}

bool MusicAbstractQueryRequest::findUrlFileSize(TTK::MusicSongProperty *prop) const
{
    TTK_NETWORK_QUERY_CHECK(false);
    if(prop->m_size.isEmpty() || prop->m_size == TTK_DEFAULT_STR)
    {
        prop->m_size = TTK::Number::sizeByteToLabel(TTK::queryFileSizeByUrl(prop->m_url));
    }
    TTK_NETWORK_QUERY_CHECK(false);
    return true;
}

bool MusicAbstractQueryRequest::findUrlFileSize(TTK::MusicSongPropertyList *props) const
{
    for(int i = 0; i < props->count(); ++i)
    {
        if(!findUrlFileSize(&(*props)[i]))
        {
            return false;
        }
    }
    return true;
}
