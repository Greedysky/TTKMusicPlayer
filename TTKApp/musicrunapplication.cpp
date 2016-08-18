#include "musicrunapplication.h"

#include <QStringList>
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFile>

class MusicRunApplicationPrivate : public TTKPrivate<MusicRunApplication>
{
public:
    MusicRunApplicationPrivate();
    ~MusicRunApplicationPrivate();

    void listen(const QString &serverName);

    bool m_isRunning;
    QLocalServer *m_localServer;
};

MusicRunApplicationPrivate::MusicRunApplicationPrivate()
{
    m_isRunning = false;
    m_localServer = nullptr;
}

MusicRunApplicationPrivate::~MusicRunApplicationPrivate()
{
    delete m_localServer;
}

void MusicRunApplicationPrivate::listen(const QString &serverName)
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
MusicRunApplication::MusicRunApplication(int argc, char **argv)
  : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE;
    TTK_D(MusicRunApplication);

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

bool MusicRunApplication::isRunning()
{
    TTK_D(MusicRunApplication);
    return d->m_isRunning;
}

void MusicRunApplication::newLocalConnection()
{
    TTK_D(MusicRunApplication);
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
