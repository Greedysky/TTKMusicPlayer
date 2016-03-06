#include "musicwebmusicradiowidget.h"
#include "ui_musicwebmusicradiowidget.h"
#include "musicuiobject.h"
#include "musicbgthememanager.h"
#include "musicradioplaylistthread.h"
#include "musicradiosongsthread.h"

#include <QDebug>

MusicWebMusicRadioWidget::MusicWebMusicRadioWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicWebMusicRadioWidget), m_playListThread(nullptr),
      m_songsThread(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->lrcLabel->setStyleSheet("background:#545432");
    ui->artistLabel->setStyleSheet("background:#545432");

    ui->playButton->setIcon(QIcon(":/image/play"));
    ui->previousButton->setIcon(QIcon(":/image/previous"));
    ui->nextButton->setIcon(QIcon(":/image/next"));
}

MusicWebMusicRadioWidget::~MusicWebMusicRadioWidget()
{
    delete m_songsThread;
    delete m_playListThread;
}

void MusicWebMusicRadioWidget::setNetworkCookie(QNetworkCookieJar *jar)
{
    if(m_songsThread == nullptr || m_playListThread == nullptr)
    {
        m_playListThread = new MusicRadioPlayListThread(this, jar);
        connect(m_playListThread, SIGNAL(networkReplyFinished(QString)), SLOT(getPlayListFinished()));
        m_songsThread = new MusicRadioSongsThread(this, jar);
        connect(m_songsThread, SIGNAL(networkReplyFinished(QString)), SLOT(getSongInfoFinished()));
    }
}

void MusicWebMusicRadioWidget::updateRadioList(const QString &category)
{
    if(m_playListThread)
    {
        m_playListThread->startToDownload(category);
    }
}

void MusicWebMusicRadioWidget::getPlayListFinished()
{
    m_playListIds = m_playListThread->getMusicPlayList();
    if(m_songsThread && !m_playListIds.isEmpty())
    {
        m_songsThread->startToDownload(m_playListIds.first());
    }
}

void MusicWebMusicRadioWidget::getSongInfoFinished()
{
    qDebug() << m_songsThread->getMusicSongInfo().m_songName;
    qDebug() << m_songsThread->getMusicSongInfo().m_artistName;
    qDebug() << m_songsThread->getMusicSongInfo().m_songRealLink;
}

void MusicWebMusicRadioWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
