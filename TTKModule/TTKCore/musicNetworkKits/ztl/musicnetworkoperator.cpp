#include "musicnetworkoperator.h"
#include "musicdownloadsourcethread.h"
#include "musicobject.h"

#include <QStringList>

#define IP_CHECK_URL    "NDBiQkVybExJc0l5ZUZFVXc4TDMzVEhsYkJVWmg1cXlVQzdBZ2c9PQ=="

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
    in.setCodec("utf-8");

    QString line, text(in.readAll());
    QRegExp regx("<p align=\"center\">([^<]+)</p>");
    int pos = text.indexOf(regx);
    while(pos != -1)
    {
        line = regx.cap(0).remove("<p align=\"center\">").remove("</p>").trimmed();
        line = line.right(2);
//        pos += regx.matchedLength();
//        pos = regx.indexIn(text, pos);
        break;
    }

    Q_EMIT getNetworkOperatorFinished(line);
    deleteLater();
}
