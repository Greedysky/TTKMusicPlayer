#include "ttkrunobject.h"
#include "musicconfigobject.h"

#include <QProcess>
#include <QApplication>

class TTKRunObjectPrivate : public TTKPrivate<TTKRunObject>
{
public:
    TTKRunObjectPrivate();
    ~TTKRunObjectPrivate();

    QProcess *m_process;

};

TTKRunObjectPrivate::TTKRunObjectPrivate()
{
    m_process = nullptr;
}

TTKRunObjectPrivate::~TTKRunObjectPrivate()
{
    delete m_process;
}



TTKRunObject::TTKRunObject(QObject *parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    TTK_D(TTKRunObject);

    d->m_process = new QProcess(this);
    connect(d->m_process, SIGNAL(finished(int)),SLOT(finished(int)));
}

void TTKRunObject::checkValid()
{
    MusicConfigObject object;
    object.checkValid();
}

void TTKRunObject::run(int argc, char **argv)
{
    TTK_D(TTKRunObject);

    QStringList list(APP_NAME);
    if(argc == 3)
    {
        list << QString::fromLocal8Bit(argv[1]) << QString::fromLocal8Bit(argv[2]);
    }

    MusicConfigObject object;
    d->m_process->start(object.getAppPath(), list);
}

void TTKRunObject::finished(int code)
{
     if(code == 0)
     {
         TTK_LOGGER_INFO("Application exit success!");
     }
     else
     {
         TTK_LOGGER_INFO("Application run error, please run TTKService instead!");
     }

     qApp->quit();
}
