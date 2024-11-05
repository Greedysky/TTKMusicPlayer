#include "musicvideoview.h"
#include "musictoastlabel.h"
#include "musiccoremplayer.h"
#include "musicbarragewidget.h"
#include "musicwidgetutils.h"

#include <QMouseEvent>

MusicViewWidget::MusicViewWidget(QWidget *parent)
    : QWidget(parent),
      m_leftPressed(false)
{
    m_clickedTimer = new QTimer(this);
    m_clickedTimer->setSingleShot(true);
    connect(m_clickedTimer, SIGNAL(timeout()), SIGNAL(timeToPlay()));
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
        m_clickedTimer->start(300);
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
        Q_EMIT fullScreenMode();
    }
}

void MusicViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    TTKObjectCast(MusicVideoView*, parent())->createRightMenu();
}


MusicVideoView::MusicVideoView(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    setObjectName(className());
    setStyleSheet(QString("#%1{ %2 }").arg(objectName(), TTK::UI::BackgroundStyle02));

    m_player = new MusicCoreMPlayer(this);
    m_videoWidget = new MusicViewWidget(this);
    m_videoControl = new MusicVideoControlWidget(this);
    m_barrageWidget = new MusicBarrageWidget(this);

    connect(m_videoWidget, SIGNAL(timeToPlay()), SLOT(play()));
    connect(m_videoWidget, SIGNAL(fullScreenMode()), this, SIGNAL(fullscreenButtonClicked()));

    connect(m_videoControl, SIGNAL(mediaUrlChanged(QString)), parent, SLOT(mediaUrlChanged(QString)));
    connect(m_videoControl, SIGNAL(sliderValueChanged(int)), SLOT(setPosition(int)));
    connect(m_videoControl, SIGNAL(addBarrageChanged(MusicBarrageRecord)), m_barrageWidget, SLOT(addBarrage(MusicBarrageRecord)));
    connect(m_videoControl, SIGNAL(pushBarrageChanged(bool)), m_barrageWidget, SLOT(barrageStateChanged(bool)));

    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_player, SIGNAL(mediaChanged(QString)), SLOT(mediaChanged(QString)));
    connect(m_player, SIGNAL(finished(int)), SLOT(mediaPlayFinished(int)));

    resizeGeometry(0, 0);
}

MusicVideoView::~MusicVideoView()
{
    delete m_player;
    delete m_videoWidget;
    delete m_videoControl;
    delete m_barrageWidget;
}

void MusicVideoView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    createRightMenu();
}

void MusicVideoView::setMedia(const QString &url)
{
    m_player->setMedia(MusicCoreMPlayer::Module::Video, url, (int)m_videoWidget->winId());
    m_videoControl->setQualityActionState();
}

void MusicVideoView::setBarrage(const QString &name, const QString &id)
{
    m_barrageWidget->setBarrage(name, id);
}

void MusicVideoView::resizeGeometry(int width, int height)
{
    m_videoWidget->setGeometry(20, 20, 640 + width, 372 + height);
    m_videoControl->setGeometry(0, 413 + height, 680 + width, 60);
    m_barrageWidget->setSize(m_videoWidget->size());
}

void MusicVideoView::createRightMenu()
{
    QMenu menu(this);
    menu.setStyleSheet(TTK::UI::MenuStyle02);

    menu.addAction({}, this, SLOT(play()))->setText(m_player->isPlaying() ? tr("Video Pause") : tr("Video Play"));
    menu.addAction(tr("Video Stop"), this, SLOT(stop()));
    menu.addSeparator();
    menu.addAction(tr("Search"), this, SIGNAL(searchButtonClicked()));
    menu.addAction(tr("Popup"), this, SIGNAL(popupButtonClicked()));
    menu.addAction(tr("Fullscreen"), this, SLOT(fullscreenButtonTrigger()));
    menu.addAction(tr("Download"), this, SIGNAL(downloadButtonClicked()));
    menu.addAction(tr("Share"), this, SIGNAL(shareButtonClicked()));
    TTK::Widget::adjustMenuPosition(&menu);

    menu.exec(QCursor::pos());
}

void MusicVideoView::play()
{
    m_player->play();
    switch(m_player->state())
    {
        case TTK::PlayState::Playing:
        {
            m_videoControl->setButtonStyle(false);
            m_barrageWidget->start();
            break;
        }
        case TTK::PlayState::Paused:
        {
            m_videoControl->setButtonStyle(true);
            m_barrageWidget->pause();
            break;
        }
        default: break;
    }
}

void MusicVideoView::stop()
{
    m_player->stop();
    m_barrageWidget->stop();
    update();
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
    m_player->setPosition(position / TTK_DN_S2MS);
}

void MusicVideoView::volumeChanged(int volume)
{
    m_player->setVolume(volume);
}

void MusicVideoView::mediaChanged(const QString &url)
{
    m_videoControl->mediaChanged(url);
}

void MusicVideoView::mediaPlayFinished(int code)
{
    if(TTK_LOW_LEVEL == code)
    {
        stop();
        MusicToastLabel::popup(tr("Audio play time out"));
    }
}

void MusicVideoView::fullscreenButtonTrigger()
{
    TTK_SIGNLE_SHOT(fullscreenButtonClicked, TTK_SIGNAL);
}
