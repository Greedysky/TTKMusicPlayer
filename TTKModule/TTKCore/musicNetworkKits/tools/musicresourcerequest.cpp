#include "musicresourcerequest.h"
#include "musicdownloaddatarequest.h"

#include "qsync/qsyncutils.h"

static constexpr const char *OS_RESOURCE_DIR = "resource";
static constexpr const char *OS_RESOURCE_URL = "resource.json";

MusicResourceRequest::MusicResourceRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

void MusicResourceRequest::startToRequest()
{
    QNetworkRequest request;
    request.setUrl(QSyncUtils::makeDataBucketUrl() + OS_RESOURCE_URL);
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicResourceRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicAbstractNetwork::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QDir dir(APPCACHE_DIR_FULL);
        if(!dir.exists(OS_RESOURCE_DIR))
        {
            dir.mkdir(OS_RESOURCE_DIR);
        }

        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            const QVariantList &datas = data.toList();
            for(const QVariant &var : qAsConst(datas))
            {
                const QVariantMap &value = var.toMap();
                const QString &key = value["key"].toString();
                const QString &path = APPCACHE_DIR_FULL + key;

                if(QDateTime::fromString(value["time"].toString(), TTK_YEAR_FORMAT) > QFileInfo(path).lastModified())
                {
                    MusicDownloadDataRequest *d = new MusicDownloadDataRequest(QSyncUtils::makeDataBucketUrl() + key, path, TTK::Download::Other, this);
                    d->startToRequest();
                }
            }
        }
    }

    deleteAll();
}
