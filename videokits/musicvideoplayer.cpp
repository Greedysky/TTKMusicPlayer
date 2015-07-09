#include "musicvideoplayer.h"
#include "musicvideocontrol.h"
#include "musicvideoplaylistwidget.h"
#include <QVideoSurfaceFormat>
#include <QGraphicsVideoItem>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QDebug>

MusicVideoPlayer::MusicVideoPlayer(QWidget *parent)
    : QGraphicsView(parent)
    , m_mediaPlayer(0, QMediaPlayer::VideoSurface)
    , m_videoItem(0) , m_videoControl(0), m_videoListWidget(0)
{
    setObjectName("VideoPlayer");
    setStyleSheet("#VideoPlayer{background:black}");
    m_videoItem = new QGraphicsVideoItem;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addItem(m_videoItem);
    setScene(scene);
    fitInView(m_videoItem, Qt::KeepAspectRatio);

    m_videoControl = new MusicVideoControl(this);
    m_videoControl->hide();

    m_mediaLists = new QMediaPlaylist(this);
    m_mediaPlayer.setVideoOutput(m_videoItem);
    m_mediaPlayer.setPlaylist(m_mediaLists);

    connect(&m_mediaPlayer, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(&m_mediaPlayer, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));

}

MusicVideoPlayer::~MusicVideoPlayer()
{
    delete m_videoListWidget;
    delete m_mediaLists;
    delete m_videoControl;
    delete m_videoItem;
}

void MusicVideoPlayer::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    m_videoControl->show();
}

void MusicVideoPlayer::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
    m_videoControl->hide();
}

void MusicVideoPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    emit movieDoubleClicked();
}

void MusicVideoPlayer::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void MusicVideoPlayer::addMedia(const QStringList& names)
{
    foreach(QString name, names)
        m_mediaLists->addMedia(QUrl::fromLocalFile(name));
    m_videoItem->setSize(QSizeF(500, 430));
}

void MusicVideoPlayer::play()
{
    if(m_mediaLists->isEmpty()) return;
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

void MusicVideoPlayer::playNext()
{
    if(m_mediaLists->isEmpty()) return;
    m_mediaLists->next();
}

void MusicVideoPlayer::playPrivious()
{
    if(m_mediaLists->isEmpty()) return;
    m_mediaLists->previous();
}

void MusicVideoPlayer::positionChanged(qint64 position)
{
    m_videoControl->setValue(position);
}

void MusicVideoPlayer::durationChanged(qint64 duration)
{
    m_videoControl->durationChanged(duration);
}

void MusicVideoPlayer::setPosition(int position)
{
    m_mediaPlayer.setPosition(position);
}

void MusicVideoPlayer::volumnChanged(int volumn)
{
    m_mediaPlayer.setVolume(volumn);
}

void MusicVideoPlayer::listDoubleClicked(const QModelIndex &index)
{
    m_mediaLists->setCurrentIndex(index.row());
    if(m_mediaPlayer.state() != QMediaPlayer::PlayingState)
    {
        m_mediaPlayer.play();
        m_videoControl->setButtonStyle(false);
    }
    m_videoListWidget->hide();
}

void MusicVideoPlayer::showPlayList()
{
    delete m_videoListWidget;
    m_videoListWidget = new MusicVideoPlayListWidget(this);

    QStringList list;
    for(int i=0; i<m_mediaLists->mediaCount(); ++i)
        list<<m_mediaLists->media(i).canonicalUrl().toString().split('/').back();
    m_videoListWidget->addItems(list);
    m_videoListWidget->setCurrentIndex(m_mediaLists->currentIndex());

    m_videoListWidget->show();
    int x = size().width() - m_videoListWidget->size().width();
    int y = size().height() - m_videoListWidget->size().height();
    m_videoListWidget->setGeometry(x/2, y/2, m_videoListWidget->size().width(),
                                             m_videoListWidget->size().height());
}
