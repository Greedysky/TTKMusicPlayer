#ifndef MUSICLOCALSONGSMANAGERTHREAD_H
#define MUSICLOCALSONGSMANAGERTHREAD_H

#include <QThread>
#include <QStringList>
#include "musiclibexportglobal.h"

class MUSIC_EXPORT MusicLocalSongsManagerThread : public QThread
{
    Q_OBJECT
public:
    explicit MusicLocalSongsManagerThread(QObject *parent = 0);
    ~MusicLocalSongsManagerThread();

    void setFindFilePath(const QString&);
    void setFindFilePath(const QStringList&);
    void stopAndQuitThread();

signals:
    void setSongNamePath(const QStringList&,const QStringList&);

public slots:
    void start();
    void run();

protected:
    void findFile(const QString&);

    bool m_run;
    QStringList m_path;
    QStringList m_filename;
    QStringList m_fileDir;

};

#endif // MUSICLOCALSONGSMANAGERTHREAD_H
