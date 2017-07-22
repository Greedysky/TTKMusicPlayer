#include "musicxmdiscoverlistthread.h"
#include "musicdownloadxminterface.h"
#///QJson import
#include "qjson/parser.h"

MusicXMDiscoverListThread::MusicXMDiscoverListThread(QObject *parent)
    : MusicDownLoadDiscoverListThread(parent)
{

}

QString MusicXMDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicXMDiscoverListThread::startToSearch()
{
    if(!m_manager)
    {
        return;
    }

    m_topListInfo.clear();
    deleteAll();

    QNetworkRequest request;
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    makeTokenQueryUrl(&request,
                      MusicUtils::Algorithm::mdII(XM_SONG_TOPLIST_DATA_URL, false),
                      MusicUtils::Algorithm::mdII(XM_SONG_TOPLIST_URL, false));
    if(!m_manager || m_stateCode != MusicNetworkAbstract::Init) return;
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicXMDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        bytes = bytes.replace("xiami(", "");
        bytes = bytes.replace("callback(", "");
        bytes.chop(1);

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                value = value["data"].toMap();
                QVariantList datas = value["songs"].toList();
                int where = datas.count();
                where = (where > 0) ? qrand()%where : 0;

                int counter = 0;
                foreach(const QVariant &var, datas)
                {
                    if((where != counter++) || var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    m_topListInfo = QString("%1 %2").arg(value["artistName"].toString())
                                                    .arg(value["songName"].toString());
                }
            }
        }
    }

    emit downLoadDataChanged(m_topListInfo);
    deleteAll();
}
