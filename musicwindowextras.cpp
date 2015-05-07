#include "musicwindowextras.h"
#include "musicapplication.h"
#include <QtWinExtras>
#include <QStyle>

MusicWindowExtras::MusicWindowExtras(MusicApplication *parent) :
        QObject(parent), m_superClass(parent), m_playToolButton(NULL),
    m_forwardToolButton(NULL), m_backwardToolButton(NULL), m_taskbarProgress(NULL),
    m_taskbarButton(NULL), m_thumbnailToolBar(NULL)
{
    if(QtWin::isCompositionEnabled())
    {
        QtWin::enableBlurBehindWindow(m_superClass);
    }
    else
    {
        QtWin::disableBlurBehindWindow(m_superClass);
    }
#ifdef MUSIC_DEBUG
    createJumpList();
    createTaskbar();
#endif
    createThumbnailToolBar();
}

MusicWindowExtras::~MusicWindowExtras()
{
    delete m_playToolButton;
    delete m_forwardToolButton;
    delete m_backwardToolButton;
    delete m_taskbarProgress;
    delete m_taskbarButton;
    delete m_thumbnailToolBar;
}

void MusicWindowExtras::showPlayStatus(bool status)
{
    if(!status)
    {
        m_playToolButton->setIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaPause));
#ifdef MUSIC_DEBUG
        m_taskbarButton->setOverlayIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaPause));
        m_taskbarProgress->show();
        m_taskbarProgress->resume();
#endif
    }
    else
    {
        m_playToolButton->setIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaPlay));
#ifdef MUSIC_DEBUG
        m_taskbarButton->setOverlayIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaPlay));
        m_taskbarProgress->show();
        m_taskbarProgress->pause();
#endif
    }
}

void MusicWindowExtras::createJumpList()
{
    QWinJumpList().recent()->setVisible(true);
}

void MusicWindowExtras::setValue(int value)
{
    m_taskbarProgress->setValue(value);
}

void MusicWindowExtras::setRange(int start, int end)
{
    m_taskbarProgress->setRange(start, end);
}

void MusicWindowExtras::createTaskbar()
{
    m_taskbarButton = new QWinTaskbarButton(this);
    m_taskbarButton->setWindow(m_superClass->windowHandle());
    m_taskbarProgress = m_taskbarButton->progress();
}

void MusicWindowExtras::createThumbnailToolBar()
{
    m_thumbnailToolBar = new QWinThumbnailToolBar(this);
    m_thumbnailToolBar->setWindow(m_superClass->windowHandle());

    m_playToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
    m_playToolButton->setToolTip(tr("Play"));
    m_playToolButton->setIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaPlay));
    connect(m_playToolButton, SIGNAL(clicked()), m_superClass, SLOT(musicKey()));

    m_forwardToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
    m_forwardToolButton->setToolTip(tr("Privious"));
    m_forwardToolButton->setIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaSeekBackward));
    connect(m_forwardToolButton, SIGNAL(clicked()), m_superClass, SLOT(musicPlayPrivious()));

    m_backwardToolButton = new QWinThumbnailToolButton(m_thumbnailToolBar);
    m_backwardToolButton->setToolTip(tr("Next"));
    m_backwardToolButton->setIcon(m_superClass->style()->standardIcon(QStyle::SP_MediaSeekForward));
    connect(m_backwardToolButton, SIGNAL(clicked()), m_superClass, SLOT(musicPlayNext()));

    m_thumbnailToolBar->addButton(m_forwardToolButton);
    m_thumbnailToolBar->addButton(m_playToolButton);
    m_thumbnailToolBar->addButton(m_backwardToolButton);
}
