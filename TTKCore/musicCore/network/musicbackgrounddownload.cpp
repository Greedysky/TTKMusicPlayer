#include "musicbackgrounddownload.h"
#include "musicsourcedownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicbackgroundmanager.h"
#include "musicconnectionpool.h"
#include "musictopareawidget.h"

MusicBackgroundDownload::MusicBackgroundDownload(const QString &name, const QString &save,
                                           QObject *parent)
    : QObject(parent), m_artName(name), m_savePath(save), m_index(0), m_counter(0)
{
    M_CONNECTION_PTR->setValue(getClassName(), this);
    M_CONNECTION_PTR->poolConnect(getClassName(), MusicTopAreaWidget::getClassName());
}

MusicBackgroundDownload::~MusicBackgroundDownload()
{
    M_CONNECTION_PTR->poolDisConnect(getClassName());
}

QString MusicBackgroundDownload::getClassName()
{
    return staticMetaObject.className();
}

void MusicBackgroundDownload::startToDownload()
{
    MusicSourceDownloadThread *download = new MusicSourceDownloadThread(this);
    ///Set search image API
    connect(download, SIGNAL(downLoadByteDataChanged(QByteArray)), SLOT(downLoadFinished(QByteArray)));
    download->startToDownload(BIG_BG_ART_URL.arg(m_artName));
}

void MusicBackgroundDownload::downLoadFinished(const QByteArray &data)
{
    QTextStream in(MConst_cast(QByteArray*, &data));
    QString line = in.readLine();

    while(!line.isNull())
    {
        ///On line reading, and reading effectively 5 times
        if(line.contains("<dd>") && m_counter < 5)
        {
            line = line.split("lazy_src=").back().split(" ").front();
            line.remove(0, 1);
            line.remove(line.length() - 1, 1);
            line = line.remove("sp");

            M_LOGGER_ERROR(line);
            MusicDataDownloadThread *down = new MusicDataDownloadThread(line, QString("%1%2%3%4").arg(BACKGROUND_DIR_FULL)
                                    .arg(m_savePath).arg(m_counter++).arg(SKN_FILE),
                                    MusicDownLoadThreadAbstract::Download_BigBG, this);
            connect(down, SIGNAL(downLoadDataChanged(QString)), SLOT(bgDownLoadFinished()));
            down->startToDownload();
        }
        line = in.readLine();
    }
}

void MusicBackgroundDownload::bgDownLoadFinished()
{
    if( ++m_index >= m_counter)
    {
        M_BACKGROUND_PTR->setArtName( m_artName );
        emit musicBgDownloadFinished();
        deleteLater();
    }
}
