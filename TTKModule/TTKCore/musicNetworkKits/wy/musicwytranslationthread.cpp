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

QString MusicWYTranslationThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYTranslationThread::startToDownload(const QString &data)
{
    Q_UNUSED(data);
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryWYThread *query = new MusicDownLoadQueryWYThread(this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, QFileInfo(m_rawData["name"].toString()).baseName());
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    QUrl musicUrl;
    if(!query->getMusicSongInfos().isEmpty())
    {
        musicUrl.setUrl(MusicUtils::Algorithm::mdII(WY_SONG_LRC_URL, false).arg(query->getMusicSongInfos().first().m_songId));
    }

    QNetworkRequest request;
    request.setUrl(musicUrl);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
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
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
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
