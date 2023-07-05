#include "ttkrunapplication.h"
#include "ttklocalpeer.h"

#include <QWidget>

/*! @brief The class of the ttk run application private.
 * @author Greedysky <greedysky@163.com>
 */
class TTKRunApplicationPrivate : public TTKPrivate<TTKRunApplication>
{
public:
    TTKRunApplicationPrivate();
    ~TTKRunApplicationPrivate();

    TTKLocalPeer *m_peer;
    QWidget *m_activeWindow;
};

TTKRunApplicationPrivate::TTKRunApplicationPrivate()
    : m_peer(nullptr),
      m_activeWindow(nullptr)
{

}

TTKRunApplicationPrivate::~TTKRunApplicationPrivate()
{
    delete m_peer;
}



TTKRunApplication::TTKRunApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKRunApplication);
    initialize();
}

TTKRunApplication::TTKRunApplication(const QString &id, int &argc, char **argv)
    : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE(TTKRunApplication);
    initialize(id);
}

bool TTKRunApplication::isRunning() const
{
    TTK_D(TTKRunApplication);
    return d->m_peer->isClient();
}

QString TTKRunApplication::id() const
{
    TTK_D(TTKRunApplication);
    return d->m_peer->applicationId();
}

void TTKRunApplication::setActivationWindow(QWidget* aw, bool activateOnMessage) const
{
    TTK_D(TTKRunApplication);
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

QWidget* TTKRunApplication::activationWindow() const
{
    TTK_D(TTKRunApplication);
    return d->m_activeWindow;
}

bool TTKRunApplication::sendMessage(const QString &message, int timeout)
{
    TTK_D(TTKRunApplication);
    return d->m_peer->sendMessage(message, timeout);
}

void TTKRunApplication::activateWindow()
{
    TTK_D(TTKRunApplication);
    if(d->m_activeWindow)
    {
        d->m_activeWindow->setWindowState(d->m_activeWindow->windowState() & ~Qt::WindowMinimized);
        d->m_activeWindow->raise();
        d->m_activeWindow->activateWindow();
    }
}

void TTKRunApplication::initialize(const QString &id)
{
    TTK_D(TTKRunApplication);
    d->m_peer = new TTKLocalPeer(this, id);
    connect(d->m_peer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
}
