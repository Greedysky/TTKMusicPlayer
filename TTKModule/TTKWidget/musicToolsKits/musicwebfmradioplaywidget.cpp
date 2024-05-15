#include "musicwebfmradioplaywidget.h"
#include "ui_musicwebfmradioplaywidget.h"
#include "musiccoremplayer.h"
#include "musicfmradiosongrequest.h"
#include "musicdownloaddatarequest.h"
#include "musiclrcanalysis.h"
#include "musicimageutils.h"
#include "musicfunctionuiobject.h"
#include "musicdownloadwidget.h"
#include "musicsong.h"

MusicWebFMRadioPlayWidget::MusicWebFMRadioPlayWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
      m_ui(new Ui::MusicWebFMRadioPlayWidget),
      m_isPlaying(false),
      m_networkRequest(nullptr)
{
    m_ui->setupUi(this);
    setFixedSize(size());
    setBackgroundLabel(m_ui->background);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(TTK::UI::ToolButtonStyle04);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    m_ui->previousButton->setIcon(QIcon(":/functions/btn_previous_hover"));
    m_ui->nextButton->setIcon(QIcon(":/functions/btn_next_hover"));
    m_ui->downloadButton->setStyleSheet(TTK::UI::BtnUnDownload);

    m_ui->playButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->previousButton->setStyleSheet(TTK::UI::BackgroundStyle01);
    m_ui->nextButton->setStyleSheet(TTK::UI::BackgroundStyle01);

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

    m_ui->volumeSlider->setStyleSheet(TTK::UI::SliderStyle10);
    m_ui->volumeSlider->setRange(0, 100);
    m_ui->volumeSlider->setValue(100);

    initialize();
    TTK::Widget::adjustWidgetPosition(this);

    m_analysis = new MusicLrcAnalysis(this);
    m_analysis->setLineMax(9);

    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(radioPlay()));
    connect(m_ui->previousButton, SIGNAL(clicked()), SLOT(radioPrevious()));
    connect(m_ui->nextButton, SIGNAL(clicked()), SLOT(radioNext()));
    connect(m_ui->downloadButton, SIGNAL(clicked()), SLOT(radioResourceDownload()));
    connect(m_ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(radioVolume(int)));
}

MusicWebFMRadioPlayWidget::~MusicWebFMRadioPlayWidget()
{
    delete m_analysis;
    delete m_player;
    delete m_networkRequest;
    delete m_ui;
}

void MusicWebFMRadioPlayWidget::show()
{
    m_networkRequest->startToRequest();
    MusicAbstractMoveWidget::show();
}

void MusicWebFMRadioPlayWidget::radioPlay()
{
    m_isPlaying = !m_isPlaying;
    m_ui->playButton->setIcon(QIcon(m_isPlaying ? ":/functions/btn_pause_hover" : ":/functions/btn_play_hover"));
    m_player->play();
}

void MusicWebFMRadioPlayWidget::radioPrevious()
{
    m_networkRequest->startToRequest();

    if(!m_isPlaying)
    {
        m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    }
}

void MusicWebFMRadioPlayWidget::radioNext()
{
    m_networkRequest->startToRequest();

    if(!m_isPlaying)
    {
        m_ui->playButton->setIcon(QIcon(":/functions/btn_pause_hover"));
    }
}

void MusicWebFMRadioPlayWidget::radioVolume(int num)
{
    m_player->setVolume(num);
}

void MusicWebFMRadioPlayWidget::radioResourceDownload()
{
    TTK::MusicSongInformation info;
    if(m_networkRequest)
    {
        info = m_networkRequest->item();
    }

    MusicDownloadWidget *widget = new MusicDownloadWidget(this);
    widget->initialize(info, MusicAbstractQueryRequest::QueryType::Music);
    widget->show();
}

