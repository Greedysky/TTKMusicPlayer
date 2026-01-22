#include "musicdownloadthunderskinrequest.h"
#include "musicdatasourcerequest.h"

static constexpr int MAX_SIZE = 30;
static constexpr const char *QUERY_URL = "eC9KOTYxbVhvVDJNcGEwckhyMVZRdVRhOHhFRHQ2eFVNdWJxaURFSzA1ZWVmZm5HOFlzS1VCY2ZKOFRlYStBL2Y3SjNEK2gzY2QwPQ==";

MusicThunderSkinConfigManager::MusicThunderSkinConfigManager()
    : TTKAbstractXml()
{

}

bool MusicThunderSkinConfigManager::readBuffer(MusicSkinRemoteGroupList &items)
{
    const QDomNodeList &nodes = m_document->elementsByTagName("group");
    for(int i = 0; i < nodes.count(); ++i)
    {
        MusicSkinRemoteGroup group;
        QDomNode node = nodes.item(i);
        group.m_group = QString("%1%2").arg(SKIN_THUNDER_DIR, node.toElement().attribute("name"));
        group.m_id = TTK_DEFAULT_STR;

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
            items << group;
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
    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadFinished(QByteArray)));
    req->startToRequest(TTK::Algorithm::mdII(QUERY_URL, false));
}

void MusicDownloadThunderSkinRequest::downloadFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroupList items;
    MusicThunderSkinConfigManager manager;
    if(manager.fromByteArray(bytes))
    {
        manager.readBuffer(items);
    }

    Q_EMIT downloadDataChanged(items);
}
