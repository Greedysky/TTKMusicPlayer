#include "musicapplicationobject.h"
#include "musicapplication.h"
#include "musicaudiorecorderwidget.h"
#include "musictimerwidget.h"
#include "musictimerautoobject.h"

#include <QPropertyAnimation>
#include <QMessageBox>

MusicApplicationObject::MusicApplicationObject(QObject *parent)
    : QObject(parent)
{
    m_supperClass = static_cast<MusicApplication*>(parent);

    m_animation = new QPropertyAnimation(parent, "windowOpacity");
    m_animation->setDuration(1000);
    m_animation->setStartValue(0);
    m_animation->setEndValue(1);
    m_animation->start();

    m_musicTimerAutoObj = new MusicTimerAutoObject(this);
    connect(m_musicTimerAutoObj, SIGNAL(setPlaySong(int)), parent,
                                 SLOT(setPlaySongChanged(int)));
    connect(m_musicTimerAutoObj, SIGNAL(setStopSong()), parent,
                                 SLOT(setStopSongChanged()));

    m_setWindowToTop = false;
    connect(this, SIGNAL(getCurrentPlayList(QStringList&)), parent,
                  SLOT(getCurrentPlayList(QStringList&)));
    musicToolSetsParameter();
}

MusicApplicationObject::~MusicApplicationObject()
{
    delete m_musicTimerAutoObj;
    delete m_animation;
}

void MusicApplicationObject::musicAboutUs()
{
    QMessageBox::about(NULL, tr("About QMusicPlayer"),
       tr("QMusicPlayer") + QString("\n\n") +
       tr("Directed By Greedysky") +
       QString("\nCopyright© 2014-2015") +
       QString("\nMail:Greedysky@163.com"));
}

void MusicApplicationObject::musicAudioRecorder()
{
    MusicAudioRecorderWidget recorder;
    recorder.exec();
}

void MusicApplicationObject::musicTimerWidget()
{
    MusicTimerWidget timer;
    QStringList list;
    emit getCurrentPlayList(list);
    timer.setSongStringList(list);
    connect(&timer, SIGNAL(timerParameterChanged()),
                    SLOT(musicToolSetsParameter()));
    timer.exec();
}

void MusicApplicationObject::musicSetWindowToTop()
{
    m_setWindowToTop = !m_setWindowToTop;
    Qt::WindowFlags flags = m_supperClass->windowFlags();
    m_supperClass->setWindowFlags( m_setWindowToTop ?
                  (flags | Qt::WindowStaysOnTopHint) :
                  (flags & ~Qt::WindowStaysOnTopHint) );
    m_supperClass->show();
}

void MusicApplicationObject::musicToolSetsParameter()
{
    m_musicTimerAutoObj->runTimerAutoConfig();
}
