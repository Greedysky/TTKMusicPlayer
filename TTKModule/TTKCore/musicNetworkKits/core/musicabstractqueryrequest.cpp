#include "musicabstractqueryrequest.h"

MusicAbstractQueryRequest::MusicAbstractQueryRequest(QObject *parent)
    : MusicPageQueryRequest(parent),
      m_queryServer("Invalid"),
      m_queryMode(QueryMode::Normal)
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

bool MusicAbstractQueryRequest::findUrlFileSize(TTK::MusicSongProperty *prop, const QString &duration) const
{
    if(!prop->m_size.isEmpty() && prop->m_size != TTK_DEFAULT_STR)
    {
        return false;
    }

    if(prop->m_bitrate != -1 && !duration.isEmpty() && duration != TTK_DEFAULT_STR)
    {
        prop->m_size = TTK::Number::sizeByteToLabel(TTKTime::formatDuration(duration) * prop->m_bitrate / 8.0);
    }
    else
    {
        TTK_NETWORK_QUERY_CHECK(false);
        prop->m_size = TTK::Number::sizeByteToLabel(TTK::queryFileSizeByUrl(prop->m_url));
        TTK_NETWORK_QUERY_CHECK(false);
    }
    return true;
}

bool MusicAbstractQueryRequest::findUrlFileSize(TTK::MusicSongPropertyList *props, const QString &duration) const
{
    for(int i = 0; i < props->count(); ++i)
    {
        if(!findUrlFileSize(&(*props)[i], duration))
        {
            return false;
        }
    }
    return true;
}
