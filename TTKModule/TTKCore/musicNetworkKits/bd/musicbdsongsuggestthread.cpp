#include "musicbdsongsuggestthread.h"
#include "musicdownloadbdinterface.h"
#///QJson import
#include "qjson/parser.h"

MusicBDSongSuggestThread::MusicBDSongSuggestThread(QObject *parent)
    : MusicDownLoadSongSuggestThread(parent)
{

}

QString MusicBDSongSuggestThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicBDSongSuggestThread::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    QUrl musicUrl = MusicUtils::Algorithm::mdII(BD_SUGGEST_URL, false).arg(text);
    deleteAll();
    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(BD_UA_URL_1, ALG_UA_KEY, false).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicBDSongSuggestThread::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_items.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantList datas = value["song"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicPlaylistItem item;
                    item.m_name = value["songname"].toString();
                    item.m_nickName = value["artistname"].toString();
                    m_items << item;
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
