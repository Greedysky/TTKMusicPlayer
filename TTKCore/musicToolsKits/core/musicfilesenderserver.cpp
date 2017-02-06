#include "musicfilesenderserver.h"

MusicFileSenderServer::MusicFileSenderServer(QObject *parent)
    : QObject(parent)
{
    m_nextFile = false;

    m_sendSocket = new QUdpSocket(this);
    m_sendSocket->bind(QHostAddress::LocalHost, SEND_PORT);
    connect(m_sendSocket, SIGNAL(readyRead()), SLOT(readPendingDatagrams()));

    m_file = new QFile(this);
}

MusicFileSenderServer::~MusicFileSenderServer()
{
    delete m_sendSocket;
    delete m_file;
}

QString MusicFileSenderServer::getClassName()
{
    return staticMetaObject.className();
}

void MusicFileSenderServer::setSendFile(const QString &file)
{
    m_files << file;
}

void MusicFileSenderServer::setSendFiles(const QStringList &files)
{
    m_files << files;
}

void MusicFileSenderServer::setReceiveAddress(const QString &ip)
{
    m_receiveIp = ip;
}

QString MusicFileSenderServer::getReceiveAddress() const
{
    return m_receiveIp;
}

void MusicFileSenderServer::start()
{
    int count = m_files.count();
    if(m_files.isEmpty())
    {
        return;
    }

    QString str = m_files.takeLast();
    m_file->setFileName( str );
    while(!m_file->open(QIODevice::ReadOnly))
    {
        if(m_files.isEmpty())
        {
            return;
        }
        count = m_files.count();
        str = m_files.takeLast();
        m_file->setFileName( str );
    }

    QString filestr = QString::number(count) + SPLITE_STR + m_file->fileName();
    m_sendSocket->writeDatagram( filestr.toLocal8Bit() , QHostAddress(m_receiveIp), RECEVIE_PORT);
}

void MusicFileSenderServer::sendData()
{
    if(!m_file->atEnd())
    {
        QByteArray data = m_file->read(MAX_DATA);
        m_sendSocket->writeDatagram(data , QHostAddress(m_receiveIp), RECEVIE_PORT);
        if(data.size() < MAX_DATA)
        {
            emit fileSendFinished();
            m_nextFile = true;
        }
    }
}

void MusicFileSenderServer::readPendingDatagrams()
{
    while(m_sendSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_sendSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_sendSocket->readDatagram(datagram.data(), datagram.size(),
                                   &sender, &senderPort);

        if(m_nextFile)
        {
            m_file->close();
            m_nextFile = false;
            start();
        }
        else if(datagram == DATAGRAM)
        {
            sendData();
        }
    }
}
