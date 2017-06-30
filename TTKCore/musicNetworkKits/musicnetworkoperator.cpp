#include "musicnetworkoperator.h"
#include "musicsourcedownloadthread.h"
#include "musicobject.h"

#include <QStringList>

const QString IP_CHECK_URL = "eEw0OUdIU0pDSWJnODRGZndnWjZNQmRneGxjb1hEWERLVXI0U0RMdWdIaz0=";

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : QObject(parent)
{

}

MusicNetworkOperator::~MusicNetworkOperator()
{

}

QString MusicNetworkOperator::getClassName()
{
    return staticMetaObject.className();
}

void MusicNetworkOperator::startToDownload()
{
    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    ///Set search ip operator API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(IP_CHECK_URL, false));
}

void MusicNetworkOperator::downLoadFinished(const QByteArray &data)
{
    QTextStream in(MConst_cast(QByteArray*, &data));
    QString line = in.readLine();

    QString dataLine;
    while(!line.isNull())
    {
        if(line.contains("<center>"))
        {
            QStringList l = line.split("center>");
            if(l.count() >= 2)
            {
                l = l[1].split(" ");
                dataLine = l.last();
                dataLine.chop(2); /// remove </
            }
        }
        line = in.readLine();
    }

    emit getNetworkOperatorFinished(dataLine);
    deleteLater();
}
