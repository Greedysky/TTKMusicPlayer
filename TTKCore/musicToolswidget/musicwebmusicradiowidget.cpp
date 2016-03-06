#include "musicwebmusicradiowidget.h"
#include "ui_musicwebmusicradiowidget.h"
#include "musiccoremplayer.h"
#include "musicuiobject.h"
#include "musicbgthememanager.h"
#include "musicradioplaylistthread.h"
#include "musicradiosongsthread.h"

#include <QDebug>

MusicWebMusicRadioWidget::MusicWebMusicRadioWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicWebMusicRadioWidget), m_radio(nullptr), m_playListThread(nullptr),
      m_songsThread(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    m_currentPlayListIndex = 0;
    m_isPlaying = false;

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->lrcLabel->setStyleSheet("background:#545432");
    ui->artistLabel->setStyleSheet("background:#545432");

    ui->playButton->setIcon(QIcon(":/image/stop"));
    ui->previousButton->setIcon(QIcon(":/image/previous"));
    ui->nextButton->setIcon(QIcon(":/image/next"));

    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->previousButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);
    ui->nextButton->setStyleSheet(MusicUIObject::MPushButtonStyle05);

    ui->playButton->setIconSize(QSize(31, 31));
    ui->previousButton->setIconSize(QSize(31, 31));
    ui->nextButton->setIconSize(QSize(31, 31));

    ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));

    ui->volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(100);

    connect(ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(ui->previousButton, SIGNAL(clicked()), SLOT(radioPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), SLOT(radioNext()));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
}

MusicWebMusicRadioWidget::~MusicWebMusicRadioWidget()
{
    delete m_radio;
    delete m_songsThread;
    delete m_playListThread;
}

void MusicWebMusicRadioWidget::closeEvent(QCloseEvent *event)
{
    delete m_radio;
    m_radio = nullptr;
    QWidget::closeEvent(event);
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

void MusicWebMusicRadioWidget::radioPlay()
{
    m_isPlaying = !m_isPlaying;
    if(m_isPlaying)
    {
        ui->playButton->setIcon(QIcon(":/image/stop"));
    }
    else
    {
        ui->playButton->setIcon(QIcon(":/image/play"));
        m_radio->stop();
        return;
    }

    startToPlay();
}

void MusicWebMusicRadioWidget::radioPrevious()
{
    if(m_playListIds.isEmpty())
    {
        return;
    }

    --m_currentPlayListIndex;
    if(m_currentPlayListIndex > m_playListIds.count())
    {
        m_currentPlayListIndex = m_playListIds.count() - 1;
    }
    else if(m_currentPlayListIndex < 0)
    {
        m_currentPlayListIndex = 0;
    }
    m_songsThread->startToDownload(m_playListIds[m_currentPlayListIndex]);

    if(!m_isPlaying)
    {
        ui->playButton->setIcon(QIcon(":/image/stop"));
    }
}

void MusicWebMusicRadioWidget::radioNext()
{
    if(m_playListIds.isEmpty())
    {
        return;
    }

    ++m_currentPlayListIndex;
    if(m_currentPlayListIndex > m_playListIds.count())
    {
        m_currentPlayListIndex = m_playListIds.count() - 1;
    }
    else if(m_currentPlayListIndex < 0)
    {
        m_currentPlayListIndex = 0;
    }
    m_songsThread->startToDownload(m_playListIds[m_currentPlayListIndex]);

    if(!m_isPlaying)
    {
        ui->playButton->setIcon(QIcon(":/image/stop"));
    }
}

void MusicWebMusicRadioWidget::radioVolume(int num)
{
    if(m_radio)
    {
        m_radio->setVolume(num);
    }
}

void MusicWebMusicRadioWidget::getPlayListFinished()
{
    m_playListIds = m_playListThread->getMusicPlayList();
    m_currentPlayListIndex = 0;
    if(m_songsThread && !m_playListIds.isEmpty())
    {
        m_songsThread->startToDownload(m_playListIds.first());
    }
}

void MusicWebMusicRadioWidget::getSongInfoFinished()
{
    m_isPlaying = true;
    startToPlay();
}

void MusicWebMusicRadioWidget::startToPlay()
{
    SongInfo info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }
    if(info.m_songRealLink.isEmpty())
    {
        return;
    }

    delete m_radio;
    m_radio = new MusicCoreMPlayer(this);
    m_radio->setMedia(MusicCoreMPlayer::RadioCategory, info.m_songRealLink, -1);
    m_radio->setVolume(ui->volumeSlider->value());
}

void MusicWebMusicRadioWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
