#include "musicserviceobject.h"
#include "musicversion.h"

#include <QApplication>

class MusicServiceObjectPrivate : public TTKPrivate<MusicServiceObject>
{
public:
    MusicServiceObjectPrivate();
    ~MusicServiceObjectPrivate();

    QProcess *m_process;

};

MusicServiceObjectPrivate::MusicServiceObjectPrivate()
{
    m_process = nullptr;
}

MusicServiceObjectPrivate::~MusicServiceObjectPrivate()
{
    delete m_process;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///
MusicServiceObject::MusicServiceObject(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(MusicServiceObject);

    d->m_process = new QProcess(this);
    connect(d->m_process, SIGNAL(finished(int)), qApp, SLOT(quit()));
}

void MusicServiceObject::run()
{
    TTK_D(MusicServiceObject);
    d->m_process->start(QString("%1/TTKRun.exe").arg(TTKMUSIC_VERSION_STR));
}
