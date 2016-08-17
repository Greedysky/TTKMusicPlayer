#include "musicserviceapplication.h"

#include <QStringList>
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFile>

class MusicServiceApplicationPrivate : public TTKPrivate<MusicServiceApplication>
{
public:
    MusicServiceApplicationPrivate();
    ~MusicServiceApplicationPrivate();

    void listen(const QString &serverName);

    bool m_isRunning;
    QLocalServer *m_localServer;
};

MusicServiceApplicationPrivate::MusicServiceApplicationPrivate()
{
    m_isRunning = false;
    m_localServer = nullptr;
}

MusicServiceApplicationPrivate::~MusicServiceApplicationPrivate()
{
    delete m_localServer;
}

void MusicServiceApplicationPrivate::listen(const QString &serverName)
{
    if(m_localServer->listen(serverName))
    {
        if(m_localServer->serverError() == QAbstractSocket::AddressInUseError &&
           QFile::exists(m_localServer->serverName()))
        {
            QFile::remove(m_localServer->serverName());
            m_localServer->listen(serverName);
        }
    }
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///
MusicServiceApplication::MusicServiceApplication(int argc, char **argv)
  : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE;
    TTK_D(MusicServiceApplication);

    QCoreApplication::setApplicationName("MusicService");
    QString serverName = QCoreApplication::applicationName();

    QLocalSocket socket;
    socket.connectToServer(serverName);

    if(socket.waitForConnected(500))
    {
        QTextStream stream(&socket);
        QStringList args = QCoreApplication::arguments();
        if(args.count()>1)
        {
            stream << args.last();
        }else
        {
            stream << QString();
        }
        stream.flush();
        qDebug() << "Connected server, program will quit";
        socket.waitForBytesWritten();

        d->m_isRunning = true;
        return;
    }

    qDebug() << "Can't connect to server,build a server";
    d->m_localServer = new QLocalServer(this);
    connect(d->m_localServer, SIGNAL(newConnection()), SLOT(newLocalConnection()));
    d->listen(serverName);
}

bool MusicServiceApplication::isRunning()
{
    TTK_D(MusicServiceApplication);
    return d->m_isRunning;
}

void MusicServiceApplication::newLocalConnection()
{
    TTK_D(MusicServiceApplication);
    QLocalSocket *socket = d->m_localServer->nextPendingConnection();
    if(!socket)
    {
        return;
    }

    socket->waitForReadyRead(1000);
    QTextStream in(socket);
    QString string;
    in >> string;
    qDebug() << "The value is: " << string;

    delete socket;
}
