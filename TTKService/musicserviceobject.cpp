#include "musicserviceobject.h"
#include "musicversion.h"

#include <QApplication>

MusicServiceObject::MusicServiceObject(QObject *parent)
    : QObject(parent)
{
    m_process = new QProcess(this);
    connect(m_process, SIGNAL(finished(int)), qApp, SLOT(quit()));
}

MusicServiceObject::~MusicServiceObject()
{
    delete m_process;
}

void MusicServiceObject::run()
{
    m_process->start(QString("%1/TTKRun.exe").arg(TTKMUSIC_VERSION_STR));
}
