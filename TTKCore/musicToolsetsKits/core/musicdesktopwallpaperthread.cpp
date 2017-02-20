#include "musicdesktopwallpaperthread.h"
#include "musicbackgroundmanager.h"
#include "musicregeditmanager.h"
#include "musictime.h"

#include <QFileInfo>

#if defined Q_OS_WIN
#   include <Windows.h>
#   if defined Q_CC_MSVC
#      pragma comment(lib, "user32.lib")
#   endif
#endif

MusicDesktopWallpaperThread::MusicDesktopWallpaperThread(QObject *parent)
    : QThread(parent)
{
#if defined Q_OS_WIN
    MusicRegeditManager().getDesktopWallControlPanel(m_originPath, m_originType);
    MusicTime::timeSRand();
#endif

    m_run = true;
    m_currentImageIndex = 0;
    m_returnToOrigin = false;
}

MusicDesktopWallpaperThread::~MusicDesktopWallpaperThread()
{
    stopAndQuitThread();
    if(m_returnToOrigin)
    {
        setWallpaper(m_originPath, m_originType);
    }
}

QString MusicDesktopWallpaperThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicDesktopWallpaperThread::start()
{
    m_run = true;
    QThread::start();
}

void MusicDesktopWallpaperThread::run()
{
    QStringList path = m_paramter.value("Path").toStringList();
    int time = m_paramter.value("Time").toInt();
    int type = m_paramter.value("Type").toInt();
    int func = m_paramter.value("Func").toInt();
    m_returnToOrigin = m_paramter.value("Close").toBool();
    while(m_run)
    {
        if(path.isEmpty())
        {
            stopAndQuitThread();
            return;
        }
        if(m_paramter["Mode"].toInt() == 2)
        {
            path.clear();
            QStringList names = M_BACKGROUND_PTR->getArtPhotoPaths();
            !names.isEmpty() ? path << names : path << m_originPath;
        }
        if( func == 1) ///random mode
        {
            m_currentImageIndex = qrand() % path.size();
        }
        else if(++m_currentImageIndex >= path.size())
        {
            m_currentImageIndex = 0;
        }
        setWallpaper(path[m_currentImageIndex], type);
        sleep(time);
    }
}

void MusicDesktopWallpaperThread::setParamters(const MusicObject::MStriantMap &p)
{
    m_paramter = p;
}

void MusicDesktopWallpaperThread::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicDesktopWallpaperThread::setWallpaper(const QString &path, int type) const
{
#if defined Q_OS_WIN
    MusicRegeditManager().setDesktopWallControlPanel(path, type);
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,
                         (TCHAR*)path.toStdWString().c_str(),
                         SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
#else
    Q_UNUSED(path)
    Q_UNUSED(type)
#endif
}
