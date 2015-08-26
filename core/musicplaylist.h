#ifndef QMUSICPLAYLIST_H
#define QMUSICPLAYLIST_H

#include <QStringList>
#include "musicobject.h"
#include "musiclibexportglobal.h"

class MUSIC_CORE_EXPORT MusicPlaylist : public QObject
{
    Q_OBJECT
public:

    MusicPlaylist(QObject *parent = 0);
    ~MusicPlaylist();

    MusicObject::SongPlayType playbackMode() const;
    void setPlaybackMode(MusicObject::SongPlayType mode);

    int currentIndex() const;
    QString currentMediaString() const;
    int mediaCount() const;
    bool isEmpty() const;
    bool clear();

    void addMedia(const QString &content);
    void addMedia(const QStringList &items);
    bool insertMedia(int index, const QString &content);
    bool insertMedia(int index, const QStringList &items);
    bool removeMedia(int pos);
    bool removeMedia(int start, int end);

signals:
    void currentIndexChanged(int index);
    void playbackModeChanged(MusicObject::SongPlayType mode);
    void removeCurrentMedia();

public slots:
    void setCurrentIndex(int index);
    void setCurrentIndex();
    void appendMedia(const QString &content);
    void appendMedia(const QStringList &items);
    void updateMediaLists(const QStringList &list, int index);

protected:
    MusicObject::SongPlayType m_playbackMode;
    QStringList m_mediaList;
    int m_currentIndex;

};

#endif // QMUSICPLAYLIST_H
