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
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    Q_UNUSED(data);
    MusicAbstractNetwork::deleteAll();

    TTKEventLoop loop;
    MusicWYQueryRequest query(this), *req = &query;
    connect(req, SIGNAL(downloadDataChanged(QString)), &loop, SLOT(quit()));
    req->setQueryMode(MusicAbstractQueryRequest::QueryMode::Meta);
    req->startToSearch(QFileInfo(header("name").toString()).baseName());
    loop.exec();

    if(req->isEmpty())
    {
        TTK_INFO_STREAM(metaObject()->className() << "downloadFinished");
        Q_EMIT downloadDataChanged({});
        deleteAll();
        return;
    }

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(WY_SONG_LRC_OLD_URL, false).arg(req->items().first().m_songId));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request);

    m_reply = m_manager.get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicWYTranslationRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    MusicAbstractTranslationRequest::downloadFinished();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
                        static TTKRegularExpression regx("\\[.+\\]");
                        const QString &prefix = (regx.match(ort) != -1) ? regx.captured(0) : QString();

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

                    Q_EMIT downloadDataChanged(TTK_AUTHOR_NAME + text);
                    deleteAll();
                    return;
                }
            }
        }
    }

    TTK_ERROR_STREAM("Translation source data error");
    Q_EMIT downloadDataChanged({});
    deleteAll();
}
