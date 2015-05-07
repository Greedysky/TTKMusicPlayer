#ifndef MUSICDOWNLOADMANAGERTHREAD_H
#define MUSICDOWNLOADMANAGERTHREAD_H

#include <QObject>
#include <QNetworkReply>
#include "musiclibexportglobal.h"

class QNetworkAccessManager;

enum DownLoadType{ DisConnection, DownLoading, Buffing, Waiting };

class MUSIC_EXPORT MusicDownLoadManagerThread : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownLoadManagerThread(QObject *parent = 0);
    virtual ~MusicDownLoadManagerThread();

    void deleteAll();
    void startSearchSong(const QString&);
    void startSearchSongId();
    inline int getSongIdIndex() const { return m_songIdIndex;}
    inline QList< QStringList >& getMusicSongInfo(){ return m_musicSongInfo;}

signals:
    void showDownLoadInfoFor(DownLoadType);
    void showDownLoadInfoFinished(const QString&);
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, double);

public slots:
    void searchFinshed();
    void songIdSearchFinshed();
    void replyError(QNetworkReply::NetworkError);
    void songIdReplyError(QNetworkReply::NetworkError);

private:
    QNetworkAccessManager *m_manager, *m_songIdManager;
    QNetworkReply *m_reply, *m_songIdReply;
    QStringList m_songIdList;     /// save song_id
    QList< QStringList > m_musicSongInfo;
    ///music-link and lrc-link pc-link and name
    QString m_searchText;
    int m_songIdIndex;   ///The index of the corresponding songIdList length

};

#endif // MUSICDOWNLOADMANAGERTHREAD_H
