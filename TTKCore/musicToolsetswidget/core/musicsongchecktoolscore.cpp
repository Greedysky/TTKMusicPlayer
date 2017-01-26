#include "musicsongchecktoolscore.h"
#include <QDebug>

MusicSongCheckToolsRenameCore::MusicSongCheckToolsRenameCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsRenameCore::~MusicSongCheckToolsRenameCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsRenameCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsRenameCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsRenameCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsRenameCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsRenameCore::run()
{
    if(m_songItems)
    {
        qDebug() << "MusicSongCheckToolsRenameCore" << m_songItems->count();
    }
}



MusicSongCheckToolsDuplicateCore::MusicSongCheckToolsDuplicateCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsDuplicateCore::~MusicSongCheckToolsDuplicateCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsDuplicateCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsDuplicateCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsDuplicateCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsDuplicateCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsDuplicateCore::run()
{
    if(m_songItems)
    {
        qDebug() << "MusicSongCheckToolsRenameCore" << m_songItems->count();
    }
}



MusicSongCheckToolsQualityCore::MusicSongCheckToolsQualityCore(QObject *parent)
    : QThread(parent)
{
    m_run = false;
    m_songItems = nullptr;
}

MusicSongCheckToolsQualityCore::~MusicSongCheckToolsQualityCore()
{
    stopAndQuitThread();
}

QString MusicSongCheckToolsQualityCore::getClassName()
{
    return staticMetaObject.className();
}

void MusicSongCheckToolsQualityCore::setRenameSongs(MusicSongItems *songs)
{
    m_songItems = songs;
}

void MusicSongCheckToolsQualityCore::stopAndQuitThread()
{
    if(isRunning())
    {
        m_run = false;
        wait();
    }
    quit();
}

void MusicSongCheckToolsQualityCore::start()
{
    m_run = true;
    QThread::start();
}

void MusicSongCheckToolsQualityCore::run()
{
    if(m_songItems)
    {
        qDebug() << "MusicSongCheckToolsRenameCore" << m_songItems->count();
    }
}
