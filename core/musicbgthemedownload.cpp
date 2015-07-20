#include "musicbgthemedownload.h"
#include "musicsongdownloadthread.h"
#include <QDebug>
#include <QHostInfo>

MusicBgThemeDownload::MusicBgThemeDownload(const QString &name, const QString &save,
                                           QObject *parent)
    : QObject(parent), m_savePath(save), m_index(0)
{
    MusicSongDownloadThread *download = new MusicSongDownloadThread(
                 "http://www.kuwo.cn/mingxing/" + name + "/pic.htm"
                  ,TMP_DOWNLOAD,this);
    ///Set search image API
    connect(download,SIGNAL(musicDownLoadFinished(QString)),
                     SLOT(downLoadFinished(QString)));
    connect(this,SIGNAL(musicBgDownloadFinished()),parent,
                 SIGNAL(musicBgDownloadFinished()));
    download->startToDownload();
}

void MusicBgThemeDownload::downLoadFinished(const QString&)
{
    QFile file(TMP_DOWNLOAD);
    ///Check if the file exists and can be written
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();
    int counter = 0;

    while(!line.isNull())
    {
        ///On line reading, and reading effectively 5 times
        if(line.contains("<dd>") && counter < 5)
        {
            line = line.split("lazy_src=").back().split(" ").front();
            line.remove(0, 1);
            line.remove(line.length() - 1, 1);
            line = line.remove("sp");
            qDebug()<<line;
            MusicSongDownloadThread *down = new MusicSongDownloadThread(line, QString("%1%2%3%4")
                                  .arg(ART_BG).arg(m_savePath).arg(counter++).arg(SKN_FILE),this);
            connect(down, SIGNAL(musicDownLoadFinished(QString)),SLOT(bgDownLoadFinished(QString)));
            down->startToDownload();
        }
        line = in.readLine();
    }
    file.close();
//    file.remove();///The file is closed and remove the temporary files
    QFile::remove(TMP_DOWNLOAD);
}

void MusicBgThemeDownload::bgDownLoadFinished(const QString&)
{
    if( ++m_index >= 4)
    {
        emit musicBgDownloadFinished();
        deleteLater();
    }
}
