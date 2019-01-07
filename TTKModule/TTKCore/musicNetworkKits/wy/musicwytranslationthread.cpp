#include "musicwytranslationthread.h"
#include "musicsemaphoreloop.h"
#include "musicdownloadquerywythread.h"
#include "musicdownloadwyinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicWYTranslationThread::MusicWYTranslationThread(QObject *parent)
    : MusicTranslationThreadAbstract(parent)
{

}

MusicWYTranslationThread::~MusicWYTranslationThread()
{
    deleteAll();
}

void MusicWYTranslationThread::startToDownload(const QString &data)
{
    Q_UNUSED(data);
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryWYThread *d = new MusicDownLoadQueryWYThread(this);
    d->setQueryAllRecords(false);
    d->setQuerySimplify(true);
    d->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, QFileInfo(m_rawData["name"].toString()).baseName());
    connect(d, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    QUrl musicUrl;
    if(!d->isEmpty())
    {
        musicUrl.setUrl(MusicUtils::Algorithm::mdII(WY_SONG_LRC_URL, false).arg(d->getMusicSongInfos().first().m_songId));
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYTranslationThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value["code"].toInt() == 200)
            {
                value = value["tlyric"].toMap();
                emit downLoadDataChanged(value["lyric"].toString());
            }
        }
        else
        {
            emit downLoadDataChanged(QString());
        }
    }
    else
    {
        M_LOGGER_ERROR("Translation source data error");
        emit downLoadDataChanged(QString());
    }

    deleteAll();
}
