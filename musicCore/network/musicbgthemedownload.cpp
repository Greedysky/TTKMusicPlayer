#include "musicbgthemedownload.h"
#include "musicdatadownloadthread.h"
#include "musicbgthememanager.h"
#include "musicconnectionpool.h"

MusicBgThemeDownload::MusicBgThemeDownload(const QString &name, const QString &save,
                                           QObject *parent)
    : QObject(parent), m_artName(name), m_savePath(save), m_index(0), m_counter(0)
{
    M_CONNECTION->setValue("MusicBgThemeDownload", this);
    M_CONNECTION->connect("MusicBgThemeDownload", "MusicTopAreaWidget");
}

MusicBgThemeDownload::~MusicBgThemeDownload()
{
    M_CONNECTION->disConnect("MusicBgThemeDownload");
}

void MusicBgThemeDownload::startToDownload()
{
    MusicDataDownloadThread *download = new MusicDataDownloadThread(
                             BIG_BG_ART_URL.arg(m_artName), TMP_DOWNLOAD,
                             MusicDownLoadThreadAbstract::Download_BigBG, this);
    ///Set search image API
    connect(download, SIGNAL(musicDownLoadFinished(QString)), SLOT(downLoadFinished()));
    download->startToDownload();
}

void MusicBgThemeDownload::downLoadFinished()
{
    QFile file(TMP_DOWNLOAD);
    ///Check if the file exists and can be written
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream in(&file);
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
            M_LOGGER << line << LOG_END;
            MusicDataDownloadThread *down = new MusicDataDownloadThread(line, QString("%1%2%3%4").arg(ART_BG_AL)
                                    .arg(m_savePath).arg(m_counter++).arg(SKN_FILE),
                                    MusicDownLoadThreadAbstract::Download_BigBG, this);
            connect(down, SIGNAL(musicDownLoadFinished(QString)), SLOT(bgDownLoadFinished()));
            down->startToDownload();
        }
        line = in.readLine();
    }
    file.close();
    //The file is closed and remove the temporary files
    QFile::remove(TMP_DOWNLOAD);
}

void MusicBgThemeDownload::bgDownLoadFinished()
{
    if( ++m_index >= m_counter)
    {
        M_BG_MANAGER->setArtName( m_artName );
        emit musicBgDownloadFinished();
        deleteLater();
    }
}
