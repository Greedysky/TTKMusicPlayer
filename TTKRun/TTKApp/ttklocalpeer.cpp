#include "ttklocalpeer.h"
#include "ttklockedfile.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QTime>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDir>

#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QRegularExpression>
#endif

#if defined(Q_OS_WIN)
#  include <QLibrary>
#  include <qt_windows.h>
typedef BOOL(WINAPI*PProcessIdToSessionId)(DWORD,DWORD*);
static PProcessIdToSessionId pProcessIdToSessionId = 0;
#endif
#if defined(Q_OS_UNIX)
#  include <sys/types.h>
#  include <time.h>
#  include <unistd.h>
#endif

namespace TTKLockedPrivate {
#include "ttklockedfile.cpp"
#if defined(Q_OS_WIN)
#  include "ttklockedfile_win.cpp"
#else
#  include "ttklockedfile_unix.cpp"
#endif
}

/*! @brief The class of the ttk local peer private.
 * @author Greedysky <greedysky@163.com>
 */
class TTKLocalPeerPrivate : public TTKPrivate<TTKLocalPeer>
{
public:
    TTKLocalPeerPrivate();
    ~TTKLocalPeerPrivate();

    QString m_id;
    QString m_socketName;
    QLocalServer *m_server;
    TTKLockedPrivate::TTKLockedFile m_lockFile;
    static const char *m_ack;
};
const char *TTKLocalPeerPrivate::m_ack = "ack";

TTKLocalPeerPrivate::TTKLocalPeerPrivate()
{
    m_server = nullptr;
}

TTKLocalPeerPrivate::~TTKLocalPeerPrivate()
{
    delete m_server;
}



TTKLocalPeer::TTKLocalPeer(QObject *parent, const QString &appId)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(TTKLocalPeer);
    TTK_D(TTKLocalPeer);

    QString prefix = d->m_id = appId;
    if(prefix.isEmpty())
    {
        d->m_id = QCoreApplication::applicationFilePath();
#if defined(Q_OS_WIN)
        d->m_id = d->m_id.toLower();
#endif
        prefix = d->m_id.section(QLatin1Char('/'), -1);
    }
#if TTK_QT_VERSION_CHECK(5,0,0)
    prefix.remove(QRegularExpression("[^a-zA-Z]"));
#else
    prefix.remove(QRegExp("[^a-zA-Z]"));
#endif
    prefix.truncate(6);

    const QByteArray &idc = d->m_id.toUtf8();
#if TTK_QT_VERSION_CHECK(6,0,0)
    quint16 idNum = qChecksum(QByteArrayView(idc.constData(), idc.length()));
#else
    quint16 idNum = qChecksum(idc.constData(), idc.length());
#endif
    d->m_socketName = QLatin1String("qtsingleapp-") + prefix + QLatin1Char('-') + QString::number(idNum, 16);

#if defined(Q_OS_WIN)
    if(!pProcessIdToSessionId)
    {
        QLibrary lib("kernel32");
        pProcessIdToSessionId = (PProcessIdToSessionId)lib.resolve("ProcessIdToSessionId");
    }
    if(pProcessIdToSessionId)
    {
        DWORD sessionId = 0;
        pProcessIdToSessionId(GetCurrentProcessId(), &sessionId);
        d->m_socketName += QLatin1Char('-') + QString::number(sessionId, 16);
    }
#else
    d->m_socketName += QLatin1Char('-') + QString::number(::getuid(), 16);
#endif

    d->m_server = new QLocalServer(this);
    const QString &lockName = QDir(QDir::tempPath()).absolutePath() + QLatin1Char('/') + d->m_socketName + QLatin1String("-lockfile");
    d->m_lockFile.setFileName(lockName);
    d->m_lockFile.open(QIODevice::ReadWrite);
}

bool TTKLocalPeer::isClient() const
{
    TTK_D(TTKLocalPeer);
    if(d->m_lockFile.isLocked())
    {
        return false;
    }

    if(!d->m_lockFile.lock(TTKLockedPrivate::TTKLockedFile::WriteLock, false))
    {
        return true;
    }

    bool res = d->m_server->listen(d->m_socketName);
#if defined(Q_OS_UNIX) && TTK_QT_VERSION_CHECK(4,5,0)
    // ### Workaround
    if(!res && d->m_server->serverError() == QAbstractSocket::AddressInUseError)
    {
        QFile::remove(QDir::cleanPath(QDir::tempPath()) + QLatin1Char('/') + d->m_socketName);
        res = d->m_server->listen(d->m_socketName);
    }
#endif
    if(!res)
    {
        TTK_LOGGER_WARN("Application: listen on local socket failed, " << qPrintable(d->m_server->errorString()));
    }

    connect(d->m_server, SIGNAL(newConnection()), SLOT(receiveConnection()));
    return false;
}

bool TTKLocalPeer::sendMessage(const QString &message, int timeout) const
{
    TTK_D(TTKLocalPeer);
    if(!isClient())
    {
        return false;
    }

    QLocalSocket socket;
    bool connOk = false;
    for(int i=0; i<2; i++)
    {
        // Try twice, in case the other instance is just starting up
        socket.connectToServer(d->m_socketName);
        connOk = socket.waitForConnected(timeout / 2);
        if(connOk || i)
        {
            break;
        }
        const int ms = 250;
#if defined(Q_OS_WIN)
        Sleep(DWORD(ms));
#else
        struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, nullptr);
#endif
    }
    if(!connOk)
    {
        return false;
    }

    const QByteArray uMsg(message.toUtf8());
    QDataStream ds(&socket);
    ds.writeBytes(uMsg.constData(), uMsg.length());

    bool res = socket.waitForBytesWritten(timeout);
    if(res)
    {
        res &= socket.waitForReadyRead(timeout);   // wait for ack
        if(res)
        {
            res &= (socket.read(qstrlen(d->m_ack)) == d->m_ack);
        }
    }
    return res;
}

QString TTKLocalPeer::applicationId() const
{
    TTK_D(TTKLocalPeer);
    return d->m_id;
}

void TTKLocalPeer::receiveConnection()
{
    TTK_D(TTKLocalPeer);
    QLocalSocket *socket = d->m_server->nextPendingConnection();
    if(!socket)
    {
        return;
    }

    while(true)
    {
        if(socket->state() == QLocalSocket::UnconnectedState)
        {
            TTK_LOGGER_WARN("QtLocalPeer: Peer disconnected");
            delete socket;
            return;
        }
        
        if(socket->bytesAvailable() >= qint64(sizeof(quint32)))
        {
            break;
        }
        socket->waitForReadyRead();
    }

    QDataStream ds(socket);
    QByteArray uMsg;
    quint32 remaining;
    ds >> remaining;
    uMsg.resize(remaining);

    int got = 0;
    char *uMsgBuf = uMsg.data();
    do
    {
        got = ds.readRawData(uMsgBuf, remaining);
        remaining -= got;
        uMsgBuf += got;
    } while(remaining && got >= 0 && socket->waitForReadyRead(2000));

    if(got < 0)
    {
        TTK_LOGGER_WARN("TTKLocalPeer: Message reception failed " << socket->errorString().toLatin1().constData());
        delete socket;
        return;
    }

    const QString message(QString::fromUtf8(uMsg));
    socket->write(d->m_ack, qstrlen(d->m_ack));
    socket->waitForBytesWritten(1000);
    socket->waitForDisconnected(1000); // make sure client reads ack
    delete socket;

    Q_EMIT messageReceived(message); //### (might take a long time to return)
}
