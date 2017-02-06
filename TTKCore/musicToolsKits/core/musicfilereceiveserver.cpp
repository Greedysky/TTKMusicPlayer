#include "musicfilereceiveserver.h"

#include <QFileInfo>

MusicFileReceiveServer::MusicFileReceiveServer(QObject *parent)
    : QObject(parent)
{
    m_receiveSocket = new QUdpSocket(this);
    m_receiveSocket->bind(QHostAddress::Any, RECEVIE_PORT);
    connect(m_receiveSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

    m_file = new QFile(this);
}

MusicFileReceiveServer::~MusicFileReceiveServer()
{
    delete m_receiveSocket;
    delete m_file;
}

QString MusicFileReceiveServer::getClassName()
{
    return staticMetaObject.className();
}

void MusicFileReceiveServer::setSavePathDir(const QString &dir)
{
    m_saveDir = dir;
}

void MusicFileReceiveServer::readPendingDatagrams()
{
    while(m_receiveSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_receiveSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_receiveSocket->readDatagram(datagram.data(), datagram.size(),
                                      &sender, &senderPort);

        if(datagram.contains(SPLITE_STR))
        {
            if(m_saveDir.isEmpty())
            {
                M_LOGGER_ERROR("The Save Path Is Empty!");
                return;
            }
            QString path = QString(datagram).split(SPLITE_STR).first();
            m_file->close();
            m_file->setFileName(m_saveDir + "/" + QFileInfo(path).fileName() );
            m_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered);
        }
        else
        {
            m_file->write(datagram.data(),datagram.size());
        }

        QByteArray msg(DATAGRAM);
        m_receiveSocket->writeDatagram(msg, msg.length(), sender, SEND_PORT);
    }
}