void MusicWebFMRadioPlayWidget::querySongInfoFinished()
{
    TTK::MusicSongInformation info;
    if(m_networkRequest)
    {
        info = m_networkRequest->item();
    }

    m_isPlaying = true;
    if(info.m_songProps.isEmpty())
    {
        return;
    }

    std::sort(info.m_songProps.begin(), info.m_songProps.end()); //to find out the min bitrate
    m_player->setMedia(MusicCoreMPlayer::Module::Music, info.m_songProps.front().m_url);
    m_player->play();

    /// fix current play volume temporary
    const int v = m_ui->volumeSlider->value();
    m_ui->volumeSlider->setValue(0);
    m_ui->volumeSlider->setValue(v);

    QString name = TTK::String::lrcDirPrefix() + TTK::generateSongName(info.m_songName, info.m_artistName) + LRC_FILE;
    if(!QFile::exists(name))
    {
        MusicWYDownLoadTextRequest* d = new MusicWYDownLoadTextRequest(info.m_lrcUrl, name, this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(lrcDownloadStateChanged()));
        d->startToRequest();
    }
    else
    {
        lrcDownloadStateChanged();
    }

    name = ART_DIR_FULL + info.m_artistName + SKN_FILE;
    if(!QFile::exists(name))
    {
        MusicDownloadDataRequest *d = new MusicDownloadDataRequest(info.m_coverUrl, name, TTK::Download::Cover, this);
        connect(d, SIGNAL(downLoadDataChanged(QString)), SLOT(picDownloadStateChanged()));
        d->startToRequest();
    }
    else
    {
        picDownloadStateChanged();
    }
}

void MusicWebFMRadioPlayWidget::closeEvent(QCloseEvent *event)
{
    m_player->stop();
    m_isPlaying = false;
    QWidget::closeEvent(event);
}

void MusicWebFMRadioPlayWidget::initialize()
{
    m_player = new MusicCoreMPlayer(this);
    m_networkRequest = new MusicFMRadioSongRequest(this);

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_networkRequest, SIGNAL(downLoadDataChanged(QString)), SLOT(querySongInfoFinished()));
}

void MusicWebFMRadioPlayWidget::lrcDownloadStateChanged()
{
    TTK::MusicSongInformation info;
    if(m_networkRequest)
    {
        info = m_networkRequest->item();
    }

    const QString &name = TTK::generateSongName(info.m_songName, info.m_artistName).trimmed();
    m_ui->titleWidget->setText(name);
    m_analysis->loadFromLrcFile(TTK::String::lrcDirPrefix() + name + LRC_FILE);
}

void MusicWebFMRadioPlayWidget::picDownloadStateChanged()
{
    TTK::MusicSongInformation info;
    if(m_networkRequest)
    {
        info = m_networkRequest->item();
    }

    QPixmap pix(ART_DIR_FULL + info.m_artistName + SKN_FILE);
    if(pix.isNull())
    {
        pix.load(":/image/lb_default_art");
    }

    pix = TTK::Image::roundedPixmap(pix, QSize(150, 150), 150, 150);
    m_ui->artistLabel->setPixmap(pix);
    m_ui->artistLabel->start();
}

void MusicWebFMRadioPlayWidget::positionChanged(qint64 position)
{
    m_ui->positionLabel->setText(TTKTime::formatDuration(position * TTK_DN_S2MS));

    if(m_analysis->isEmpty())
    {
        QString lrc = QString("<p style='font-weight:600;' align='center'>%1</p>").arg(tr("No lrc data file found"));
        m_ui->lrcLabel->setText(lrc);
        return;
    }

    const int index = m_analysis->currentIndex();
    const qint64 time = m_analysis->findTime(index);

    if(time < position * TTK_DN_S2MS && time != -1)
    {
        QString lrc;
        for(int i = 0; i < m_analysis->lineMax(); ++i)
        {
            if(i == m_analysis->lineMiddle())
            {
                lrc += QString("<p style='font-weight:600;' align='center'>");
            }
            else
            {
                lrc += QString("<p align='center'>");
            }

            lrc += m_analysis->text(i);
            lrc += QString("</p>");
        }
        m_ui->lrcLabel->setText(lrc);
        m_analysis->setCurrentIndex(index + 1);
    }
}

void MusicWebFMRadioPlayWidget::durationChanged(qint64 duration)
{
    m_ui->durationLabel->setText(QString("/%1").arg(TTKTime::formatDuration(duration * TTK_DN_S2MS)));
}
