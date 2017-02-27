#include "musicwydiscoverlistthread.h"
#include "musicdownloadwyinterface.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

MusicWYDiscoverListThread::MusicWYDiscoverListThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    MusicTime::timeSRand();
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

MusicWYDiscoverListThread::~MusicWYDiscoverListThread()
{
    deleteAll();
}

QString MusicWYDiscoverListThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWYDiscoverListThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

void MusicWYDiscoverListThread::startSearchSong()
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(WY_SONG_TOPLIST_URL, URL_KEY);
    deleteAll();

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWYDiscoverListThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    m_topListInfo.clear();
    if(m_reply->error() == QNetworkReply::NoError)
    {
        MusicObject::MIntSet ids;
        while(m_reply->canReadLine())
        {
            QString text = m_reply->readLine();
            QRegExp regx(QString("/song\\?id=(\\d+)"));
            int pos = QString(text).indexOf(regx);
            while(pos != -1)
            {
                ids << regx.cap(1).toInt();
                pos += regx.matchedLength();
                pos = regx.indexIn(text, pos);
            }
        }

        if(!ids.isEmpty())
        {
            int id = ids.toList()[qrand()%ids.count()];
            searchTopListInformation(QString::number(id));
            return;
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicWYDiscoverListThread::searchTopListInfoFinished()
{
    QNetworkReply *reply = MObject_cast(QNetworkReply*, QObject::sender());
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("code") && value.contains("songs") && value["code"].toInt() == 200)
            {
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    QVariantList artistsArray = value["artists"].toList();
                    foreach(const QVariant &artistValue, artistsArray)
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        QVariantMap artistMap = artistValue.toMap();
                        m_topListInfo = artistMap["name"].toString();
                    }

                    m_topListInfo += " " + value["name"].toString();
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicWYDiscoverListThread::searchTopListInformation(const QString &id)
{
    QNetworkRequest request;
    request.setUrl(QUrl(MusicCryptographicHash::decryptData(WY_SONG_URL, URL_KEY).arg(id)));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Origin", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
    request.setRawHeader("Referer", MusicCryptographicHash::decryptData(WY_BASE_URL, URL_KEY).toUtf8());
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    QNetworkReply *reply = m_manager->get(request);
    connect(reply, SIGNAL(finished()), SLOT(searchTopListInfoFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
