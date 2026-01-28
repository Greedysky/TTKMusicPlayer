#include "musicidentifysongrequest.h"
#include "musicdatasourcerequest.h"

#include "qsync/qsyncutils.h"

#include <QFile>

static constexpr const char *OS_ACRCLOUD_URL = "acrcloud";
static constexpr const char *ACR_QUERY_URL = "VzBxZCtBUDBKK1R6aHNiTGxMdy84SzlIUVA5a3cvbjdKQ1ZIVGdYRThBS0hZMTlZSnhRQ0Y5N0lZdi9QQ3VveVEyVDdXbll3ZUZvPQ==";
static constexpr const char *XF_QUERY_URL = "cFJiZUh0Z3FxalV6NzlBcEhUVmN5VGRzMm5NZ01rbzlWaFUzRE1ubzBoTU53WlI5cEZpNnVxT3l6OEE9";
static constexpr const char *XF_QUERY_BODY = "VHd2bFMwaTZGNFQxeDBuU0prYXE0SDIzOVYyWlVwMWk=";
static constexpr const char *XF_QUERY_DATA = "a29sU1NRVExPbzIzRDdiNjRlbWUwdz09";

MusicAbstractIdentifyRequest::MusicAbstractIdentifyRequest(QObject *parent)
    : MusicAbstractNetwork(parent),
      m_items(nullptr)
{

}


MusicACRIdentifyRequest::MusicACRIdentifyRequest(QObject *parent)
    : MusicAbstractIdentifyRequest(parent)
{

}

void MusicACRIdentifyRequest::startToRequest(const QString &path, const QVariantMap &keys)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        TTK_ERROR_STREAM("Load input audio wav file error");
        return;
    }

    const QString &accessKey = keys["key"].toString();
    const QString &accessSecret = keys["secret"].toString();

    const QString &boundary = "----";
    const QString &start = "--" + boundary;
    const QString &end = "\r\n--" + boundary + "--\r\n";
    const QString &contentType = "multipart/form-data; boundary=" + boundary;

    const QString &method = "POST";
    const QString &endpoint = "/v1/identify";
    const QString &type = "fingerprint";
    const QString &version = "1";
    const QString &timeStamp = QString::number(TTKDateTime::currentTimestamp());

    const QString &sign = method + "\n" + endpoint + "\n" + accessKey + "\n" + type + "\n" + version + "\n" + timeStamp;
    QByteArray body = TTK::Algorithm::hmacSha1(sign.toUtf8(), accessSecret.toUtf8()).toBase64();

    QString value;
    value += start + "\r\nContent-Disposition: form-data; name=\"access_key\"\r\n\r\n" + accessKey + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"data_type\"\r\n\r\n" + type + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"timestamp\"\r\n\r\n" + timeStamp + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"signature_version\"\r\n\r\n" + version + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"signature\"\r\n\r\n" + body + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"sample_bytes\"\r\n\r\n" + QString::number(file.size()) + "\r\n";
    value += start + "\r\nContent-Disposition: form-data; name=\"sample\"; filename=\"" + file.fileName() + "\"\r\n";
    value += "Content-Type: application/octet-stream\r\n\r\n";

    body.clear();
    body.append(value.toUtf8());
    body.append(file.readAll());
    body.append(end.toUtf8());
    file.close();

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(ACR_QUERY_URL, false));
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request, contentType.toUtf8());

    m_reply = m_manager.post(request, body);
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicACRIdentifyRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    m_items->clear();
    MusicAbstractIdentifyRequest::downloadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
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
                        song.m_artistName = TTK::String::charactersReplace(value["name"].toString());
                        break;
                    }

                    m_items->append(song);
                    Q_EMIT downloadDataChanged("Found result");
                    deleteAll();
                    return;
                }
            }
            else
            {
                TTK_INFO_STREAM("No result in acrcloud server");
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}


MusicXFIdentifyRequest::MusicXFIdentifyRequest(QObject *parent)
    : MusicAbstractIdentifyRequest(parent)
{

}

