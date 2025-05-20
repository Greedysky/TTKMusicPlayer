#include "ttkapplication.h"
#include "ttklocalpeer.h"

#include <QWidget>

/*! @brief The class of the ttk application private.
 * @author Greedysky <greedysky@163.com>
 */
class TTKApplicationPrivate : public TTKPrivate<TTKApplication>
{
public:
    TTKApplicationPrivate() noexcept;
    ~TTKApplicationPrivate() noexcept;

    TTKLocalPeer *m_peer;
    QWidget *m_activeWindow;
};

TTKApplicationPrivate::TTKApplicationPrivate() noexcept
    : m_peer(nullptr),
      m_activeWindow(nullptr)
{

}

TTKApplicationPrivate::~TTKApplicationPrivate() noexcept
{
    delete m_peer;
}



TTKApplication::TTKApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKApplication);
    initialize();
}

TTKApplication::TTKApplication(const QString &id, int &argc, char **argv)
    : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKApplication);
    initialize(id);
}

bool TTKApplication::isRunning() const
{
    TTK_D(const TTKApplication);
    return d->m_peer->isClient();
}

QString TTKApplication::id() const noexcept
{
    TTK_D(const TTKApplication);
    return d->m_peer->applicationId();
}

void TTKApplication::setActivationWindow(QWidget* aw, bool activateOnMessage) const noexcept
{
    TTK_D(TTKApplication);
    d->m_activeWindow = aw;

    if(activateOnMessage)
    {
        connect(d->m_peer, SIGNAL(messageReceived(QString)), this, SLOT(activateWindow()));
    }
    else
    {
        disconnect(d->m_peer, SIGNAL(messageReceived(QString)), this, SLOT(activateWindow()));
    }
}

QWidget* TTKApplication::activationWindow() const noexcept
{
    TTK_D(const TTKApplication);
    return d->m_activeWindow;
}

bool TTKApplication::sendMessage(const QString &message, int timeout)
{
    TTK_D(const TTKApplication);
    return d->m_peer->sendMessage(message, timeout);
}

void TTKApplication::activateWindow()
{
    TTK_D(TTKApplication);
    if(d->m_activeWindow)
    {
        d->m_activeWindow->setWindowState(d->m_activeWindow->windowState() & ~Qt::WindowMinimized);
        d->m_activeWindow->raise();
        d->m_activeWindow->activateWindow();
    }
}

void TTKApplication::initialize(const QString &id)
{
    TTK_D(TTKApplication);
    d->m_peer = new TTKLocalPeer(this, id);
    connect(d->m_peer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
}
