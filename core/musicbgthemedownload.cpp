#include "musicbgthemedownload.h"
#include "musicsongdownloadthread.h"
#include <QDebug>
#include <QHostInfo>

MusicBgThemeDownload::MusicBgThemeDownload(const QString &name,
                                           QObject *parent)
    : QObject(parent),artName(name)
{
    MusicSongDownloadThread *download = new MusicSongDownloadThread(
                 "http://image.baidu.com/i?tn=baiduimage&ipn=r&ct=201326592&cl=2&"\
                 "lm=-1&st=-1&fm=index&fr=&sf=1&fmq=&pv=&ic=0&nc=1&z=&se=1&showtab=0&"\
                 "fb=0&width=&height=&face=0&istype=2&ie=utf-8&word=" + name + "+%E5%A3%81%E7%BA%B8" +
                 "#z=0&pn=&ic=0&st=-1&face=0&s=0&lm=-1"
                  ,TMP_DOWNLOAD,this);
    ///Set search image API
    connect(download,SIGNAL(musicDownLoadFinished(QString)),
                     SLOT(downLoadFinished(QString)));
}

void MusicBgThemeDownload::downLoadFinished(const QString&)
{
    QFile file(TMP_DOWNLOAD);
    ///Check if the file exists and can be written
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString line = in.readLine();
    QString buffer;
    int counter = 0;
    while(!line.isNull() && counter <= 3)
    {   ///On line reading, and reading effectively 4 times
        if(line.contains("\"objURL\":\""))
        {
            buffer = line.split("\"objURL\":\"").back().remove("\",");
            ///Get to download the URL
            if(buffer.startsWith("http://") && !buffer.contains(',') )
            {
                ++counter;
                qDebug()<<"objURL"<<buffer;
                new MusicSongDownloadThread(buffer, QString("%1%2%3%4").arg(ART_BG).
                                                    arg(artName).arg(counter).arg(JPG_FILE),this);
                ///To start the download the corresponding background picture
            }
        }
        else if(line.contains("\"hoverURL\":\""))
        {
            buffer = line.split("\"hoverURL\":\"").back().remove("\",");
            if(buffer.startsWith("http://") && !buffer.contains(',') )
            {
                ++counter;
                qDebug()<<"hoverURL"<<buffer;
                new MusicSongDownloadThread(buffer, QString("%1%2%3%4").arg(ART_BG).
                                                    arg(artName).arg(counter).arg(JPG_FILE),this);

            }
        }
        line = in.readLine();
    }
    file.close();
    file.remove();///The file is closed and remove the temporary files
//    QFile::remove(TMP_DOWNLOAD);
}
