#include "musicidentifysongsthread.h"
#include "musicdownloadsourcethread.h"
#include "musicsemaphoreloop.h"
#include "musicalgorithmutils.h"
#include "musicsourceupdatethread.h"
#include "musicsettingmanager.h"
#///QJson import
#include "qjson/parser.h"

#include <QFile>

#define QUERY_URL     "VzBxZCtBUDBKK1R6aHNiTGxMdy84SzlIUVA5a3cvbjdKQ1ZIVGdYRThBS0hZMTlZSnhRQ0Y5N0lZdi9QQ3VveVEyVDdXbll3ZUZvPQ=="
#define QN_ACRUA_URL  "acrcloud"

MusicIdentifySongsThread::MusicIdentifySongsThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

void MusicIdentifySongsThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
}

bool MusicIdentifySongsThread::getKey()
{
    MusicSemaphoreLoop loop;
    connect(this, SIGNAL(getKeyFinished()), &loop, SLOT(quit()));

    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(keyDownLoadFinished(QByteArray)));
    const QString &buketUrl = M_SETTING_PTR->value(MusicSettingManager::QiNiuDataConfigChoiced).toString();
    download->startToDownload(MusicUtils::Algorithm::mdII(buketUrl, false) + QN_ACRUA_URL);

    loop.exec();

    return !m_accessKey.isEmpty() && !m_accessSecret.isEmpty();
}

void MusicIdentifySongsThread::startToDownload(const QString &path)
{
    const QString &boundary = "----";
    const QString &startBoundary = "--" + boundary;
    const QString &endBoundary = "\r\n--" + boundary + "--\r\n";
    const QString &contentType = "multipart/form-data; boundary=" + boundary;

    const QString &httpMethod = "POST";
    const QString &httpUri = "/v1/identify";
    const QString &dataType = "audio";
    const QString &signatureVersion = "1";
    const QString &timeStamp = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

    const QString &postData = httpMethod + "\n" + httpUri + "\n" + m_accessKey + "\n" + dataType + "\n" + signatureVersion + "\n" + timeStamp;
    QByteArray content = MusicUtils::Algorithm::hmacSha1(postData.toUtf8(), m_accessSecret.toUtf8()).toBase64();

    QString value;
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"access_key\"\r\n\r\n" + m_accessKey + "\r\n";
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"data_type\"\r\n\r\n" + dataType + "\r\n";
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"timestamp\"\r\n\r\n" + timeStamp + "\r\n";
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"signature_version\"\r\n\r\n" + signatureVersion + "\r\n";
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"signature\"\r\n\r\n" + content + "\r\n";

    QFile file(path);
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"sample_bytes\"\r\n\r\n" + QString::number(file.size()) + "\r\n";
    value += startBoundary + "\r\nContent-Disposition: form-data; name=\"sample\"; filename=\"" + file.fileName() + "\"\r\n";
    value += "Content-Type: application/octet-stream\r\n\r\n";

    content.clear();
    content.append(value.toUtf8());
    content.append(file.readAll());
    content.append(endBoundary);

    QNetworkRequest request;
    request.setUrl(QUrl(MusicUtils::Algorithm::mdII(QUERY_URL, false)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->post(request, content);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

void MusicIdentifySongsThread::downLoadFinished()
{
    m_songIdentifys.clear();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("metadata"))
            {
                value = value["metadata"].toMap();
                const QVariantList &list = value["music"].toList();
                foreach(const QVariant &var, list)
                {
                    value = var.toMap();

                    MusicSongIdentify song;
                    song.m_songName = value["title"].toString();
                    foreach(const QVariant &artists, value["artists"].toList())
                    {
                        value = artists.toMap();
                        song.m_singerName = value["name"].toString();
                        break;
                    }
                    m_songIdentifys << song;
                    break;
                }
            }
        }
    }

    emit downLoadDataChanged( QString() );
    deleteAll();
}

void MusicIdentifySongsThread::keyDownLoadFinished(const QByteArray &data)
{
    QJson::Parser parser;
    bool ok;
    const QVariant &dt = parser.parse(data, &ok);
    if(ok)
    {
        const QVariantMap &value = dt.toMap();
        if(QDateTime::fromString( value["time"].toString(), "yyyy-MM-dd HH:mm:ss") > QDateTime::currentDateTime())
        {
            m_accessKey = value["key"].toString();
            m_accessSecret = value["secret"].toString();
        }
    }
    emit getKeyFinished();
}
