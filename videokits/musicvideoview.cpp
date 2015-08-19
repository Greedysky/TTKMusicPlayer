#include "musicvideoview.h"
#include "musicvideocontrol.h"

#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QMouseEvent>

MusicVideoView::MusicVideoView(QWidget *parent)
    : QGraphicsView(parent)
    , m_mediaPlayer(0, QMediaPlayer::VideoSurface)
    , m_videoItem(0) , m_videoControl(0)
{
    setObjectName("VideoPlayer");
    setStyleSheet(MusicUIObject::MCustomStyle18);
    m_videoItem = new QGraphicsVideoItem;
    m_mediaPlayer.setVideoOutput(m_videoItem);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(m_videoItem);
    setScene(scene);
    m_videoItem->setSize(QSizeF(500,400));
//    fitInView(m_videoItem, Qt::KeepAspectRatio);

    m_videoControl = new MusicVideoControl(this);
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

void MusicVideoView::setMedia(const QString& data)
{
    m_mediaPlayer.setMedia(QUrl(data));
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
    m_mediaPlayer.setPosition(position);
}

void MusicVideoView::volumnChanged(int volumn)
{
    m_mediaPlayer.setVolume(volumn);
}
