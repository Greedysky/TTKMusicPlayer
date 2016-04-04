#include "musicnetworkoperator.h"
#include "musicdatadownloadthread.h"

#define IP_DOWNLOAD "ip_tmp"
const QString IP_CHECK_URL = "http://1212.ip138.com/ic.asp";

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : QObject(parent)
{

}

MusicNetworkOperator::~MusicNetworkOperator()
{

}

void MusicNetworkOperator::startToOperator()
{
    MusicDataDownloadThread *download = new MusicDataDownloadThread(
                             IP_CHECK_URL, IP_DOWNLOAD,
                             MusicDownLoadThreadAbstract::Download_Other, this);
    ///Set search ip operator API
    connect(download, SIGNAL(musicDownLoadFinished(QString)), SLOT(downLoadFinished()));
    download->startToDownload();
}

void MusicNetworkOperator::downLoadFinished()
{
    QFile file(IP_DOWNLOAD);
    ///Check if the file exists and can be written
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    QString data;
    while(!line.isNull())
    {
        if(line.contains("<center>"))
        {
            QStringList l = line.split("center>");
            if(l.count() >= 2)
            {
                l = l[1].split(" ");
                data = l.last();
                data.chop(2); /// remove </
            }
        }
        line = in.readLine();
    }
    file.close();
    ///The file is closed and remove the temporary files
    QFile::remove(IP_DOWNLOAD);

    emit getNetworkOperatorFinished(data);
    deleteLater();
}
