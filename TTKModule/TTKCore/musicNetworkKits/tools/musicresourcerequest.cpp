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
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicResourceRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractNetwork::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        const QDir dir(APPDATA_DIR_FULL);
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
                const QString &path = APPDATA_DIR_FULL + key;

                if(QDateTime::fromString(value["time"].toString(), TTK_DATE_FORMAT) > QFileInfo(path).lastModified())
                {
                    MusicDownloadDataRequest *req = new MusicDownloadDataRequest(QSyncUtils::makeDataBucketUrl() + key, path, TTK::Download::Extra, this);
                    req->startToRequest();
                }
            }
        }
    }

    deleteAll();
}
