#include "musicresourcerequest.h"
#include "musicdownloaddatarequest.h"

#include "qsync/qsyncutils.h"

static constexpr const char *QUERY_RESOURCE_DIR = "resource";
static constexpr const char *QUERY_RESOURCE_URL = "resource.json";

MusicResourceRequest::MusicResourceRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicResourceRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(QSyncUtils::makeDataBucketUrl() + QUERY_RESOURCE_URL);
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicResourceRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << __FUNCTION__);

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QDir dir(APPCACHE_DIR_FULL);
        if(!dir.exists(QUERY_RESOURCE_DIR))
        {
            dir.mkdir(QUERY_RESOURCE_DIR);
        }

        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            const QVariantList &datas = json.toVariant().toList();
            for(const QVariant &var : qAsConst(datas))
            {
                const QVariantMap &value = var.toMap();
                const QString &key = value["key"].toString();
                const QString &path = APPCACHE_DIR_FULL + key;

                if(QDateTime::fromString(value["time"].toString(), TTK_DATE_FORMAT) > QFileInfo(path).lastModified())
                {
                    MusicDownloadDataRequest *d = new MusicDownloadDataRequest(QSyncUtils::makeDataBucketUrl() + key, path, TTK::Download::Extra, this);
                    d->startToRequest();
                }
            }
        }
    }

    deleteAll();
}
