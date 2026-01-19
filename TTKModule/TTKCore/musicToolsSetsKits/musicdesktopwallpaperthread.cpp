#include "musicdesktopwallpaperthread.h"
#include "musicbackgroundconfigmanager.h"
#include "musicbackgroundmanager.h"
#include "musicextractmanager.h"

#include <QTimer>
#include <QPixmap>

MusicDesktopWallpaperThread::MusicDesktopWallpaperThread(QObject *parent)
    : QObject(parent),
      m_run(false),
      m_random(false),
      m_currentImageIndex(0)
{
    TTK::initRandom();

    m_timer = new QTimer(this);
    setInterval(20 * TTK_DN_S2MS);
    connect(m_timer, SIGNAL(timeout()), SLOT(timeout()));
}

MusicDesktopWallpaperThread::~MusicDesktopWallpaperThread()
{
    stop();
    delete m_timer;
}

void MusicDesktopWallpaperThread::setInterval(int msec)
{
    m_timer->setInterval(msec);
}

bool MusicDesktopWallpaperThread::isRunning() const noexcept
{
    return m_run;
}

void MusicDesktopWallpaperThread::setRandom(bool random) noexcept
{
    m_random = random;
}

void MusicDesktopWallpaperThread::setImagePath(const QStringList &list) noexcept
{
    m_path = list;
}

void MusicDesktopWallpaperThread::start()
{
    m_run = true;
    timeout();
    m_timer->start();
}

void MusicDesktopWallpaperThread::stop()
{
    m_run = false;
    m_timer->stop();
}

void MusicDesktopWallpaperThread::timeout()
{
    if(!m_run)
    {
        return;
    }

    if(!m_path.isEmpty())
    {
        if(m_random) ///random mode
        {
            m_currentImageIndex = TTK::random(m_path.count());
        }
        else if(++m_currentImageIndex >= m_path.count())
        {
            m_currentImageIndex = 0;
        }

        Q_EMIT updateBackground(QPixmap(m_path[m_currentImageIndex]));
    }
    else
    {
        MusicBackgroundImage image;
        MusicExtractManager::outputSkin(&image, G_BACKGROUND_PTR->backgroundUrl());
        Q_EMIT updateBackground(image.m_pix);
    }
}
