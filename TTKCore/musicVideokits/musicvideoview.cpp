#include "musicvideoview.h"
#include "musicvideocontrol.h"
#include "musicmessagebox.h"
#include "musiccoremplayer.h"
#include "musicbarragewidget.h"

#include <QTimer>
#include <QMouseEvent>

MusicViewWidget::MusicViewWidget(QWidget *parent)
    : QWidget(parent)
{

}

QString MusicViewWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicViewWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        emit setClick();
    }
}

void MusicViewWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        emit setFullScreen();
    }
}

void MusicViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    bool playing;
    emit mediaIsPlaying(playing);
    menu.addAction(tr("videoPlay"), parent(), SLOT(play()))
            ->setText(playing ? tr("videoPause") : tr("videoPlay"));
    menu.addAction(tr("videoStop"), parent(), SLOT(stop()));
    menu.exec(QCursor::pos());
    event->accept();
}


MusicVideoView::MusicVideoView(bool popup, QWidget *parent)
    : QGraphicsView(parent)
{
    setStyleSheet(MusicUIObject::MCustomStyle19);

    m_positionChanged = false;
    m_mediaPlayer = new MusicCoreMPlayer(this);
    m_videoWidget = new MusicViewWidget(this);
    m_barrageCore = new MusicBarrageWidget(this);

    connect(m_videoWidget, SIGNAL(setClick()), SLOT(play()));
    connect(m_videoWidget, SIGNAL(setFullScreen()), SLOT(setFullScreen()));
    connect(m_videoWidget, SIGNAL(mediaIsPlaying(bool&)), SLOT(mediaIsPlaying(bool&)));

    m_videoControl = new MusicVideoControl(popup, this);
    connect(m_videoControl, SIGNAL(mvURLChanged(QString)), parent, SLOT(mvURLChanged(QString)));
    connect(m_videoControl, SIGNAL(sliderValueChanged(int)), SLOT(setPosition(int)));
    connect(m_videoControl, SIGNAL(addBarrageChanged(QString)), SLOT(addBarrageChanged(QString)));
    connect(m_videoControl, SIGNAL(pushBarrageChanged(bool)), SLOT(pushBarrageChanged(bool)));
    connect(m_videoControl, SIGNAL(barrageSizeButtonChanged(int)), SLOT(barrageSizeButtonChanged(int)));
    connect(m_videoControl, SIGNAL(barrageColorButtonChanged(QColor)), SLOT(barrageColorButtonChanged(QColor)));
    m_videoControl->hide();

    resizeWindow(0, 0);

    connect(m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(m_mediaPlayer, SIGNAL(mediaChanged(QString)), SLOT(mediaChanged(QString)));
}

MusicVideoView::~MusicVideoView()
{
    delete m_barrageCore;
    delete m_mediaPlayer;
    delete m_videoControl;
    delete m_videoWidget;
}

QString MusicVideoView::getClassName()
{
    return staticMetaObject.className();
}

void MusicVideoView::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_videoControl->show();
}

void MusicVideoView::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_videoControl->hide();
}

void MusicVideoView::contextMenuEvent(QContextMenuEvent *event)
{
    QWidget::contextMenuEvent(event);
    QMenu menu(this);
    menu.setStyleSheet(MusicUIObject::MMenuStyle02);
    bool playing;
    mediaIsPlaying(playing);
    menu.addAction(tr("videoPlay"), this, SLOT(play()))
            ->setText(playing ? tr("videoPause") : tr("videoPlay"));
    menu.addAction(tr("videoStop"), this, SLOT(stop()));
    menu.exec(QCursor::pos());
    event->accept();
}

void MusicVideoView::setMedia(const QString &data)
{
    m_mediaPlayer->setMedia(MusicCoreMPlayer::VideoCategory, data,
                           (int)m_videoWidget->winId());
    QTimer::singleShot(5*MT_S2MS, this, SLOT(stop()));
}

void MusicVideoView::resizeWindow(int width, int height)
{
    m_videoWidget->setGeometry(10, 35, 635 + width, 355 + height);
    m_videoControl->setGeometry(0, 410 + height, 660 + width, 40);
    m_barrageCore->setSize(m_videoWidget->size());
}

void MusicVideoView::setFullScreen()
{
    if(m_videoControl->isPopup())
    {
        m_videoControl->fullButtonClicked();
    }
}

void MusicVideoView::play()
{
    m_mediaPlayer->play();
    switch(m_mediaPlayer->state())
    {
        case MusicCoreMPlayer::PlayingState:
            m_videoControl->setButtonStyle(false);
            m_barrageCore->start();
            break;
        case MusicCoreMPlayer::PausedState:
            m_videoControl->setButtonStyle(true);
            m_barrageCore->pause();
            break;
        default: break;
    }
}

void MusicVideoView::stop()
{
    if(!m_positionChanged)
    {
        m_mediaPlayer->stop();
        m_barrageCore->stop();
        MusicMessageBox message;
        message.setText(tr("Session time out, try again!"));
        message.exec();
    }
    m_positionChanged = false;
}

void MusicVideoView::positionChanged(qint64 position)
{
    if(position != 0)
    {
        m_positionChanged = true;
    }
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

void MusicVideoView::mediaIsPlaying(bool &play)
{
    play = (m_mediaPlayer->state() == MusicCoreMPlayer::PlayingState);
}

void MusicVideoView::addBarrageChanged(const QString &string)
{
    m_barrageCore->addBarrage(string);
}

void MusicVideoView::pushBarrageChanged(bool on)
{
    m_barrageCore->barrageStateChanged(on);
}

void MusicVideoView::barrageSizeButtonChanged(int size)
{
    m_barrageCore->setLabelTextSize(size);
}

void MusicVideoView::barrageColorButtonChanged(const QColor &color)
{
    m_barrageCore->setLabelBackground(color);
}
