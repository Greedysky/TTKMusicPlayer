#include "musicwebmusicradiowidget.h"
#include "ui_musicwebmusicradiowidget.h"
#include "musiccoremplayer.h"
#include "musicuiobject.h"
#include "musicbackgroundmanager.h"
#include "musicradioplaylistthread.h"
#include "musicradiosongsthread.h"
#include "musictextdownloadthread.h"
#include "musicdatadownloadthread.h"
#include "musiclrcanalysis.h"
#include "musictime.h"
#include "musicutils.h"

MusicWebMusicRadioWidget::MusicWebMusicRadioWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      ui(new Ui::MusicWebMusicRadioWidget), m_radio(nullptr), m_playListThread(nullptr),
      m_songsThread(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);

    m_currentPlayListIndex = 0;
    m_isPlaying = false;
    m_analysis = new MusicLrcAnalysis(this);

    m_autoNextTimer.setInterval(3*1000);
    connect(&m_autoNextTimer, SIGNAL(timeout()), SLOT(radioNext()));
    connect(ui->volumeSlider, SIGNAL(sliderMoved(int)), &m_autoNextTimer, SLOT(stop()));
    connect(ui->volumeSlider, SIGNAL(sliderReleased()), &m_autoNextTimer, SLOT(start()));

    ui->topTitleCloseButton->setIcon(QIcon(":/share/searchclosed"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->playButton->setIcon(QIcon(":/image/stop"));
    ui->previousButton->setIcon(QIcon(":/image/previous"));
    ui->nextButton->setIcon(QIcon(":/image/next"));

    ui->playButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->previousButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);
    ui->nextButton->setStyleSheet(MusicUIObject::MPushButtonStyle08);

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
    m_autoNextTimer.stop();
    delete m_analysis;
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
    SongRadioInfo info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }
    if(info.m_songUrl.isEmpty())
    {
        return;
    }

    delete m_radio;
    m_radio = new MusicCoreMPlayer(this);
    connect(m_radio, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_radio, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    m_radio->setMedia(MusicCoreMPlayer::MusicCategory, info.m_songUrl, -1);
    m_radio->setVolume(ui->volumeSlider->value());

    QString name = LRC_DOWNLOAD_AL + info.m_artistName + " - " + info.m_songName + LRC_FILE;
    if(!QFile::exists(name))
    {
        MusicTextDownLoadThread* lrcDownload = new MusicTextDownLoadThread(info.m_lrcUrl, name,
                                 MusicDownLoadThreadAbstract::Download_Lrc, this);
        connect(lrcDownload, SIGNAL(musicDownLoadFinished(QString)), SLOT(lrcDownloadStateChanged()));
        lrcDownload->startToDownload();
    }
    else
    {
        lrcDownloadStateChanged();
    }

    name = ART_DOWNLOAD_AL + info.m_artistName + SKN_FILE;
    if(!QFile::exists(name))
    {
        MusicDataDownloadThread *picDwonload = new MusicDataDownloadThread(info.m_songPicUrl, name,
                                 MusicDownLoadThreadAbstract::Download_SmlBG, this);
        connect(picDwonload, SIGNAL(musicDownLoadFinished(QString)), SLOT(picDownloadStateChanged()));
        picDwonload->startToDownload();
    }
    else
    {
        picDownloadStateChanged();
    }
}

void MusicWebMusicRadioWidget::lrcDownloadStateChanged()
{
    SongRadioInfo info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }
    if(info.m_songUrl.isEmpty())
    {
        return;
    }

    QString name = info.m_artistName + " - " + info.m_songName;
    ui->titleWidget->setText(name);
    m_analysis->transLrcFileToTime(LRC_DOWNLOAD_AL + name + LRC_FILE);
}

void MusicWebMusicRadioWidget::picDownloadStateChanged()
{
    SongRadioInfo info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }
    if(info.m_songUrl.isEmpty())
    {
        return;
    }

    QString path = ART_DOWNLOAD_AL + info.m_artistName + SKN_FILE;
    QPixmap pix(path);
    if(pix.isNull())
    {
        pix.load(":/share/defaultArt");
    }
    pix = MusicUtils::pixmapToRound(pix, QSize(150, 150), 100, 100);
    ui->artistLabel->setPixmap(pix);
    ui->artistLabel->start();
}

void MusicWebMusicRadioWidget::positionChanged(qint64 position)
{
    if(!m_radio)
    {
        return;
    }

    m_autoNextTimer.stop();
    m_autoNextTimer.start();
    ui->positionLabel->setText(QString("%1").arg(MusicTime::msecTime2LabelJustified(position*1000)));

    if(m_analysis->isEmpty())
    {
        QString lrc = QString("<p style='font-weight:600;' align='center'>%1</p>").arg(tr("unFoundLrc"));
        ui->lrcLabel->setText(lrc);
        return;
    }
    int index = m_analysis->getCurrentIndex();
    qint64 time = m_analysis->findTime(index);
    if(time < position*1000 && time != -1)
    {
        QString lrc;
        for(int i=0; i<MIN_LRCCONTAIN_COUNT; ++i)
        {
            if(i == CURRENT_LRC_PAINT)
            {
                lrc += QString("<p style='font-weight:600;' align='center'>");
            }
            else
            {
                lrc += QString("<p align='center'>");
            }
            lrc += m_analysis->getText(i);
            lrc += QString("</p>");
        }
        ui->lrcLabel->setText(lrc);
        m_analysis->setCurrentIndex(++index);
    }
}

void MusicWebMusicRadioWidget::durationChanged(qint64 duration)
{
    if(!m_radio)
    {
        return;
    }
    ui->durationLabel->setText(QString("/%1").arg(MusicTime::msecTime2LabelJustified(duration*1000)));
}

void MusicWebMusicRadioWidget::show()
{
    QPixmap pix(M_BG_MANAGER->getMBackground());
    ui->background->setPixmap(pix.scaled( size() ));
    MusicAbstractMoveWidget::show();
}
