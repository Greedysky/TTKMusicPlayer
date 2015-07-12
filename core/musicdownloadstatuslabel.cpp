#include "musicdownloadstatuslabel.h"
#include "../musicapplication.h"
#include "musiclrcdownloadthread.h"
#include "musicsongdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musicbgthemedownload.h"
#include "musicobject.h"
#include <QTimer>
#include <QMovie>
#include <QLabel>

MusicDownloadStatusLabel::MusicDownloadStatusLabel(MusicApplication *w) :
    m_parentWidget(w), QObject(w), m_movie(NULL)
{
    m_downloadLrcThreadTimer = NULL;
    m_downloadLrcThread = NULL;
}

MusicDownloadStatusLabel::~MusicDownloadStatusLabel()
{
    delete m_downloadLrcThreadTimer;
    delete m_downloadLrcThread;
    delete m_movie;
}

void MusicDownloadStatusLabel::showDownLoadInfoFor(DownLoadType type)
{
    QString stringType;
    switch(type)
    {
       case DisConnection:
            stringType = ":/download/disconnection";
            break;
       case DownLoading:
            stringType = ":/download/downloading";
            break;
       case Buffing:
            stringType = ":/download/buffing";
            break;
       case Waiting:
            stringType = QString();
            break;
       default:
            stringType = QString();
            break;
    }
    ///Start up the information for show download - message;
    delete m_movie;
    m_movie = new QMovie(stringType, QByteArray(), this);
    m_parentWidget->getshowDownloadLabel()->setMovie(m_movie);
    m_movie->start();
    ///start movie
}

void MusicDownloadStatusLabel::showDownLoadInfoFinished(const QString& type)
{
    ///If the lyrics download finished immediately loaded to display
    if(type == "Lrc")
       m_parentWidget->musicLoadCurrentSongLrc();
    m_parentWidget->getshowDownloadLabel()->clear();
}

void MusicDownloadStatusLabel::musicCheckHasLrcAlready()
{
    if( m_parentWidget->checkSettingParameterValue() )
    {///Check there is no opening lyrics display mode
       if( m_parentWidget->checkMusicListCurrentIndex() )
          return;

       QString filename = m_parentWidget->getCurrentFileName();
       ///Check if the file exists
       QFile file(LRC_DOWNLOAD + filename + LRC_FILE);
       if(file.exists()) return;

       if(m_downloadLrcThread)
       {
           delete m_downloadLrcThread;
           m_downloadLrcThread = NULL;
       }
       ///Start the request query
       m_downloadLrcThread = new MusicDownLoadManagerThread(this);
       m_downloadLrcThread->startSearchSong(filename);

       showDownLoadInfoFor(Buffing);
       connect(m_downloadLrcThread,SIGNAL(showDownLoadInfoFor(DownLoadType)),
               SLOT(showDownLoadInfoFor(DownLoadType)));

       if(m_downloadLrcThreadTimer)
       {
           delete m_downloadLrcThreadTimer;
           m_downloadLrcThreadTimer = NULL;
       }
       ///This delay of two seconds so that request can be obtained correctly
       m_downloadLrcThreadTimer = new QTimer(this);
       connect(m_downloadLrcThreadTimer,SIGNAL(timeout()),this,SLOT(musicHaveNoLrcAlready()));
       m_downloadLrcThreadTimer->start(2000);
    }
}

void MusicDownloadStatusLabel::musicHaveNoLrcAlready()
{
    QList< QStringList > musicSongInfo(m_downloadLrcThread->getMusicSongInfo());
    static int counter = 5;
    if(!musicSongInfo.isEmpty())
    {
        QString filename = m_parentWidget->getCurrentFileName();
        ///download art picture
        (new MusicDataDownloadThread(musicSongInfo[0][2],
             ART_DOWNLOAD + musicSongInfo[0][3] + JPG_FILE,this))->startToDownload();

        ///download big picture
        new MusicBgThemeDownload(musicSongInfo[0][3],this);

        ///download lrc
        MusicLrcDownLoadThread* lrc = new MusicLrcDownLoadThread(musicSongInfo[0][1],
                                     LRC_DOWNLOAD + filename + LRC_FILE,this);
        connect(lrc,SIGNAL(musicDownLoadFinished(QString)),
                this,SLOT(showDownLoadInfoFinished(QString)));
        lrc->startToDownload();

        m_downloadLrcThreadTimer->stop();
        counter = 5;
        return;
    }

    if(--counter == 0)
    {
        counter = 5;
        m_downloadLrcThreadTimer->stop();
        showDownLoadInfoFinished("find error!");
    }
}
