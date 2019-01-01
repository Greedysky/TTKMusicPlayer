#include "musicsourceupdatethread.h"
#include "musicdownloadsourcethread.h"
#///QJson import
#include "qjson/parser.h"

#define QN_VERSION       "version"
#define QN_CONFIG_URL    "MDN2ZTFCbndoYkFzK2pQN2wzNVVKYWpwVTd2bXdKWjk0MmhLSjRuUFpROHpzWUhLcENKK2doUmVoa2NqMWFSaGdiaUwyTG5mL2tmelJSakJnT2dmcWJQYjRPTVR0SWROMGZWMkRkaEJtbWhFQ1loRFFSd3VReXl6bXFZPQ=="

MusicSourceUpdateThread::MusicSourceUpdateThread(QObject *parent)
    : QObject(parent)
{

}

void MusicSourceUpdateThread::startToDownload()
{
    MusicDownloadSourceThread *download = new MusicDownloadSourceThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicUtils::Algorithm::mdII(QN_CONFIG_URL, false));
}

QString MusicSourceUpdateThread::getLastedVersion() const
{
    return m_rawData["version"].toString();
}

bool MusicSourceUpdateThread::isLastedVersion() const
{
    const QString &v = getLastedVersion();
    if(v.isEmpty())
    {
        return true;
    }
    else
    {
        return v == TTKMUSIC_VERSION_STR;
    }
}

QString MusicSourceUpdateThread::getLastedVersionDes() const
{
    return m_rawData["data"].toString();
}

void MusicSourceUpdateThread::downLoadFinished(const QByteArray &data)
{
    QJson::Parser parser;
    bool ok;
    const QVariant &parseData = parser.parse(data, &ok);
    if(!ok)
    {
        return;
    }

    m_rawData = parseData.toMap();
    emit downLoadDataChanged(m_rawData);
}
