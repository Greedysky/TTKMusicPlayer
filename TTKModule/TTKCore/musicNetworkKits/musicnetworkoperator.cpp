#include "musicnetworkoperator.h"
#include "musicdownloadsourcethread.h"
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
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    ///Set search ip operator API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(IP_CHECK_URL, false));
}

void MusicNetworkOperator::downLoadFinished(const QByteArray &data)
{
    QTextStream in(MConst_cast(QByteArray*, &data));
    in.setCodec("gb2312");

    QString dataLine, text(in.readAll());
    QRegExp regx(QString("<center>([^<]+)</center>"));
    int pos = text.indexOf(regx);
    while(pos != -1)
    {
        dataLine = regx.cap(0).remove("<center>").remove("</center>").trimmed();
        dataLine = dataLine.right(2);
        pos += regx.matchedLength();
        pos = regx.indexIn(text, pos);
    }

    emit getNetworkOperatorFinished(dataLine);
    deleteLater();
}
