#include "musicwytranslationrequest.h"
#include "musicwyqueryinterface.h"
#include "musicwyqueryrequest.h"

MusicWYTranslationRequest::MusicWYTranslationRequest(QObject *parent)
    : MusicAbstractTranslationRequest(parent)
{

}

void MusicWYTranslationRequest::startRequest(const QString &data)
{
    TTK_INFO_STREAM(className() << "startRequest");

    Q_UNUSED(data);
    MusicAbstractNetwork::deleteAll();

    TTKSemaphoreLoop loop;
    MusicWYQueryRequest query(this), *d = &query;
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    d->setQueryType(MusicAbstractQueryRequest::QueryType::Music);
    d->startToSearch(QFileInfo(header("name").toString()).baseName());
    loop.exec();

    if(d->isEmpty())
    {
        TTK_INFO_STREAM(className() << "downLoadFinished");
        Q_EMIT downLoadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(d->songInfoList().front().m_songId));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYTranslationRequest::downLoadFinished()
{
    TTK_INFO_STREAM(className() << "downLoadFinished");

    MusicAbstractTranslationRequest::downLoadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200)
            {
                value = value["tlyric"].toMap();
                Q_EMIT downLoadDataChanged(value["lyric"].toString());
                deleteAll();
                return;
            }
        }
    }

    TTK_ERROR_STREAM("Translation source data error");
    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
