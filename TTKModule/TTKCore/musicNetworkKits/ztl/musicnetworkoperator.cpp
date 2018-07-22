#include "musicnetworkoperator.h"
#include "musicdownloadsourcethread.h"
#include "musicobject.h"
#///QJson import
#include "qjson/parser.h"

#include <QStringList>

const QString IP_CHECK_URL = "d2luWkp3WFhkRU1FL3JxWWV0VW9FREcvODhCZjI3OTNES0hBZm5EMlhLQnBRQXpL";

MusicNetworkOperator::MusicNetworkOperator(QObject *parent)
    : QObject(parent)
{

}

void MusicNetworkOperator::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    ///Set search ip operator API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(IP_CHECK_URL, false));
}

void MusicNetworkOperator::downLoadFinished(const QByteArray &data)
{
    QByteArray bytes(data);
    bytes.replace("'", "\"");
    bytes.replace("ip", "\"ip\"");
    bytes.replace("address", "\"address\"");

    QJson::Parser parser;
    bool ok;
    QVariant vdata = parser.parse(bytes, &ok);
    QString line;
    if(ok)
    {
        QVariantMap value = vdata.toMap();
        line = value["address"].toString();
        QStringList ls = line.split(" ");
        if(ls.count() >= 2)
        {
            line = ls.last();
        }
    }

    emit getNetworkOperatorFinished(line);
    deleteLater();
}
