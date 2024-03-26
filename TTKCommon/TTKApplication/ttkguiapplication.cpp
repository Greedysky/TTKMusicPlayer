#include "ttkguiapplication.h"
#include "ttklocalpeer.h"

#include <QWidget>

/*! @brief The class of the ttk gui application private.
 * @author Greedysky <greedysky@163.com>
 */
class TTKGuiApplicationPrivate : public TTKPrivate<TTKGuiApplication>
{
public:
    TTKGuiApplicationPrivate();
    ~TTKGuiApplicationPrivate();

    TTKLocalPeer *m_peer;
};

TTKGuiApplicationPrivate::TTKGuiApplicationPrivate()
    : m_peer(nullptr)
{

}

TTKGuiApplicationPrivate::~TTKGuiApplicationPrivate()
{
    delete m_peer;
}



TTKGuiApplication::TTKGuiApplication(int &argc, char **argv)
    : QGuiApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKGuiApplication);
    initialize();
}

TTKGuiApplication::TTKGuiApplication(const QString &id, int &argc, char **argv)
    : QGuiApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKGuiApplication);
    initialize(id);
}

bool TTKGuiApplication::isRunning() const
{
    TTK_D(TTKGuiApplication);
    return d->m_peer->isClient();
}

QString TTKGuiApplication::id() const
{
    TTK_D(TTKGuiApplication);
    return d->m_peer->applicationId();
}

bool TTKGuiApplication::sendMessage(const QString &message, int timeout)
{
    TTK_D(TTKGuiApplication);
    return d->m_peer->sendMessage(message, timeout);
}

void TTKGuiApplication::initialize(const QString &id)
{
    TTK_D(TTKGuiApplication);
    d->m_peer = new TTKLocalPeer(this, id);
    connect(d->m_peer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
}
