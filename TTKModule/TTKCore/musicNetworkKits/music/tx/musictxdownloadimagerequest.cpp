#include "musictxdownloadimagerequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicdownloaddatarequest.h"
#include "musictxqueryinterface.h"

MusicTXImageConfigManager::MusicTXImageConfigManager(QObject *parent)
    : MusicAbstractXml(parent)
{

}

void MusicTXImageConfigManager::readBuffer(QStringList &items)
{
    const QDomNodeList &nodeList = m_document->elementsByTagName("url");
    for(int i = 0; i < nodeList.count(); ++i)
    {
        QDomNode node = nodeList.at(i);
        items << node.toElement().text();
    }
}


MusicTXDownloadBackgroundRequest::MusicTXDownloadBackgroundRequest(const QString &name, const QString &path, QObject *parent)
    : MusicDownloadImageRequest(name, path, parent)
{

}

void MusicTXDownloadBackgroundRequest::startToDownload()
{
    TTK_LOGGER_INFO(QString("%1 startToDownload").arg(className()));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(TX_SEARCH_URL, false));
    MusicTXInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.post(request, MusicUtils::Algorithm::mdII(TX_SEARCH_DATA_URL, false).arg(m_artName).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadDataFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicTXDownloadBackgroundRequest::downLoadDataFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadDataFinished").arg(className()));

    MusicAbstractNetwork::downLoadFinished();
    QString id;
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            value = value["music.search.SearchCgiService"].toMap();
            if(value.contains("code") && value["code"].toInt() == 0)
            {
                value = value["data"].toMap();
                value = value["body"].toMap();
                value = value["song"].toMap();
                const QVariantList &datas = value["list"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();
                    id = value["id"].toString();
                    break;
                }
            }
        }
    }

    if(id.isEmpty())
    {
        deleteAll();
        return;
    }

    downLoadUrl(id);
}

void MusicTXDownloadBackgroundRequest::downLoadUrlFinished()
{
    TTK_LOGGER_INFO(QString("%1 downLoadUrlFinished").arg(className()));

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QStringList items;
        MusicTXImageConfigManager manager;
        if(manager.fromByteArray(m_reply->readAll()))
        {
            manager.readBuffer(items);
        }

        for(const QString &url : qAsConst(items))
        {
            if(m_counter < MAX_IMAGE_COUNTER)
            {
                MusicDownloadDataRequest *download = new MusicDownloadDataRequest(url, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL, m_savePath).arg(m_counter++).arg(SKN_FILE), MusicObject::Download::Background, this);
                connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(downLoadFinished()));
                download->startToDownload();
            }
        }
    }

    emit downLoadDataChanged(QString::number(m_counter));
    if(m_counter == 0)
    {
        deleteAll();
    }
}

void MusicTXDownloadBackgroundRequest::downLoadUrl(const QString &id)
{
    TTK_LOGGER_INFO(QString("%1 downLoadUrl %2").arg(className(), id));

    MusicAbstractNetwork::deleteAll();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(TX_BACKGROUND_URL, false).arg(id));
    MusicTXInterface::makeRequestRawHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadUrlFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}
