#include "musicwebfmradioplaywidget.h"
#include "ui_musicwebfmradioplaywidget.h"
#include "musiccoremplayer.h"
#include "musicuiobject.h"
#include "musicfmradiosongsrequest.h"
#include "musicfmradiodownloadtextrequest.h"
#include "musicdownloaddatarequest.h"
#include "musiclrcanalysis.h"
#include "musiccoreutils.h"
#include "musicimageutils.h"
#include "musicfunctionuiobject.h"
#include "musicdownloadwidget.h"

MusicWebFMRadioPlayWidget::MusicWebFMRadioPlayWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicWebFMRadioPlayWidget),
      m_songsThread(nullptr)
{
    m_ui->setupUi(this);
    setFixedSize(size());

    m_isPlaying = false;
    m_analysis = new MusicLrcAnalysis(this);
    m_analysis->setLineMax(9);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MQSSToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));
    m_ui->downloadButton->setStyleSheet(MusicUIObject::MQSSBtnUnDownload);
    m_ui->shareButton->setStyleSheet(MusicUIObject::MQSSBtnMore);

    m_ui->playButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->previousButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);
    m_ui->nextButton->setStyleSheet(MusicUIObject::MQSSBackgroundStyle01);

#ifdef Q_OS_UNIX
    m_ui->playButton->setFocusPolicy(Qt::NoFocus);
    m_ui->previousButton->setFocusPolicy(Qt::NoFocus);
    m_ui->nextButton->setFocusPolicy(Qt::NoFocus);
#endif

    m_ui->playButton->setIconSize(QSize(31, 31));
    m_ui->previousButton->setIconSize(QSize(31, 31));
    m_ui->nextButton->setIconSize(QSize(31, 31));

    m_ui->playButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->previousButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->nextButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->downloadButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->shareButton->setCursor(QCursor(Qt::PointingHandCursor));

    m_ui->volumeSlider->setStyleSheet(MusicUIObject::MQSSSliderStyle10);
    m_ui->volumeSlider->setRange(0, 100);
    m_ui->volumeSlider->setValue(100);

    createCoreModule();

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(radioPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(radioNext()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(radioResourceDownload()));
    connect(m_ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
}

MusicWebFMRadioPlayWidget::~MusicWebFMRadioPlayWidget()
{
    delete m_analysis;
    delete m_mediaPlayer;
    delete m_songsThread;
    delete m_ui;
}

void MusicWebFMRadioPlayWidget::closeEvent(QCloseEvent *event)
{
    delete m_mediaPlayer;
    m_mediaPlayer = nullptr;
    QWidget::closeEvent(event);
}

void MusicWebFMRadioPlayWidget::updateRadioSong(const QString &id)
{
    m_currentID = id;
    m_songsThread->startToDownload(m_currentID);
}

void MusicWebFMRadioPlayWidget::mediaAutionPlayError(int code)
{
    if(DEFAULT_LEVEL_NORMAL == code)
    {
        radioNext();
    }
}

void MusicWebFMRadioPlayWidget::radioPlay()
{
    if(!m_mediaPlayer)
    {
        return;
    }

    m_isPlaying = !m_isPlaying;
    m_ui->playButton->setIcon(QIcon(m_isPlaying ? ":/functions/btn_pause_hover" : ":/functions/btn_play_hover"));
    m_mediaPlayer->play();
}

void MusicWebFMRadioPlayWidget::radioPrevious()
{
    if(m_currentID.isEmpty())
    {
        return;
    }

    m_songsThread->startToDownload(m_currentID);

    if(!m_isPlaying)
    {
        m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    }
}

void MusicWebFMRadioPlayWidget::radioNext()
{
    if(m_currentID.isEmpty())
    {
        return;
    }

    m_songsThread->startToDownload(m_currentID);

    if(!m_isPlaying)
    {
        m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    }
}

void MusicWebFMRadioPlayWidget::radioVolume(int num)
{
    if(m_mediaPlayer)
    {
        m_mediaPlayer->setVolume(num);
    }
}

void MusicWebFMRadioPlayWidget::radioResourceDownload()
{
    MusicObject::MusicSongInformation info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }

    if(info.m_songAttrs.isEmpty())
    {
        return;
    }

    MusicDownloadWidget *download = new MusicDownloadWidget(this);
    download->setSongName(info, MusicAbstractQueryRequest::MusicQuery);
    download->show();
}

void MusicWebFMRadioPlayWidget::getSongInfoFinished()
{
    m_isPlaying = true;
    startToPlay();
}

