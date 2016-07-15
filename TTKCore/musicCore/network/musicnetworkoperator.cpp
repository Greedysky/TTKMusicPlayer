#include "musicnetworkoperator.h"
#include "musicsourcedownloadthread.h"
#include "musicobject.h"

#include <QStringList>

const QString IP_CHECK_URL = "http://1212.ip138.com/ic.asp";

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

void MusicNetworkOperator::startToOperator()
{
    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    ///Set search ip operator API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(IP_CHECK_URL);
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
