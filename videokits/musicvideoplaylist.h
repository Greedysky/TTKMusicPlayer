#ifndef QMUSICPLAYLIST_H
#define QMUSICPLAYLIST_H

#include <QObject>
#include <QStringList>
//#include "musiclibexportglobal.h"

class  MusicVideoPlaylist : public QObject
{
    Q_OBJECT
public:
    MusicVideoPlaylist(QObject *parent = 0);
    ~MusicVideoPlaylist();

    int currentIndex() const;
    QString currentMediaString() const;
    int mediaCount() const;
    bool isEmpty() const;
    bool clear();

    bool addMedia(const QString &content);
    bool addMedia(const QStringList &items);
    bool insertMedia(int index, const QString &content);
    bool insertMedia(int index, const QStringList &items);
    bool removeMedia(int pos);
    bool removeMedia(int start, int end);

signals:
    void currentIndexChanged(int index);
    void removeCurrentMedia();

public slots:
    void setCurrentIndex(int index);
    void setCurrentIndex();
    void appendMedia(const QString &content);
    void appendMedia(const QStringList &items);

protected:
    QStringList m_mediaList;
    int m_currentIndex;

};

#endif // QMUSICPLAYLIST_H
