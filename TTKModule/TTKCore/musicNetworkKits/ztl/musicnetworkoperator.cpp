#include "musicnetworkoperator.h"
#include "musicdownloadsourcethread.h"
#include "musicobject.h"

#include <QStringList>

#define IP_CHECK_URL    "QmZadEd0V0ovaUpsMlc4MG1uSWVUUUpnenhKRFdSSTJKV0hDdlRIcE1tND0="

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
    QTextStream in(MConst_cast(QByteArray*, &data));
    in.setCodec("gb2312");

    QString line, text(in.readAll());
    QRegExp regx("<center>([^<]+)</center>");
    int pos = text.indexOf(regx);
    while(pos != -1)
    {
        line = regx.cap(0).remove("<center>").remove("</center>").trimmed();
        line = line.right(2);
        pos += regx.matchedLength();
        pos = regx.indexIn(text, pos);
    }

    emit getNetworkOperatorFinished(line);
    deleteLater();
}
