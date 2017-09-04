#include "musicrunobject.h"
#include "musicinitobject.h"

#include <QProcess>
#include <QApplication>

class MusicRunObjectPrivate : public TTKPrivate<MusicRunObject>
{
public:
    MusicRunObjectPrivate();
    ~MusicRunObjectPrivate();

    QProcess *m_process;

};

MusicRunObjectPrivate::MusicRunObjectPrivate()
{
    m_process = nullptr;
}

MusicRunObjectPrivate::~MusicRunObjectPrivate()
{
    delete m_process;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///
MusicRunObject::MusicRunObject(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(MusicRunObject);

    d->m_process = new QProcess(this);
    connect(d->m_process, SIGNAL(finished(int)),SLOT(finished(int)));
}

void MusicRunObject::checkValid()
{
    MusicInitObject object;
    object.checkValid();
}

void MusicRunObject::run(int argc, char **argv)
{
    TTK_D(MusicRunObject);
    QStringList list(APPNAME);
    if(argc == 3)
    {
        list << argv[1] << argv[2];
    }

    d->m_process->start(S_TTKSERVICE_FULL, list);
}

void MusicRunObject::finished(int code)
{
     if(code == 0)
     {
         qDebug() << "Application exit success!";
     }
     else
     {
         qDebug() << "Application run error, please run TTKService instead!";
     }

     qApp->quit();
}
