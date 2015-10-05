#include "musicvideoview.h"
#include "musicvideocontrol.h"
#include "musicmessagebox.h"

#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QMouseEvent>
#include <QTimer>

MusicVideoView::MusicVideoView(bool popup, QWidget *parent)
    : QGraphicsView(parent)
    , m_mediaPlayer(0, QMediaPlayer::VideoSurface)
    , m_videoItem(0) , m_videoControl(0)
{
    setObjectName("VideoPlayer");
    setStyleSheet(MusicUIObject::MCustomStyle19);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_videoItem = new QGraphicsVideoItem;
    m_mediaPlayer.setVideoOutput(m_videoItem);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(m_videoItem);
    setScene(scene);
    m_videoItem->setSize(QSizeF(500, 400));
    m_positionChanged = false;

    m_videoControl = new MusicVideoControl(popup, this);
    m_videoControl->hide();

    connect(&m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(&m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));

}

MusicVideoView::~MusicVideoView()
{
    delete m_videoControl;
    delete m_videoItem;
}

void MusicVideoView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    m_videoControl->show();
}

void MusicVideoView::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
    m_videoControl->hide();
}

void MusicVideoView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        play();
    }
}

void MusicVideoView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicVideoView::setMedia(const QString &data)
{
    m_mediaPlayer.setMedia(QUrl(data));
    QTimer::singleShot(5*1000, this, SLOT(stop()));
}

void MusicVideoView::resizeWindow(bool resize, QSize size)
{
    if(resize)
    {
        m_videoItem->setSize(QSizeF(size.width()*0.8, size.height()*0.8));
        m_videoControl->setFixedSize( size.width(), 40 );
        m_videoControl->move(0, size.height() - 40 - 50);
    }
    else
    {
        m_videoItem->setSize(QSizeF(500, 400));
        m_videoControl->setFixedSize( 520, 40 );
        m_videoControl->move(0, 375);
    }
}

void MusicVideoView::play()
{
    switch(m_mediaPlayer.state())
    {
        case QMediaPlayer::PlayingState:
            m_mediaPlayer.pause();
            m_videoControl->setButtonStyle(true);
            break;
        default:
            m_mediaPlayer.play();
            m_videoControl->setButtonStyle(false);
            break;
    }
}

void MusicVideoView::stop()
{
    if(!m_positionChanged)
    {
        m_mediaPlayer.stop();
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
    m_mediaPlayer.setPosition(position);
}

void MusicVideoView::volumnChanged(int volumn)
{
    m_mediaPlayer.setVolume(volumn);
}
