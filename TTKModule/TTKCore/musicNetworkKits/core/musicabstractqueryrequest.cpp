#include "musicabstractqueryrequest.h"

MusicAbstractQueryRequest::MusicAbstractQueryRequest(QObject *parent)
    : MusicPageQueryRequest(parent),
      m_queryServer("Invalid"),
      m_queryType(QueryType::Music),
      m_queryMode(QueryMode::Normal)
{

}

void MusicAbstractQueryRequest::startToSearchByID(const QString &value)
{
    Q_UNUSED(value);
}

void MusicAbstractQueryRequest::startToQueryResult(TTK::MusicSongInformation *info, int bitrate)
{
    for(TTK::MusicSongInformation &var : m_songInfos)
    {
        if(var.m_songId == info->m_songId)
        {
            var.m_songProps = info->m_songProps;
            break;
        }
    }

    Q_UNUSED(bitrate);
}

void MusicAbstractQueryRequest::downLoadFinished()
{
    Q_EMIT clearItems();
    m_songInfos.clear();
    MusicPageQueryRequest::downLoadFinished();
}

QString MusicAbstractQueryRequest::serverToString() const
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
    else if(!m_queryServer.isEmpty())
    {
        return v.arg(m_queryServer);
    }
    else
    {
        return {};
    }
}

bool MusicAbstractQueryRequest::findUrlPathSize(TTK::MusicSongProperty *prop, const QString &duration) const
{
    if(!prop->m_size.isEmpty() && prop->m_size != TTK_DEFAULT_STR)
    {
        return false;
    }

    if(prop->m_bitrate != -1 && !duration.isEmpty() && duration != TTK_DEFAULT_STR)
    {
        prop->m_size = TTK::Number::sizeByteToLabel(duration, prop->m_bitrate);
    }
    else
    {
        TTK_NETWORK_QUERY_CHECK(false);
        prop->m_size = TTK::Number::sizeByteToLabel(TTK::queryFileSizeByUrl(prop->m_url));
        TTK_NETWORK_QUERY_CHECK(false);
    }
    return true;
}

bool MusicAbstractQueryRequest::findUrlPathSize(TTK::MusicSongPropertyList *props, const QString &duration) const
{
    for(int i = 0; i < props->count(); ++i)
    {
        if(!findUrlPathSize(&(*props)[i], duration))
        {
            return false;
        }
    }
    return true;
}
