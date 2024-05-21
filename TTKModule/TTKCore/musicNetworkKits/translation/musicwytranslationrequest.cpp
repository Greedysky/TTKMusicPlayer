#include "musicwytranslationrequest.h"
#include "musicwyqueryinterface.h"
#include "musicwyqueryrequest.h"
#include "musicotherdefine.h"

MusicWYTranslationRequest::MusicWYTranslationRequest(QObject *parent)
    : MusicAbstractTranslationRequest(parent)
{

}

void MusicWYTranslationRequest::startToRequest(const QString &data)
{
    TTK_INFO_STREAM(className() << "startToRequest");

    Q_UNUSED(data);
    MusicAbstractNetwork::deleteAll();

    TTKSemaphoreLoop loop;
    MusicWYQueryRequest query(this), *d = &query;
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    d->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
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
    request.setUrl(TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(d->items().front().m_songId));
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
                const QVariantMap &lValue = value["lrc"].toMap();
                const QString &lrc = lValue["lyric"].toString();

                const QVariantMap &tValue = value["tlyric"].toMap();
                const QString &tlrc = tValue["lyric"].toString();

                if(!lrc.isEmpty() && !tlrc.isEmpty())
                {
                    QString text;
                    QStringList orts = lrc.split(TTK_LINEFEED);
                    QStringList trts = tlrc.split(TTK_LINEFEED);

                    for(QString &ort : orts)
                    {
                        const QRegExp regx("\\[.+\\]");
                        const QString &prefix = (regx.indexIn(ort) != -1) ? regx.cap(0) : QString();

                        if(prefix.isEmpty())
                        {
                            continue;
                        }

                        text += ort.remove(prefix) + TTK_WLINEFEED;

                        for(QString &trt : trts)
                        {
                            if(trt.startsWith(prefix))
                            {
                                text += trt.remove(prefix) + TTK_WLINEFEED;
                                break;
                            }
                        }
                    }

                    Q_EMIT downLoadDataChanged(TTK_AUTHOR_NAME + text);
                    deleteAll();
                    return;
                }
            }
        }
    }

    TTK_ERROR_STREAM("Translation source data error");
    Q_EMIT downLoadDataChanged({});
    deleteAll();
}
