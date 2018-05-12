#include "ttkrunapplication.h"
#include "ttklocalpeer.h"

#include <QWidget>

class TTKRunApplicationPrivate : public TTKPrivate<TTKRunApplication>
{
public:
    TTKRunApplicationPrivate();
    ~TTKRunApplicationPrivate();

    TTKLocalPeer *m_peer;
    QWidget *m_activeWindow;
};

TTKRunApplicationPrivate::TTKRunApplicationPrivate()
{
    m_peer = nullptr;
    m_activeWindow = nullptr;
}

TTKRunApplicationPrivate::~TTKRunApplicationPrivate()
{
    delete m_peer;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///

TTKRunApplication::TTKRunApplication(int &argc, char **argv, bool GUIenabled)
    : QApplication(argc, argv, GUIenabled)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

TTKRunApplication::TTKRunApplication(const QString &appId, int &argc, char **argv)
    : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE;
    sysInit(appId);
}

#if QT_VERSION < 0x050000
TTKRunApplication::TTKRunApplication(int &argc, char **argv, Type type)
    : QApplication(argc, argv, type)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

#  if defined(Q_WS_X11)
TTKRunApplication::TTKRunApplication(Display* dpy, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, visual, cmap)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

TTKRunApplication::TTKRunApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

TTKRunApplication::TTKRunApplication(Display* dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap)
{
    TTK_INIT_PRIVATE;
    sysInit(appId);
}
#  endif
#endif

void TTKRunApplication::initialize(bool dummy)
{
    Q_UNUSED(dummy);
    isRunning();
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

void TTKRunApplication::setActivationWindow(QWidget* aw, bool activateOnMessage)
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

void TTKRunApplication::sysInit(const QString &appId)
{
    TTK_D(TTKRunApplication);
    d->m_peer = new TTKLocalPeer(this, appId);
    connect(d->m_peer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
}
