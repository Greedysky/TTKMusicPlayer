#include "musicvideoview.h"
#include "musictoastlabel.h"
#include "musiccoremplayer.h"
#include "musicbarragewidget.h"
#include "musicnumberdefine.h"
#include "musicuiobject.h"

#include <QMouseEvent>

MusicViewWidget::MusicViewWidget(QWidget *parent)
    : QWidget(parent)
{
    m_clickedTimer = new QTimer(this);
    m_clickedTimer->setSingleShot(true);
    m_leftPressed = false;

    connect(m_clickedTimer, SIGNAL(timeout()), SIGNAL(setClick()));
}

MusicViewWidget::~MusicViewWidget()
{
    delete m_clickedTimer;
}

void MusicViewWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_leftPressed = false;
    if(event->button() == Qt::LeftButton)
    {
        m_leftPressed = true;
        m_clickedTimer->start(300*MT_MS);
    }
}

void MusicViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(m_leftPressed)
    {
        if(m_clickedTimer->isActive())
        {
            m_clickedTimer->stop();
        }
    }
}

void MusicViewWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        if(m_clickedTimer->isActive())
        {
            m_clickedTimer->stop();
        }
        emit setFullScreen();
    }
}

void MusicViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);

    bool playing;
    emit mediaIsPlaying(playing);

    menu.addAction(tr("videoPlay"), parent(), SLOT(play()))->setText(playing ? tr("videoPause") : tr("videoPlay"));
    menu.addAction(tr("videoStop"), parent(), SLOT(stop()));
    menu.exec(QCursor::pos());
}



MusicVideoView::MusicVideoView(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    setObjectName("MusicVideoView");
    setStyleSheet(QString("#MusicVideoView{%1}").arg(MusicUIObject::MBackgroundStyle02));

    m_mediaPlayer = new MusicCoreMPlayer(this);
    m_videoWidget = new MusicViewWidget(this);
    m_barrageCore = new MusicBarrageWidget(this);

    connect(m_videoWidget, SIGNAL(setClick()), SLOT(play()));
    connect(m_videoWidget, SIGNAL(setFullScreen()), parent, SLOT(fullscreenButtonClicked()));
    connect(m_videoWidget, SIGNAL(mediaIsPlaying(bool&)), SLOT(mediaIsPlaying(bool&)));

    m_videoControl = new MusicVideoControlWidget(this);
    connect(m_videoControl, SIGNAL(mediaUrlChanged(QString)), parent, SLOT(mediaUrlChanged(QString)));
    connect(m_videoControl, SIGNAL(sliderValueChanged(int)), SLOT(setPosition(int)));
    connect(m_videoControl, SIGNAL(addBarrageChanged(MusicBarrageRecord)), SLOT(addBarrageChanged(MusicBarrageRecord)));
    connect(m_videoControl, SIGNAL(pushBarrageChanged(bool)), SLOT(pushBarrageChanged(bool)));

    resizeWindow(0, 0);

    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(mediaChanged(QString)), SLOT(mediaChanged(QString)));
    connect(m_mediaPlayer, SIGNAL(finished(int)), SLOT(mediaAutionPlayError(int)));
}

MusicVideoView::~MusicVideoView()
{
    delete m_barrageCore;
    delete m_mediaPlayer;
    delete m_videoControl;
    delete m_videoWidget;
}

void MusicVideoView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);

    bool playing;
    mediaIsPlaying(playing);
    menu.addAction(tr("videoPlay"), this, SLOT(play()))->setText(playing ? tr("videoPause") : tr("videoPlay"));
    menu.addAction(tr("videoStop"), this, SLOT(stop()));

    menu.exec(QCursor::pos());
}

void MusicVideoView::setMedia(const QString &data)
{
    m_mediaPlayer->setMedia(MusicCoreMPlayer::VideoCategory, data, (int)m_videoWidget->winId());
    m_videoControl->setQualityActionState();
}

void MusicVideoView::resizeWindow(int width, int height)
{
    m_videoWidget->setGeometry(10, 35, 655 + width, 372 + height);
    m_videoControl->setGeometry(0, 438 + height, 680 + width, 60);
    m_barrageCore->setSize(m_videoWidget->size());
}

void MusicVideoView::play()
{
    m_mediaPlayer->play();
    switch(m_mediaPlayer->state())
    {
        case MusicObject::PS_PlayingState:
            m_videoControl->setButtonStyle(false);
            m_barrageCore->start();
            break;
        case MusicObject::PS_PausedState:
            m_videoControl->setButtonStyle(true);
            m_barrageCore->pause();
            break;
        default: break;
    }
}

void MusicVideoView::stop()
{
    m_mediaPlayer->stop();
    m_barrageCore->stop();
}

void MusicVideoView::positionChanged(qint64 position)
{
    m_videoControl->setValue(position);
}

void MusicVideoView::durationChanged(qint64 duration)
{
    m_videoControl->durationChanged(duration);
}

void MusicVideoView::setPosition(int position)
{
    m_mediaPlayer->setPosition(position/MT_S2MS);
}

void MusicVideoView::volumeChanged(int volume)
{
    m_mediaPlayer->setVolume(volume);
}

void MusicVideoView::mediaChanged(const QString &data)
{
    m_videoControl->mediaChanged(data);
}

void MusicVideoView::mediaAutionPlayError(int code)
{
    if(DEFAULT_LEVEL_NORMAL == code)
    {
        stop();

        MusicToastLabel *toast = new MusicToastLabel(this);
        toast->defaultLabel(this, tr("Audio Play Time out!"));
    }
}

void MusicVideoView::mediaIsPlaying(bool &play)
{
    play = m_mediaPlayer->isPlaying();
}

void MusicVideoView::addBarrageChanged(const MusicBarrageRecord &record)
{
    m_barrageCore->addBarrage(record);
}

void MusicVideoView::pushBarrageChanged(bool on)
{
    m_barrageCore->barrageStateChanged(on);
}
