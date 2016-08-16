#include "musicserviceapplication.h"

#include <QStringList>
#include <QSharedMemory>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFile>

MusicServiceApplication::MusicServiceApplication(int argc, char **argv)
  : QApplication(argc, argv)
{
    m_isRunning = false;

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

        m_isRunning = true;
        return;
    }
    qDebug() << "Can't connect to server,build a server";

    m_localServer = new QLocalServer(this);
    connect(m_localServer, SIGNAL(newConnection()), SLOT(newLocalConnection()));
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

MusicServiceApplication::~MusicServiceApplication()
{
    delete m_localServer;
}

bool MusicServiceApplication::isRunning()
{
    return m_isRunning;
}

void MusicServiceApplication::newLocalConnection()
{
    QLocalSocket *socket = m_localServer->nextPendingConnection();
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
