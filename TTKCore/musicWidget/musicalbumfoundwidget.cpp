#include "musicalbumfoundwidget.h"
#include "musicdownloadqueryfactory.h"
#include "musicuiobject.h"

#include <QBoxLayout>

MusicAlbumFoundWidget::MusicAlbumFoundWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    m_mainWindow = new QWidget(this);
    m_mainWindow->setStyleSheet(MusicUIObject::MBackgroundStyle17);
    layout->addWidget(m_mainWindow);
    setLayout(layout);

    m_statusLabel = new QLabel(tr("Loading Now ... "), m_mainWindow);
    m_statusLabel->setStyleSheet(MusicUIObject::MCustomStyle04 + MusicUIObject::MCustomStyle01);
    QHBoxLayout *mLayout = new QHBoxLayout(m_mainWindow);
    mLayout->addWidget(m_statusLabel, 0, Qt::AlignCenter);
    m_mainWindow->setLayout(mLayout);

    m_downloadThread = M_DOWNLOAD_QUERY_PTR->getQueryThread(this);
    connect(m_downloadThread, SIGNAL(downLoadDataChanged(QString)), SLOT(queryAllFinished()));
}

MusicAlbumFoundWidget::~MusicAlbumFoundWidget()
{
    delete m_downloadThread;
    delete m_statusLabel;
    delete m_mainWindow;
}

QString MusicAlbumFoundWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicAlbumFoundWidget::setSongName(const QString &name)
{
    m_songNameFull = name;
    m_downloadThread->setQueryAllRecords(false);
    m_downloadThread->startSearchSong(MusicDownLoadQueryThreadAbstract::MusicQuery, name.split("-").front().trimmed());
}

void MusicAlbumFoundWidget::queryAllFinished()
{
//    MusicObject::MusicSongInfomations musicSongInfos(m_downloadThread->getMusicSongInfos());
    m_statusLabel->setPixmap(QPixmap(":/image/lb_noSimilar"));
//    foreach(const MusicObject::MusicSongInfomation &info, musicSongInfos)
//    {
//        if(m_songNameFull.contains(info.m_songName))
//        {

//        }
//    }
}
