#include "musicdownloadbackgroundremotethread.h"
#include "musicdownloadsourcethread.h"
#include "musicotherdefine.h"
#///QJson import
#include "qjson/parser.h"

#define THUD_URL   "eC9KOTYxbVhvVDJNcGEwckhyMVZRdVRhOHhFRHQ2eFVNdWJxaURFSzA1ZWVmZm5HOFlzS1VCY2ZKOFRlYStBL2Y3SjNEK2gzY2QwPQ=="
#define BIGU_URL   "UEQvb1lxVXFnV0dqRmxzNkY0alFJUHZUSUhyZUVNY0Y2OGZ1L255cS9CMklCakk4Q1dNQkF3PT0="
#define BING_URL   "bkRaMGo0WEhveVlwbEV6a0FDbEsrNmNGVHVrZTh1VmFDZTBmdElkZ0ZCYXk2dDJMaXF3MUlrV2JndmlpUWVudkF5UVVaMklvSXQydGI3cFhaTFRtaUV2VUZBcz0="

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



MusicDownloadBackgroundRemoteThread::MusicDownloadBackgroundRemoteThread(QObject *parent)
    : QObject(parent)
{

}



MusicDownloadBackgroundThunderThread::MusicDownloadBackgroundThunderThread(QObject *parent)
    : MusicDownloadBackgroundRemoteThread(parent)
{

}

void MusicDownloadBackgroundThunderThread::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadDataFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(THUD_URL, false));
}

void MusicDownloadBackgroundThunderThread::downLoadDataFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroups items;

    MusicSkinThunderConfigManager manager;
    if(manager.fromByteArray(bytes))
    {
        manager.readSkinRemoteData(items);
    }

    emit downLoadDataChanged(items);
}



MusicDownloadBackgroundBingThread::MusicDownloadBackgroundBingThread(QObject *parent)
    : MusicDownloadBackgroundRemoteThread(parent)
{

}

void MusicDownloadBackgroundBingThread::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadDataFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(BING_URL, false));
}

void MusicDownloadBackgroundBingThread::downLoadDataFinished(const QByteArray &bytes)
{
    MusicSkinRemoteGroups items;

    QJson::Parser parser;
    bool ok;
    const QVariant &data = parser.parse(bytes, &ok);
    if(ok)
    {
        QVariantMap value = data.toMap();
        if(value.contains("images"))
        {
            MusicSkinRemoteGroup group;
            group.m_group = MUSIC_DAILY_DIR;
            const QVariantList &datas = value["images"].toList();
            foreach(const QVariant &var, datas)
            {
                if(var.isNull())
                {
                    continue;
                }

                value = var.toMap();
                MusicSkinRemoteItem item;
                item.m_name = value["copyright"].toString();
                item.m_useCount = value["startdate"].toInt();
                item.m_url = MusicUtils::Algorithm::mdII(BIGU_URL, false).arg(value["urlbase"].toString());

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

    emit downLoadDataChanged(items);
}
