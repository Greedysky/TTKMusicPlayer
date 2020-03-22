#include "musicnetworkoperator.h"
#include "musicdownloadsourcethread.h"
#include "musicobject.h"

#include <QStringList>

#define IP_CHECK_URL    "emtRdWI5YVg3eHduQjNwYUZNV2JXVjhGVmI2VnJJbEc="

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : QObject(parent)
{

}

void MusicNetworkOperator::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(IP_CHECK_URL, false));
}

void MusicNetworkOperator::downLoadFinished(const QByteArray &data)
{
    QTextStream in(TTKConst_cast(QByteArray*, &data));
    in.setCodec("gb2312");

    QString text(in.readAll());
    QRegExp regx("<iframe src=\"([^<]+)\" ");
    regx.setMinimal(true);
    const int pos = text.indexOf(regx);
    while(pos != -1)
    {
        MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
        connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadQueryFinished(QByteArray)));
        text = regx.cap(1);
        if(!text.contains("http:"))
        {
          text = "http:" + text;
        }
        download->startToDownload(text);
        break;
    }

    if(pos == -1)
    {
        Q_EMIT getNetworkOperatorFinished(QString());
        deleteLater();
    }
}

void MusicNetworkOperator::downLoadQueryFinished(const QByteArray &data)
{
    QTextStream in(TTKConst_cast(QByteArray*, &data));
    in.setCodec("utf-8");

    QString line, text(in.readAll());
    QRegExp regx("<p align=\"center\">([^<]+)</p>");
    int pos = text.indexOf(regx);
    while(pos != -1)
    {
        line = regx.cap(0).remove("<p align=\"center\">").remove("</p>").trimmed();
        line = line.right(2);
        break;
    }

    Q_EMIT getNetworkOperatorFinished(line);
    deleteLater();
}
