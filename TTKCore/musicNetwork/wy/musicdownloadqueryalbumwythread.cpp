#include "musicdownloadqueryalbumwythread.h"
#include "musicdownloadquerywythread.h"
#include "musicnumberutils.h"
#include "musictime.h"
#///QJson import
#include "qjson/parser.h"

#include <QDateTime>

MusicDownLoadQueryAlbumWYThread::MusicDownLoadQueryAlbumWYThread(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{

}

QString MusicDownLoadQueryAlbumWYThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDownLoadQueryAlbumWYThread::startSearchSong(QueryType type, const QString &text)
{
    Q_UNUSED(type);
    startSearchSong(text);
}

void MusicDownLoadQueryAlbumWYThread::startSearchSong(const QString &album)
{
    QUrl musicUrl = MusicCryptographicHash::decryptData(WY_SONG_ALBUM_URL, URL_KEY).arg(album);

    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = nullptr;
    }

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
    m_reply = m_manager->get( request );
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryAlbumWYThread::downLoadFinished()
{
    if(m_reply == nullptr)
    {
        deleteAll();
        return;
    }

    emit clearAllItems();      ///Clear origin items
    m_musicSongInfos.clear();  ///Empty the last search to songsInfo

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll(); ///Get all the data obtained by request

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["code"].toInt() == 200 && value.contains("album"))
            {
                value = value["album"].toMap();
                QString albumInfo = value["name"].toString() + "<>" +
                                    value["language"].toString() + "<>" +
                                    value["company"].toString() + "<>" +
                        QDateTime::fromMSecsSinceEpoch(value["publishTime"].toULongLong())
                                     .toString("yyyy-MM-dd");
                QVariantList datas = value["songs"].toList();
                foreach(const QVariant &var, datas)
                {
                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    MusicObject::MusicSongInfomation info;
                    info.m_songName = value["name"].toString();
                    info.m_timeLength = MusicTime::msecTime2LabelJustified(value["duration"].toInt());
                    info.m_lrcUrl = MusicCryptographicHash::decryptData(WY_SONG_LRC_URL, URL_KEY).arg(value["id"].toInt());

                    QVariantMap albumObject = value["album"].toMap();
                    info.m_smallPicUrl = albumObject["picUrl"].toString();
                    info.m_albumId = albumInfo;

                    QVariantList artistsArray = value["artists"].toList();
                    foreach(const QVariant &artistValue, artistsArray)
                    {
                        if(artistValue.isNull())
                        {
                            continue;
                        }
                        QVariantMap artistMap = artistValue.toMap();
                        info.m_singerName = artistMap["name"].toString();
                    }

                    if(m_queryAllRecords)
                    {
                        readFromMusicSongAttribute(&info, value["lMusic"].toMap(), MB_32);
                        readFromMusicSongAttribute(&info, value["bMusic"].toMap(), MB_128);
                        readFromMusicSongAttribute(&info, value["mMusic"].toMap(), MB_192);
                        readFromMusicSongAttribute(&info, value["hMusic"].toMap(), MB_320);
                    }
                    else
                    {
                        if(m_searchQuality == tr("ST"))
                            readFromMusicSongAttribute(&info, value["lMusic"].toMap(), MB_32);
                        if(m_searchQuality == tr("SD"))
                            readFromMusicSongAttribute(&info, value["bMusic"].toMap(), MB_128);
                        else if(m_searchQuality == tr("HD"))
                            readFromMusicSongAttribute(&info, value["mMusic"].toMap(), MB_192);
                        else if(m_searchQuality == tr("SQ"))
                            readFromMusicSongAttribute(&info, value["hMusic"].toMap(), MB_320);
                    }

                    if(info.m_songAttrs.isEmpty())
                    {
                        continue;
                    }

                    emit createSearchedItems(info.m_songName, info.m_singerName, info.m_timeLength);
                    m_musicSongInfos << info;
                }
            }
        }
        else
        {
            emit downLoadDataChanged(QString());
            deleteAll();
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
}

void MusicDownLoadQueryAlbumWYThread::readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                                                 const QVariantMap &key, int bitrate)
{
    MusicObject::MusicSongAttribute attr;
    qlonglong dfsId = key.value("dfsId").toLongLong();
    attr.m_bitrate = bitrate;
    attr.m_format = key.value("extension").toString();
    attr.m_size = MusicUtils::Number::size2Label(key.value("size").toInt());
    attr.m_url = MusicCryptographicHash::decryptData(WY_SONG_PATH_URL, URL_KEY).arg(encryptedId(dfsId)).arg(dfsId);
    info->m_songAttrs.append(attr);
}

QString MusicDownLoadQueryAlbumWYThread::encryptedId(qlonglong id)
{
    return encryptedId(QString::number(id));
}

QString MusicDownLoadQueryAlbumWYThread::encryptedId(const QString &string)
{
    QByteArray array1(WY_ENCRYPT_STRING);
    QByteArray array2 = string.toUtf8();
    int length = array1.length();
    for(int i=0; i<array2.length(); ++i)
    {
        array2[i] = array2[i]^array1[i%length];
    }

    QByteArray encodedData = QCryptographicHash::hash(array2, QCryptographicHash::Md5);
#if (defined MUSIC_GREATER_NEW && !defined MUSIC_NO_WINEXTRAS)
    encodedData = encodedData.toBase64(QByteArray::Base64UrlEncoding);
#else
    encodedData = encodedData.toBase64();
    encodedData.replace('+', '-');
    encodedData.replace('/', '_');
#endif
    return encodedData;
}
