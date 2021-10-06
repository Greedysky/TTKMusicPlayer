#include "musicapplicationmpris.h"
#include "musicapplicationmprismodule.h"

MusicApplicationMPRIS::MusicApplicationMPRIS(QObject *parent)
    : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    //MPRISv2.0
    m_root = new MusicApplicationMPRISRoot(this);
    m_player = new MusicApplicationMPRISPlayer(this);
    //
    connection.registerService("org.mpris.MediaPlayer2." APP_NAME);
    connection.registerObject("/org/mpris/MediaPlayer2", this);
}

MusicApplicationMPRIS::~MusicApplicationMPRIS()
{
    QDBusConnection::sessionBus().unregisterService("org.mpris.MediaPlayer2." APP_NAME);
    delete m_root;
    delete m_player;
}

void MusicApplicationMPRIS::run()
{
    ///do nothing
}
