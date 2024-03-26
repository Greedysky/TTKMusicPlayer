#include "ttkcoreapplication.h"
#include "ttklocalpeer.h"

/*! @brief The class of the ttk core application private.
 * @author Greedysky <greedysky@163.com>
 */
class TTKCoreApplicationPrivate : public TTKPrivate<TTKCoreApplication>
{
public:
    TTKCoreApplicationPrivate();
    ~TTKCoreApplicationPrivate();

    TTKLocalPeer *m_peer;
};

TTKCoreApplicationPrivate::TTKCoreApplicationPrivate()
    : m_peer(nullptr)
{

}

TTKCoreApplicationPrivate::~TTKCoreApplicationPrivate()
{
    delete m_peer;
}



TTKCoreApplication::TTKCoreApplication(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKCoreApplication);
    initialize();
}

TTKCoreApplication::TTKCoreApplication(const QString &id, int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKCoreApplication);
    initialize(id);
}

bool TTKCoreApplication::isRunning() const
{
    TTK_D(TTKCoreApplication);
    return d->m_peer->isClient();
}

QString TTKCoreApplication::id() const
{
    TTK_D(TTKCoreApplication);
    return d->m_peer->applicationId();
}

bool TTKCoreApplication::sendMessage(const QString &message, int timeout)
{
    TTK_D(TTKCoreApplication);
    return d->m_peer->sendMessage(message, timeout);
}

void TTKCoreApplication::initialize(const QString &id)
{
    TTK_D(TTKCoreApplication);
    d->m_peer = new TTKLocalPeer(this, id);
    connect(d->m_peer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
}
