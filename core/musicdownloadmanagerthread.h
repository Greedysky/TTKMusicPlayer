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
    inline int getSongIdIndex() const { return m_songIdIndex;}
    inline QList< QStringList >& getMusicSongInfo(){ return m_musicSongInfo;}

signals:
    void showDownLoadInfoFor(DownLoadType);
    void showDownLoadInfoFinished(const QString&);
    void clearAllItems();
    void creatSearchedItems(const QString&, const QString&, const QString&);

public slots:
    void searchFinshed();
    void replyError(QNetworkReply::NetworkError);

protected:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QList< QStringList > m_musicSongInfo;
    QString m_searchText;
    int m_songIdIndex;   ///The index of the corresponding songIdList length

};

#endif // MUSICDOWNLOADMANAGERTHREAD_H
