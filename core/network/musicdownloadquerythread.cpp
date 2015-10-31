#include "musicdownloadquerythread.h"
#include "musicconnectionpool.h"
#include "musicdownloadthreadabstract.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>

MusicDownLoadQueryThread::MusicDownLoadQueryThread(QObject *parent)
    : QObject(parent),m_reply(NULL)
{
    m_searchQuality = "标准品质";
    m_manager = new QNetworkAccessManager(this);
    M_CONNECTION->setValue("MusicDownLoadQueryThread", this);
    M_CONNECTION->connect("MusicDownLoadQueryThread", "MusicDownloadStatusLabel");
}

MusicDownLoadQueryThread::~MusicDownLoadQueryThread()
{
    M_CONNECTION->disConnect("MusicDownLoadQueryThread");
    deleteAll();///The release of all the objects
    if(m_manager)
    {
        m_manager->deleteLater();
        m_manager = NULL;
    }
}

void MusicDownLoadQueryThread::deleteAll()
{
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }
}

void MusicDownLoadQueryThread::startSearchSong(QueryType type, const QString &text)
{
    m_searchText = text.trimmed();
    m_currentType = type;

    QUrl musicUrl = (type == MusicQuery) ? MUSIC_REQUERY_URL.arg(text)
                                         : MV_REQUERY_URL.arg(text);
    ///This is a ttop music API
    if(m_reply)
    {
        m_reply->deleteLater();
        m_reply = NULL;
    }
    m_reply = m_manager->get(QNetworkRequest(musicUrl));
    connect(m_reply, SIGNAL(finished()), SLOT(searchFinshed()) );
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     SLOT(replyError(QNetworkReply::NetworkError)) );
}

void MusicDownLoadQueryThread::searchFinshed()
{
    if(m_reply == NULL)
    {
        return;
    }
    m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();///Get all the data obtained by request
        QJsonParseError jsonError;
        QJsonDocument parseDoucment = QJsonDocument::fromJson(bytes, &jsonError);
        ///Put the data into Json
        if(jsonError.error != QJsonParseError::NoError ||
           !parseDoucment.isObject())
        {
            return ;
        }

        emit clearAllItems();     ///Clear origin items
        m_musicSongInfo.clear();  ///Empty the last search to songsInfo
        QJsonObject jsonObject = parseDoucment.object();

        if(jsonObject.contains("data"))
        {
            QJsonArray array = jsonObject.take("data").toArray();
            for(int i=0; i < array.count(); i++ )
            {
                QJsonValue value = array.at(i);
                if(!value.isObject())
                {
                   continue ;
                }
                QJsonObject object = value.toObject();

                DownloadSongInfo musicInfo;
                if(m_currentType == MusicQuery)
                {
                    QString songId = QString::number(object.take("song_id").toVariant().toULongLong());
                    QString songName = object.take("song_name").toString();
                    QString singerName = object.take("singer_name").toString();
                    QJsonArray urls = object.take("audition_list").toArray();
                    for(int j=0; j<urls.count(); ++j)
                    {
                        object = urls[j].toObject();
                        if( object.value("type_description").toString() == m_searchQuality)
                        {
                            emit creatSearchedItems(songName, singerName,
                                                    object.value("duration").toString());
                            SongUrlFormat urlFormat;
                            urlFormat.m_format = m_searchQuality;
                            urlFormat.m_url = object.value("url").toString();
                            musicInfo.m_songUrl << urlFormat;

                            musicInfo.m_lrcUrl = MUSIC_LRC_URL.arg(singerName).arg(songName).arg(songId);
                            musicInfo.m_smallPicUrl = SML_BG_ART_URL.arg(singerName);
                            musicInfo.m_singerName = singerName;
                            musicInfo.m_songName = songName;
                            musicInfo.m_size = object.value("size").toString();
                            musicInfo.m_format = object.value("format").toString();
                            m_musicSongInfo << musicInfo;
                            break;
                        }
                    }
                }
                else
                {
                    QString songName = object.take("videoName").toString();
                    QString singerName = object.take("singerName").toString();
                    QJsonArray urls = object.take("mvList").toArray();
                    if( urls.count() > 0)
                    {
                        object = urls[0].toObject();
                        emit creatSearchedItems(songName, singerName,
                                                object.value("duration").toString());
                        for(int i=0; i<urls.count(); ++i)
                        {
                            object = urls[i].toObject();
                            SongUrlFormat urlFormat;
                            urlFormat.m_format = QString::number(object.value("bitRate").toInt());
                            urlFormat.m_url = object.value("url").toString();
                            musicInfo.m_songUrl << urlFormat;
                        }
                        musicInfo.m_singerName = singerName;
                        musicInfo.m_songName = songName;
                        musicInfo.m_format = object.value("suffix").toString();
                        m_musicSongInfo << musicInfo;
                    }
                }
            }
            ///If there is no search to song_id, is repeated several times in the search
            ///If more than 5 times or no results give up
            static int counter = 5;
            if(m_musicSongInfo.isEmpty() && counter-- > 0)
            {
                startSearchSong(m_currentType, m_searchText);
            }
            else
            {
                M_LOOGER << "not find the song_Id";
            }
        }
    }
    emit resolvedSuccess();
}

void MusicDownLoadQueryThread::replyError(QNetworkReply::NetworkError)
{
    M_LOOGER << "Abnormal network connection";
    deleteAll();
}
