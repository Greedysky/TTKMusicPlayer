#include "musicrunapplication.h"
#include "musiclocalpeer.h"

#include <QWidget>

class MusicRunApplicationPrivate : public TTKPrivate<MusicRunApplication>
{
public:
    MusicRunApplicationPrivate();
    ~MusicRunApplicationPrivate();

    MusicLocalPeer *m_peer;
    QWidget *m_activeWindow;
};

MusicRunApplicationPrivate::MusicRunApplicationPrivate()
{
    m_peer = nullptr;
    m_activeWindow = nullptr;
}

MusicRunApplicationPrivate::~MusicRunApplicationPrivate()
{
    delete m_peer;
}

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
///
///

MusicRunApplication::MusicRunApplication(int &argc, char **argv, bool GUIenabled)
    : QApplication(argc, argv, GUIenabled)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

MusicRunApplication::MusicRunApplication(const QString &appId, int &argc, char **argv)
    : QApplication(argc, argv)
{
    TTK_INIT_PRIVATE;
    sysInit(appId);
}

#if QT_VERSION < 0x050000
MusicRunApplication::MusicRunApplication(int &argc, char **argv, Type type)
    : QApplication(argc, argv, type)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

#  if defined(Q_WS_X11)
MusicRunApplication::MusicRunApplication(Display* dpy, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, visual, cmap)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

MusicRunApplication::MusicRunApplication(Display *dpy, int &argc, char **argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap)
{
    TTK_INIT_PRIVATE;
    sysInit();
}

MusicRunApplication::MusicRunApplication(Display* dpy, const QString &appId, int argc, char **argv, Qt::HANDLE visual, Qt::HANDLE cmap)
    : QApplication(dpy, argc, argv, visual, cmap)
{
    TTK_INIT_PRIVATE;
    sysInit(appId);
}
#  endif
#endif

void MusicRunApplication::initialize(bool dummy)
{
    Q_UNUSED(dummy);
    isRunning();
}

bool MusicRunApplication::isRunning() const
{
    TTK_D(MusicRunApplication);
    return d->m_peer->isClient();
}

QString MusicRunApplication::id() const
{
    TTK_D(MusicRunApplication);
    return d->m_peer->applicationId();
}

void MusicRunApplication::setActivationWindow(QWidget* aw, bool activateOnMessage)
{
    TTK_D(MusicRunApplication);
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

QWidget* MusicRunApplication::activationWindow() const
{
    TTK_D(MusicRunApplication);
    return d->m_activeWindow;
}

bool MusicRunApplication::sendMessage(const QString &message, int timeout)
{
    TTK_D(MusicRunApplication);
    return d->m_peer->sendMessage(message, timeout);
}

void MusicRunApplication::activateWindow()
{
    TTK_D(MusicRunApplication);
    if(d->m_activeWindow)
    {
        d->m_activeWindow->setWindowState(d->m_activeWindow->windowState() & ~Qt::WindowMinimized);
        d->m_activeWindow->raise();
        d->m_activeWindow->activateWindow();
    }
}

void MusicRunApplication::sysInit(const QString &appId)
{
    TTK_D(MusicRunApplication);
    d->m_peer = new MusicLocalPeer(this, appId);
    connect(d->m_peer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
}
