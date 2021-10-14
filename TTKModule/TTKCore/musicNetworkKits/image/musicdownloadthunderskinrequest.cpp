#include "musicdownloadthunderskinrequest.h"
#include "musicdownloadsourcerequest.h"

#define DOWNLOAD_URL   "eC9KOTYxbVhvVDJNcGEwckhyMVZRdVRhOHhFRHQ2eFVNdWJxaURFSzA1ZWVmZm5HOFlzS1VCY2ZKOFRlYStBL2Y3SjNEK2gzY2QwPQ=="

MusicSkinThunderConfigManager::MusicSkinThunderConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicSkinThunderConfigManager::readSkinRemoteData(MusicSkinRemoteGroups &items)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("group");
    for(int i=0; i<nodeList.count(); ++i)
    {
        MusicSkinRemoteGroup group;
        QDomNode node = nodeList.at(i);
        group.m_group = node.toElement().attribute("name");

        const QDomNodeList &grouplist = node.childNodes();
        for(int j=0; j<grouplist.count(); ++j)
        {
            node = grouplist.at(j);

            MusicSkinRemoteItem item;
            const QDomNodeList &packageList = node.childNodes();
            for(int k=0; k<packageList.count(); ++k)
            {
                const QDomElement &element = packageList.at(k).toElement();
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
}


MusicDownloadThunderSkinRequest::MusicDownloadThunderSkinRequest(QObject *parent)
    : MusicAbstractDownloadSkinRequest(parent)
{

}

void MusicDownloadThunderSkinRequest::startToDownload()
{
    MusicDownloadSourceRequest *download = new MusicDownloadSourceRequest(this);
    connect(download, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(DOWNLOAD_URL, false));
}

void MusicDownloadThunderSkinRequest::downLoadFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroups items;
    MusicSkinThunderConfigManager manager;
    if(manager.fromByteArray(bytes))
    {
        manager.readSkinRemoteData(items);
    }

    Q_EMIT downLoadDataChanged(items);
}
