#ifndef MUSICDOWNLOADQUERYMOVIETHREAD_H
#define MUSICDOWNLOADQUERYMOVIETHREAD_H

#include <QObject>

class MusicDownLoadQueryMovieThread : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryMovieThread(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MUSICDOWNLOADQUERYMOVIETHREAD_H