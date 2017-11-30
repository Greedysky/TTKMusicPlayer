#include "musicwstextdownloadthread.h"
#include "musiclrcanalysis.h"

MusicWSTextDownLoadThread::MusicWSTextDownLoadThread(const QString &url, const QString &save,
                                                     Download_Type type, QObject *parent)
    : MusicDownLoadThreadAbstract(url, save, type, parent)
{

}

QString MusicWSTextDownLoadThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSTextDownLoadThread::startToDownload()
{
    if( m_file && (!m_file->exists() || m_file->size() < 4) )
    {
        if( m_file->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text) )
        {
            QTextStream outstream(m_file);
            outstream.setCodec("utf-8");
            outstream << MUSIC_TTKLRCF << "\r\n" << m_url.toUtf8() << endl;
            m_file->close();

            M_LOGGER_INFO(QString("%1 download has finished!").arg(getClassName()));
            emit downLoadDataChanged( transferData() );
            deleteAll();
        }
    }
}

void MusicWSTextDownLoadThread::downLoadFinished()
{

}
