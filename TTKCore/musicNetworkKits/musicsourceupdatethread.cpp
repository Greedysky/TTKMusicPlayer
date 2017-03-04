#include "musicsourceupdatethread.h"
#include "musicsourcedownloadthread.h"
#///QJson import
#include "qjson/parser.h"

MusicSourceUpdateThread::MusicSourceUpdateThread(QObject *parent)
    : QObject(parent)
{

}

QString MusicSourceUpdateThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicSourceUpdateThread::startToDownload()
{
    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(MusicCryptographicHash::decryptData(VERSION_URL, URL_KEY));
}

QString MusicSourceUpdateThread::getLastedVersion() const
{
    return m_rawData["version"].toString();
}

bool MusicSourceUpdateThread::isLastedVersion() const
{
    return (getLastedVersion() == TTKMUSIC_VERSION_STR);
}

QString MusicSourceUpdateThread::getLastedVersionDes() const
{
    return m_rawData["data"].toString();
}

void MusicSourceUpdateThread::downLoadFinished(const QByteArray &data)
{
    QJson::Parser parser;
    bool ok;
    QVariant parseData = parser.parse(data, &ok);
    if(!ok)
    {
        return;
    }
    m_rawData = parseData.toMap();
    emit downLoadDataChanged(m_rawData);
}
