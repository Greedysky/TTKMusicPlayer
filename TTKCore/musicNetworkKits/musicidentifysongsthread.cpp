#include "musicidentifysongsthread.h"
#include "musicdownloadsourcethread.h"
#include "musicsemaphoreloop.h"
#include "musicalgorithmutils.h"
#///QJson import
#include "qjson/parser.h"

#include <QFile>

#define QUERY_URL     "VzBxZCtBUDBKK1R6aHNiTGxMdy84SzlIUVA5a3cvbjdKQ1ZIVGdYRThBS0hZMTlZSnhRQ0Y5N0lZdi9QQ3VveVEyVDdXbll3ZUZvPQ=="
#define ACRUA_URL     "U0NDS2dzQVpNM3V3aGhtZmVOdW83d2VielBHdTcxTEk4QmpTSUxvQ2ZsdWVSaE1wWW5EOG8wMm83bitKVlJnZHZ1dlBSUT09"

MusicIdentifySongsThread::MusicIdentifySongsThread(QObject *parent)
    : MusicNetworkAbstract(parent)
{
    m_manager = new QNetworkAccessManager(this);
#ifndef QT_NO_SSL
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                       SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    M_LOGGER_INFO(QString("%1 Support ssl: %2").arg(getClassName()).arg(QSslSocket::supportsSsl()));
#endif
}

QString MusicIdentifySongsThread::getClassName()
{
    return staticMetaObject.className();
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
    download->startToDownload(MusicUtils::Algorithm::mdII(ACRUA_URL, false));

    loop.exec();

    return !m_accessKey.isEmpty() && !m_accessSecret.isEmpty();
}

void MusicIdentifySongsThread::startToDownload(const QString &path)
{
    QString boundary = "----";
    QString startBoundary = "--" + boundary;
    QString endBoundary = "\r\n--" + boundary + "--\r\n";
    QString contentType = "multipart/form-data; boundary=" + boundary;

    QString httpMethod = "POST";
    QString httpUri = "/v1/identify";
    QString dataType = "audio";
    QString signatureVersion = "1";
    QString timeStamp = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

    httpMethod = httpMethod + "\n" + httpUri + "\n" + m_accessKey + "\n" + dataType + "\n" + signatureVersion + "\n" + timeStamp;
    QByteArray content = hmacSha1(httpMethod.toUtf8(), m_accessSecret.toUtf8()).toBase64();

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
#ifndef QT_NO_SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
#endif
    m_reply = m_manager->post(request, content);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
}

const MusicSongIdentifys& MusicIdentifySongsThread::getIdentifySongs() const
{
    return m_songIdentifys;
}

void MusicIdentifySongsThread::downLoadFinished()
{
    m_songIdentifys.clear();
    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("metadata"))
            {
                value = value["metadata"].toMap();
                QVariantList list = value["music"].toList();
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
    QVariant dt = parser.parse(data, &ok);
    if(ok)
    {
        QVariantMap value = dt.toMap();
        if( QDateTime::fromString( value["time"].toString(), "yyyy-MM-dd HH:mm:ss") >
            QDateTime::currentDateTime())
        {
            m_accessKey = value["key"].toString();
            m_accessSecret = value["secret"].toString();
        }
    }
    emit getKeyFinished();
}

QByteArray MusicIdentifySongsThread::hmacSha1(const QByteArray &data, const QByteArray &secretKey)
{
    int blockSize = 64;
    QByteArray newSecretKey = secretKey;
    if(newSecretKey.length() > blockSize)
    {
        newSecretKey = MusicUtils::Algorithm::sha1(newSecretKey);
    }

    QByteArray innerPadding(blockSize, char(0x36));
    QByteArray outerPadding(blockSize, char(0x5c));

    for(int i = 0; i < secretKey.length(); i++)
    {
        innerPadding[i] = innerPadding[i] ^ secretKey.at(i);
        outerPadding[i] = outerPadding[i] ^ secretKey.at(i);
    }

    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(data);
    total.append(MusicUtils::Algorithm::sha1(part));
    QByteArray hashed = MusicUtils::Algorithm::sha1(total);
    return hashed;
}
