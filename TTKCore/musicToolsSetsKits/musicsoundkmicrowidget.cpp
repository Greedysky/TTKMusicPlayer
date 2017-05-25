#include "musicsoundkmicrowidget.h"
#include "ui_musicsoundkmicrowidget.h"
#include "musicsoundkmicrosearchwidget.h"
#include "musicfunctionuiobject.h"
#include "musicvideouiobject.h"
#include "musicuiobject.h"
#include "musictime.h"
#include "musiccoremplayer.h"

MusicSoundKMicroWidget::MusicSoundKMicroWidget(QWidget *parent)
    : MusicAbstractMoveWidget(parent),
    m_ui(new Ui::MusicSoundKMicroWidget)
{
    m_ui->setupUi(this);
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
    setAttribute(Qt::WA_TranslucentBackground, false);

    m_ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    m_ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    m_ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(m_ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    m_ui->stackedWidget->setStyleSheet(MusicUIObject::MBackgroundStyle02);
    m_ui->topWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06);
    m_ui->controlWidget->setStyleSheet(MusicUIObject::MBackgroundStyle06);
    m_ui->timeLabel->setStyleSheet(MusicUIObject::MColorStyle03);
    m_ui->timeSlider->setStyleSheet(MusicUIObject::MSliderStyle01);

    setButtonStyle(true);
    setStateButtonStyle(true);
    m_queryMv = true;
    m_stateButtonOn = true;

    m_ui->volumeButton->setValue(100);
    m_ui->volumeButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_mediaPlayer = new MusicCoreMPlayer(this);
    m_searchWidget = new MusicSoundKMicroSearchWidget;
    m_searchWidget->connectTo(this);
    m_searchWidget->show();

    connect(m_ui->stateButton, SIGNAL(clicked()), SLOT(stateButtonChanged()));
    connect(m_ui->playButton, SIGNAL(clicked()), SLOT(playButtonChanged()));
    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_ui->timeSlider, SIGNAL(sliderReleasedAt(int)), SLOT(setPosition(int)));
    connect(m_ui->volumeButton, SIGNAL(musicVolumeChanged(int)), SLOT(volumeChanged(int)));
}

MusicSoundKMicroWidget::~MusicSoundKMicroWidget()
{
    delete m_mediaPlayer;
    delete m_searchWidget;
    delete m_ui;
}

QString MusicSoundKMicroWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundKMicroWidget::setButtonStyle(bool style) const
{
    m_ui->playButton->setStyleSheet(style ? MusicUIObject::MKGVideoBtnPlay :
                                            MusicUIObject::MKGVideoBtnPause);
}

void MusicSoundKMicroWidget::setStateButtonStyle(bool style)  const
{
    m_ui->stateButton->setStyleSheet(style ? MusicUIObject::MKGVideoBtnOrigin :
                                             MusicUIObject::MKGVideoBtnOriginOff);
}

void MusicSoundKMicroWidget::volumeChanged(int volume)
{
    m_mediaPlayer->setVolume(volume);
}

void MusicSoundKMicroWidget::positionChanged(qint64 position)
{
    m_ui->timeSlider->setValue(position*MT_S2MS);
    m_ui->timeLabel->setText(QString("%1/%2").arg(MusicTime::msecTime2LabelJustified(position*MT_S2MS))
                                             .arg(MusicTime::msecTime2LabelJustified(m_ui->timeSlider->maximum())));
}

void MusicSoundKMicroWidget::durationChanged(qint64 duration)
{
    m_ui->timeSlider->setRange(0, duration*MT_S2MS);
    m_ui->timeLabel->setText(QString("00:00/%1").arg(MusicTime::msecTime2LabelJustified(duration*MT_S2MS)));

    multiMediaChanged();
}

void MusicSoundKMicroWidget::setPosition(int position)
{
    m_mediaPlayer->setPosition(position/MT_S2MS);
}

void MusicSoundKMicroWidget::playButtonChanged()
{
    m_mediaPlayer->play();
    switch(m_mediaPlayer->state())
    {
        case MusicCoreMPlayer::PlayingState:
            setButtonStyle(false);
            break;
        case MusicCoreMPlayer::PausedState:
            setButtonStyle(true);
            break;
        default: break;
    }
}

void MusicSoundKMicroWidget::stateButtonChanged()
{
    m_stateButtonOn = !m_stateButtonOn;
    setStateButtonStyle(m_stateButtonOn);

    multiMediaChanged();
}

void MusicSoundKMicroWidget::mvURLChanged(bool mv, const QString &url)
{
    setButtonStyle(false);

    if(m_queryMv = mv)
    {
        m_ui->stackedWidget->setCurrentIndex(0);
        m_mediaPlayer->setMedia(MusicCoreMPlayer::VideoCategory, url, (int)m_ui->videoPage->winId());
        m_mediaPlayer->play();
    }
    else
    {
        m_ui->stackedWidget->setCurrentIndex(1);
        m_mediaPlayer->setMedia(MusicCoreMPlayer::MusicCategory, url);
        m_mediaPlayer->play();
    }
}

void MusicSoundKMicroWidget::closeEvent(QCloseEvent *event)
{
    MusicAbstractMoveWidget::closeEvent(event);
    delete m_mediaPlayer;
    m_mediaPlayer = nullptr;
    delete m_searchWidget;
    m_searchWidget = nullptr;
}

void MusicSoundKMicroWidget::paintEvent(QPaintEvent *event)
{
    MusicAbstractMoveWidget::paintEvent(event);
    m_searchWidget->move( geometry().topRight() + QPoint(5, 0) );
}

void MusicSoundKMicroWidget::mouseMoveEvent(QMouseEvent *event)
{
    MusicAbstractMoveWidget::mouseMoveEvent(event);
    m_searchWidget->move( geometry().topRight() + QPoint(5, 0) );
}

void MusicSoundKMicroWidget::multiMediaChanged()
{
    if(m_queryMv)
    {
        m_mediaPlayer->setMultiVoice(m_stateButtonOn ? 0 : 1);
    }
    else
    {
        m_stateButtonOn ? m_mediaPlayer->setRightVolume() : m_mediaPlayer->setLeftVolume();
    }
    volumeChanged(m_ui->volumeButton->value());
}
