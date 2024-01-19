#include "musicidentifysongrequest.h"
#include "musicdatasourcerequest.h"

#include "qsync/qsyncutils.h"

#include <QFile>

static constexpr const char *OS_ACRUA_URL = "acrcloud";
static constexpr const char *QUERY_URL = "VzBxZCtBUDBKK1R6aHNiTGxMdy84SzlIUVA5a3cvbjdKQ1ZIVGdYRThBS0hZMTlZSnhRQ0Y5N0lZdi9QQ3VveVEyVDdXbll3ZUZvPQ==";

MusicIdentifySongRequest::MusicIdentifySongRequest(QObject *parent)
    : MusicAbstractNetwork(parent)
{

}

bool MusicIdentifySongRequest::queryIdentifyKey()
{
    TTKSemaphoreLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

    MusicDataSourceRequest *d = new MusicDataSourceRequest(this);
    connect(d, SIGNAL(downLoadRawDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    d->startToRequest(QSyncUtils::makeDataBucketUrl() + OS_ACRUA_URL);
    loop.exec();

    return !m_accessKey.isEmpty() && !m_accessSecret.isEmpty();
}

void MusicIdentifySongRequest::startToRequest(const QString &path)
{
    const QString &boundary = "----";
    const QString &start = "--" + boundary;
    const QString &end = "\r\n--" + boundary + "--\r\n";
    const QString &contentType = "multipart/form-data; boundary=" + boundary;

    const QString &method = "POST";
    const QString &endpoint = "/v1/identify";
    const QString &type = "fingerprint";
    const QString &version = "1";
    const QString &timeStamp = QString::number(TTKDateTime::currentTimestamp());

    const QString &sign = method + "\n" + endpoint + "\n" + m_accessKey + "\n" + type + "\n" + version + "\n" + timeStamp;
    QByteArray body = TTK::Algorithm::hmacSha1(sign.toUtf8(), m_accessSecret.toUtf8()).toBase64();

    QString value;
    value += start + "\r\nContent-Disposition: form-data; name=\"access_key\"\r\n\r\n" + m_accessKey + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"data_type\"\r\n\r\n" + type + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"timestamp\"\r\n\r\n" + timeStamp + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"signature_version\"\r\n\r\n" + version + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"signature\"\r\n\r\n" + body + "\r\n";

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        TTK_ERROR_STREAM("Load input audio wav file error");
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
    request.setUrl(TTK::Algorithm::mdII(QUERY_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::makeContentTypeHeader(&request, contentType.toUtf8());

    m_reply = m_manager.post(request, body);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicIdentifySongRequest::downLoadFinished()
{
    MusicAbstractNetwork::downLoadFinished();
    m_items.clear();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser json;
        bool ok = false;
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
                    TTK_NETWORK_QUERY_CHECK();

                    MusicSongIdentifyData song;
                    song.m_songName = value["title"].toString();
                    for(const QVariant &artists : value["artists"].toList())
                    {
                        value = artists.toMap();
                        song.m_artistName = value["name"].toString();
                        break;
                    }

                    m_items << song;
                    break;
                }
            }
            else
            {
                TTK_INFO_STREAM("No result in acrcloud server");
            }
        }
    }

    Q_EMIT downLoadDataChanged({});
    deleteAll();
}

void MusicIdentifySongRequest::downLoadFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input byte data is empty");
    }
    else
    {
        QJson::Parser json;
        bool ok = false;
        const QVariant &data = json.parse(bytes, &ok);
        if(ok)
        {
            const QVariantMap &value = data.toMap();
            if(QDateTime::fromString(value["time"].toString(), TTK_YEAR_TIMEZ_FORMAT) > QDateTime::currentDateTime())
            {
                m_accessKey = value["key"].toString();
                m_accessSecret = value["secret"].toString();
            }
        }
    }

    Q_EMIT finished();
}