void MusicWebFMRadioPlayWidget::createCoreModule()
{
    m_mediaPlayer = new MusicCoreMPlayer(this);
    m_songsThread = new MusicFMRadioSongsRequest(this);

    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(finished(int)), SLOT(mediaAutionPlayError(int)));
    connect(m_songsThread, SIGNAL(downLoadDataChanged(QString)), SLOT(getSongInfoFinished()));
}

void MusicWebFMRadioPlayWidget::startToPlay()
{
    MusicObject::MusicSongInformation info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }

    if(info.m_songAttrs.isEmpty())
    {
        return;
    }

    if(!m_mediaPlayer)
    {
        createCoreModule();
    }

    TTK_LOGGER_INFO(info.m_songAttrs.first().m_url);
    m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, info.m_songAttrs.first().m_url);
    m_mediaPlayer->play();

    /// fix current play volume temporary
    const int v = m_ui->volumeSlider->value();
    m_ui->volumeSlider->setValue(0);
    m_ui->volumeSlider->setValue(v);

    QString name = MusicUtils::String::lrcPrefix() + info.m_singerName + " - " + info.m_songName + LRC_FILE;
    if(!QFile::exists(name))
    {
        MusicFMRadioDownLoadTextRequest* lrcDownload = new MusicFMRadioDownLoadTextRequest(info.m_lrcUrl, name, MusicObject::DownloadLrc, this);
        connect(lrcDownload, SIGNAL(downLoadDataChanged(QString)), SLOT(lrcDownloadStateChanged()));
        lrcDownload->startToDownload();
    }
    else
    {
        lrcDownloadStateChanged();
    }

    name = ART_DIR_FULL + info.m_singerName + SKN_FILE;
    if(!QFile::exists(name))
    {
        MusicDownloadDataRequest *download = new MusicDownloadDataRequest(info.m_smallPicUrl, name, MusicObject::DownloadSmallBackground, this);
        connect(download, SIGNAL(downLoadDataChanged(QString)), SLOT(picDownloadStateChanged()));
        download->startToDownload();
    }
    else
    {
        picDownloadStateChanged();
    }
}

void MusicWebFMRadioPlayWidget::lrcDownloadStateChanged()
{
    MusicObject::MusicSongInformation info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }

    if(info.m_songAttrs.isEmpty())
    {
        return;
    }

    const QString &name = (info.m_singerName + " - " + info.m_songName).trimmed();
    m_ui->titleWidget->setText(name);
    m_analysis->transLrcFileToTime(MusicUtils::String::lrcPrefix() + name + LRC_FILE);
}

void MusicWebFMRadioPlayWidget::picDownloadStateChanged()
{
    MusicObject::MusicSongInformation info;
    if(m_songsThread)
    {
        info = m_songsThread->getMusicSongInfo();
    }

    if(info.m_songAttrs.isEmpty())
    {
        return;
    }

    QPixmap pix(ART_DIR_FULL + info.m_singerName + SKN_FILE);
    if(pix.isNull())
    {
        pix.load(":/image/lb_defaultArt");
    }
    pix = MusicUtils::Image::pixmapToRound(pix, QSize(150, 150), 100, 100);
    m_ui->artistLabel->setPixmap(pix);
    m_ui->artistLabel->start();
}

void MusicWebFMRadioPlayWidget::positionChanged(qint64 position)
{
    if(!m_mediaPlayer)
    {
        return;
    }

    m_ui->positionLabel->setText(QString("%1").arg(MusicTime::msecTime2LabelJustified(position * MT_S2MS)));

    if(m_analysis->isEmpty())
    {
        QString lrc = QString("<p style='font-weight:600;' align='center'>%1</p>").arg(tr("unFoundLrc"));
        m_ui->lrcLabel->setText(lrc);
        return;
    }

    const int index = m_analysis->getCurrentIndex();
    const qint64 time = m_analysis->findTime(index);

    if(time < position * MT_S2MS && time != -1)
    {
        QString lrc;
        for(int i=0; i<m_analysis->getLineMax(); ++i)
        {
            if(i == m_analysis->getMiddle())
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
        m_ui->lrcLabel->setText(lrc);
        m_analysis->setCurrentIndex(index + 1);
    }
}

void MusicWebFMRadioPlayWidget::durationChanged(qint64 duration)
{
    if(!m_mediaPlayer)
    {
        return;
    }
    m_ui->durationLabel->setText(QString("/%1").arg(MusicTime::msecTime2LabelJustified(duration * MT_S2MS)));
}

void MusicWebFMRadioPlayWidget::show()
{
    setBackgroundPixmap(m_ui->background, size());
    MusicAbstractMoveWidget::show();
}
