#include "musicidentifysongsrequest.h"
#include "musicdownloadsourcerequest.h"
#include "musicsettingmanager.h"

#include "qsync/qsyncutils.h"

#include <QFile>

#define QUERY_URL     "VzBxZCtBUDBKK1R6aHNiTGxMdy84SzlIUVA5a3cvbjdKQ1ZIVGdYRThBS0hZMTlZSnhRQ0Y5N0lZdi9QQ3VveVEyVDdXbll3ZUZvPQ=="
#define OS_ACRUA_URL  "acrcloud"

MusicIdentifySongsRequest::MusicIdentifySongsRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

bool MusicIdentifySongsRequest::queryIdentifyKey()
{
    MusicSemaphoreLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

    MusicDownloadSourceRequest *d = new MusicDownloadSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startRequest(QSyncUtils::makeDataBucketUrl() + OS_ACRUA_URL);
    loop.exec();

    return !m_accessKey.isEmpty() && !m_accessSecret.isEmpty();
}

void MusicIdentifySongsRequest::startRequest(const QString &path)
{
    const QString &boundary = "----";
    const QString &start = "--" + boundary;
    const QString &end = "\r\n--" + boundary + "--\r\n";
    const QString &contentType = "multipart/form-data; boundary=" + boundary;

    const QString &method = "POST";
    const QString &endpoint = "/v1/identify";
    const QString &type = "fingerprint";
    const QString &version = "1";
    const QString &timeStamp = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());

    const QString &sign = method + "\n" + endpoint + "\n" + m_accessKey + "\n" + type + "\n" + version + "\n" + timeStamp;
    QByteArray body = MusicUtils::Algorithm::hmacSha1(sign.toUtf8(), m_accessSecret.toUtf8()).toBase64();

    QString value;
    value += start + "\r\nContent-Disposition: form-data; name=\"access_key\"\r\n\r\n" + m_accessKey + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"data_type\"\r\n\r\n" + type + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"timestamp\"\r\n\r\n" + timeStamp + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"signature_version\"\r\n\r\n" + version + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"signature\"\r\n\r\n" + body + "\r\n";

    QFile file(path);
    if(!file.open(QFile::ReadOnly))
    {
        TTK_LOGGER_ERROR("Load input audio wav file error");
        return;
    }

    value += start + "\r\nContent-Disposition: form-data; name=\"sample_bytes\"\r\n\r\n" + QString::number(file.size()) + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"sample\"; filename=\"" + file.fileName() + "\"\r\n";
    value += "Content-Type: application/octet-stream\r\n\r\n";

    body.clear();
    body.append(value.toUtf8());
    body.append(file.readAll());
    body.append(end.toUtf8());
    file.close();

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(QUERY_URL, false));
    MusicObject::setSslConfiguration(&request);
    request.setRawHeader("Content-Type", contentType.toUtf8());

    m_reply = m_manager.post(request, body);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError);
}

void MusicIdentifySongsRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    m_songIdentifys.clear();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("metadata"))
            {
                value = value["metadata"].toMap();
                const QVariantList &datas = value["music"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    value = var.toMap();

                    MusicSongIdentifyData song;
                    song.m_songName = value["title"].toString();
                    for(const QVariant &artists : value["artists"].toList())
                    {
                        value = artists.toMap();
                        song.m_singerName = value["name"].toString();
                        break;
                    }
                    m_songIdentifys << song;
                    break;
                }
            }
            else
            {
                TTK_LOGGER_INFO("No result in acrcloud server");
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}

void MusicIdentifySongsRequest::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_LOGGER_ERROR("Input byte data is empty");
    }
    else
    {
        QJson::Parser json;
        bool ok;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            const QVariantMap &value = data.toMap();
            if(QDateTime::fromString(value["time"].toString(), TTK_YEAR_STIME_FORMAT) > QDateTime::currentDateTime())
            {
                m_accessKey = value["key"].toString();
                m_accessSecret = value["secret"].toString();
            }
        }
    }

    Q_EMIT finished();
}
