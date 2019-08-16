#include "musicxmartistsimilarthread.h"
#include "musicdownloadxminterface.h"
#///QJson import
#include "qjson/parser.h"

MusicXMArtistSimilarThread::MusicXMArtistSimilarThread(QObject *parent)
    : MusicDownLoadSimilarThread(parent)
{

}

void MusicXMArtistSimilarThread::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    const QUrl &musicUrl = MusicUtils::Algorithm::mdII(XM_AR_SIM_URL, false).arg(text);
    deleteAll();

    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(XM_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMArtistSimilarThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        const QByteArray &bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(bytes, &ok);
        if(ok)
        {
            const QVariantList &datas = data.toList();
            foreach(const QVariant &var, datas)
            {
                if(m_interrupt) return;

                if(var.isNull())
                {
                    continue;
                }

                const QVariantMap &value = var.toMap();
                MusicResultsItem info;
                info.m_id = value["artist_id"].toString();
                info.m_coverUrl = value["artist_logo"].toString();
                info.m_name = value["name"].toString();
                info.m_updateTime.clear();
                if(!info.m_coverUrl.contains("http:"))
                {
                  info.m_coverUrl = "http:" + info.m_coverUrl;
                }
                emit createSimilarItem(info);
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
