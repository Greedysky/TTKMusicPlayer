#include "musicconnectionpool.h"

#include <QWidget>

MusicConnectionPool::MusicConnectionPool(QObject *parent)
    : QObject(parent)
{

}

void MusicConnectionPool::connect(const QString &from,
                                  const QString &to)
{
    QObject *first = m_para.value(from);
    QObject *second = m_para.value(to);
    if(first == NULL || second == NULL)
    {
        return;
    }

    if(from == "MusicAbstractMoveWidget" && to == "MusicPlayer" )
    {
        QObject::connect(first, SIGNAL(setSpectrum(HWND,int,int)),
                        second, SLOT(setSpectrum(HWND,int,int)));
    }

    if( (from == "MusicLocalSongsManagerWidget" && to == "MusicApplication") ||
        (from == "MusicMyDownloadRecordWidget" && to == "MusicApplication") )
    {
        QObject::connect(first, SIGNAL(addSongToPlay(QStringList)),
                        second, SLOT(addSongToPlayList(QStringList)));
    }

    if(from == "MusicTimerWidget" && to == "MusicApplicationObject" )
    {
        QObject::connect(first, SIGNAL(timerParameterChanged()),
                        second, SLOT(musicToolSetsParameter()));
    }

    if( (from == "MusicToolSetsWidget" && to == "MusicApplication") ||
        (from == "MusicApplicationObject" && to == "MusicApplication") )
    {
        QObject::connect(first, SIGNAL(getCurrentPlayList(QStringList&)),
                        second, SLOT(getCurrentPlayList(QStringList&)));
    }
}

//void MusicConnectionPool::multiConnect(const QString &from, const QString &to)
//{

//}

void MusicConnectionPool::disConnect(const QString &name)
{
    m_para.take(name);
}

void MusicConnectionPool::disMultiConnect(const QString &name, QObject *object)
{
    QList<QObject*> l = m_multiPara[name];
    for(int i=0; i<l.count(); ++i)
    {
        if(object == l[i])
        {
            l.takeAt(i);
            break;
        }
    }
    m_multiPara[name] = l;
}
