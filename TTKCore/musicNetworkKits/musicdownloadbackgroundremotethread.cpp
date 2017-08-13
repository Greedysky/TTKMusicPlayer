#include "musicdownloadbackgroundremotethread.h"
#include "musicdownloadsourcethread.h"

#define D_URL   "eC9KOTYxbVhvVDJNcGEwckhyMVZRdVRhOHhFRHQ2eFVNdWJxaURFSzA1ZWVmZm5HOFlzS1VCY2ZKOFRlYStBL2Y3SjNEK2gzY2QwPQ=="

MusicSkinRemoteConfigManager::MusicSkinRemoteConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

QString MusicSkinRemoteConfigManager::getClassName()
{
    return staticMetaObject.className();
}

void MusicSkinRemoteConfigManager::readSkinRemoteXMLConfig(MusicSkinRemoteGroups &items)
{
    QDomNodeList nodelist = m_ddom->elementsByTagName("group");
    for(int i=0; i<nodelist.count(); ++i)
    {
        MusicSkinRemoteGroup group;
        QDomNode node = nodelist.at(i);
        group.m_group = node.toElement().attribute("name");

        QDomNodeList grouplist = node.childNodes();
        for(int j=0; j<grouplist.count(); ++j)
        {
            node = grouplist.at(j);

            MusicSkinRemoteItem item;
            QDomNodeList packagelist = node.childNodes();
            for(int k=0; k<packagelist.count(); ++k)
            {
                QDomElement ele = packagelist.at(k).toElement();
                if(ele.nodeName() == "name")
                {
                    item.m_name = ele.text();
                }
                else if(ele.nodeName() == "popularity")
                {
                    item.m_useCount = ele.text().toInt();
                }
                else if(ele.nodeName() == "file")
                {
                    item.m_url = ele.text();
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



MusicDownloadBackgroundRemoteThread::MusicDownloadBackgroundRemoteThread(QObject *parent)
    : QObject(parent)
{

}

QString MusicDownloadBackgroundRemoteThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownloadBackgroundRemoteThread::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadDataFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(D_URL, false));
}

void MusicDownloadBackgroundRemoteThread::downLoadDataFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroups items;

    MusicSkinRemoteConfigManager manager;
    if(manager.fromByteArray(bytes))
    {
        manager.readSkinRemoteXMLConfig(items);
    }

    emit downLoadDataChanged(items);
}