void MusicXFIdentifyRequest::startToRequest(const QString &path, const QVariantMap &keys)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        TTK_ERROR_STREAM("Load input audio wav file error");
        return;
    }

    const QString &accessKey = keys["xfid"].toString();
    const QString &accessSecret = keys["xfkey"].toString();

    const QByteArray &data = file.readAll();
    file.close();

    const QByteArray &timeStamp = QByteArray::number(TTKDateTime::currentTimestamp() / 1000);
    const QByteArray &audioBody = TTK::Algorithm::mdII(XF_QUERY_BODY, false).toUtf8().toBase64();
    const QByteArray &md5 = TTK::Algorithm::md5(accessSecret.toUtf8() + timeStamp + audioBody);

    QNetworkRequest request;
    request.setUrl(TTK::Algorithm::mdII(XF_QUERY_URL, false));
    request.setRawHeader("X-CurTime", timeStamp);
    request.setRawHeader("X-Param", audioBody);
    request.setRawHeader("X-Appid", accessKey.toUtf8());
    request.setRawHeader("X-CheckSum", md5);
    TTK::setSslConfiguration(&request);
    TTK::setContentTypeHeader(&request);

    m_reply = m_manager.post(request, TTK::Algorithm::mdII(XF_QUERY_DATA, false).arg(data.toBase64().constData()).toUtf8());
    connect(m_reply, SIGNAL(finished()), SLOT(downloadFinished()));
    QtNetworkErrorConnect(m_reply, this, replyError, TTK_SLOT);
}

void MusicXFIdentifyRequest::downloadFinished()
{
    TTK_INFO_STREAM(metaObject()->className() << __FUNCTION__);

    m_items->clear();
    MusicAbstractIdentifyRequest::downloadFinished();

    if(m_reply && m_reply->error() == QNetworkReply::NoError)
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(m_reply->readAll(), &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            QVariantMap value = json.toVariant().toMap();
            if(value["code"].toInt() == 0)
            {
                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    value = var.toMap();
                    TTK_NETWORK_QUERY_CHECK();

                    MusicSongIdentifyData song;
                    song.m_songName = value["song"].toString();
                    song.m_artistName = value["singer"].toString();

                    m_items->append(song);
                    Q_EMIT downloadDataChanged("Found result");
                    deleteAll();
                    return;
                }
            }
            else
            {
                TTK_INFO_STREAM("No result in xf server");
            }
        }
    }

    Q_EMIT downloadDataChanged({});
    deleteAll();
}


MusicIdentifySongRequest::MusicIdentifySongRequest(QObject *parent)
    : MusicAbstractNetwork(parent),
      m_pluginSelector(-1)
{

}

bool MusicIdentifySongRequest::initialize()
{
    TTKEventLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));

    MusicDataSourceRequest *req = new MusicDataSourceRequest(this);
    connect(req, SIGNAL(downloadRawDataChanged(QByteArray)), SLOT(downloadKeyFinished(QByteArray)));
    req->startToRequest(QSyncUtils::makeDataBucketUrl() + OS_ACRCLOUD_URL);
    loop.exec();

    return !m_rawData.isEmpty();
}

void MusicIdentifySongRequest::startToRequest(const QString &path)
{
    m_path = path;
    m_pluginSelector = -1;
    findAllPlugins();
}

void MusicIdentifySongRequest::downloadDataFinished(const QString &bytes)
{
    if(bytes.isEmpty())
    {
        findAllPlugins();
        return;
    }

    Q_EMIT downloadDataChanged({});
}

void MusicIdentifySongRequest::findAllPlugins()
{
    MusicAbstractIdentifyRequest *req = nullptr;
    switch(++m_pluginSelector)
    {
        case 0: req = new MusicXFIdentifyRequest(this); break;
        case 1: req = new MusicACRIdentifyRequest(this); break;
        default: Q_EMIT downloadDataChanged({}); break;
    }

    if(req)
    {
        connect(req, SIGNAL(downloadDataChanged(QString)), SLOT(downloadDataFinished(QString)));
        //
        req->setItems(&m_items);
        req->startToRequest(m_path, m_rawData);
    }
}

void MusicIdentifySongRequest::downloadKeyFinished(const QByteArray &bytes)
{
    if(bytes.isEmpty())
    {
        TTK_ERROR_STREAM("Input key byte data is empty");
    }
    else
    {
        QJsonParseError ok;
        const QJsonDocument &json = QJsonDocument::fromJson(bytes, &ok);
        if(QJsonParseError::NoError == ok.error)
        {
            m_rawData = json.toVariant().toMap();
        }
    }

    Q_EMIT finished();
}
