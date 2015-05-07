#include "musicdesktopwallpaperthread.h"
#include <time.h>
#include <QSettings>
#include <QFileInfo>
#include "musicobject.h"

#ifdef Q_OS_WIN32
    #include <Windows.h>
#endif
#include <QDebug>

MusicDesktopWallpaperThread::MusicDesktopWallpaperThread(QObject *parent) :
    QThread(parent)
{
    QSettings appSettings("HKEY_CURRENT_USER\\Control Panel\\Desktop",QSettings::NativeFormat);
    m_originPath = appSettings.value("Wallpaper").toString();
    m_originType = appSettings.value("WallpaperStyle").toInt();
    qsrand(time(NULL));
    m_run = true;
    m_currentImageIndex = 0;
    m_returnToOrigin = false;
}

MusicDesktopWallpaperThread::~MusicDesktopWallpaperThread()
{
    stopAndQuitThread();
    if(m_returnToOrigin)
        setWallpaper(m_originPath, m_originType);
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
            QString name;
            emit getCurrentFileName(name);

            name =  QFileInfo(ART_BG + name.split('-').front().trimmed()
                              + QString::number(2) + JPG_FILE).absoluteFilePath();
            QFile::exists(name) ? path << name : path << m_originPath;
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

void MusicDesktopWallpaperThread::setParamters(const QMap<QString,QVariant>& p)
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

void MusicDesktopWallpaperThread::setWallpaper(const QString& path, int type)
{
    QSettings appSettings("HKEY_CURRENT_USER\\Control Panel\\Desktop",QSettings::NativeFormat);
    appSettings.setValue ("Wallpaper", path);
    QString wallpaperStyle;
    if(type == 0 || type == 1) wallpaperStyle = "00";
    else wallpaperStyle = "10";
    appSettings.setValue ("WallpaperStyle", wallpaperStyle);
    SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,
                         (TCHAR*)path.toStdWString().c_str(),
                         SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
}
