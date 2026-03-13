#include "musicdownloadthunderskinrequest.h"

static constexpr int MAX_SIZE = 30;
static constexpr const char *QUERY_URL = "eC9KOTYxbVhvVDJNcGEwckhyMVZRdVRhOHhFRHQ2eFVNdWJxaURFSzA1ZWVmZm5HOFlzS1VCY2ZKOFRlYStBL2Y3SjNEK2gzY2QwPQ==";

MusicThunderSkinConfigManager::MusicThunderSkinConfigManager()
    : TTKAbstractXml()
{

}

bool MusicThunderSkinConfigManager::readBuffer(MusicSkinRemoteGroupList &groups)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("group");
    for(int i = 0; i < nodes.count(); ++i)
    {
        MusicSkinRemoteGroup group;
        QDomNode node = nodes.item(i);
        group.m_id = node.toElement().attribute("name");
        group.m_name = group.m_id;
        group.m_type = MusicSkinRemoteGroup::Thunder;

        const QDomNodeList &groupNodes = node.childNodes();
        for(int j = 0; j < groupNodes.count(); ++j)
        {
            if(j > MAX_SIZE)
            {
                break;
            }

            MusicSkinRemoteItem item;
            item.m_index = j;

            node = groupNodes.item(j);
            const QDomNodeList &packageNodes = node.childNodes();

            for(int k = 0; k < packageNodes.count(); ++k)
            {
                const QDomElement &element = packageNodes.item(k).toElement();
                if(element.nodeName() == "name")
                {
                    item.m_name = element.text();
                }
                else if(element.nodeName() == "popularity")
                {
                    item.m_useCount = element.text().toInt();
                }
                else if(element.nodeName() == "file")
                {
                    item.m_url = element.text();
                }
            }

            if(item.isValid())
            {
                group.m_items << item;
            }
        }

        if(group.isValid())
        {
            groups << group;
        }
    }

    return true;
}


MusicDownloadThunderSkinRequest::MusicDownloadThunderSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadThunderSkinRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false));
    TTK::setUserAgentHeader(&request);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicDownloadThunderSkinRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicSkinRemoteGroupList groups;
    MusicAbstractDownloadSkinRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        MusicThunderSkinConfigManager manager;
        if(manager.fromByteArray(m_reply->readAll()))
        {
            manager.readBuffer(groups);
        }
    }

    Q_EMIT downloadDataChanged(groups);
    deleteAll();
}
