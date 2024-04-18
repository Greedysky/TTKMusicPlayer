#include "qglobalshortcut_p.h"
#include <xcb/xcb.h>
#include <X11/Xlib.h>
#if !TTK_QT_VERSION_CHECK(6,0,0) && TTK_QT_VERSION_CHECK(5,1,0) || !TTK_QT_VERSION_CHECK(5,0,0)
#include <QX11Info>
static Display *X11Display()
{
    return QX11Info::display();
}

static Window X11RootWindow()
{
    return QX11Info::appRootWindow();
}
#else
static Display *X11Display()
{
    static Display *display = XOpenDisplay(/*getenv("DISPLAY")*/NULL);
    return display;
}

static Window X11RootWindow()
{
    return XRootWindow(X11Display(), -1);
}
#endif

//static int (*original_x_errhandler)(Display *display, XErrorEvent *event);

//static int q_x_errhandler(Display *display, XErrorEvent *event)
//{
//    Q_UNUSED(display);
//    switch(event->error_code)
//    {
//        case BadAccess:
//        case BadValue:
//        case BadWindow:
//            if(event->request_code == 33 /* X_GrabKey */ || event->request_code == 34 /* X_UngrabKey */)
//            {
//                QGlobalShortcutPrivate::m_error = true;
//                //TODO:
//                //char errstr[256];
//                //XGetErrorText(dpy, err->error_code, errstr, 256);
//            }
//        default: return 0;
//    }
//}

#if !TTK_QT_VERSION_CHECK(5,0,0)
bool QGlobalShortcutPrivate::eventFilter(void *message)
{
    XEvent *event = TTKStaticCast(XEvent*, message);
    if(event->type == KeyPress)
    {
        XKeyEvent *key = TTKReinterpretCast(XKeyEvent*, event);
        unsigned int keycode = key->keycode;
        unsigned int keystate = key->state;
#else
bool QGlobalShortcutPrivate::nativeEventFilter(const QByteArray &type, void *message, qint *)
{
    xcb_key_press_event_t *kev = nullptr;
    if(type == "xcb_generic_event_t")
    {
        xcb_generic_event_t *event = TTKStaticCast(xcb_generic_event_t*, message);
        if((event->response_type & 127) == XCB_KEY_PRESS)
        {
            kev = TTKStaticCast(xcb_key_press_event_t*, message);
        }
    }

    if(kev != nullptr)
    {
        unsigned int keycode = kev->detail;
        unsigned int keystate = 0;

        if(kev->state & XCB_MOD_MASK_1)
            keystate |= Mod1Mask;
        if(kev->state & XCB_MOD_MASK_CONTROL)
            keystate |= ControlMask;
        if(kev->state & XCB_MOD_MASK_4)
            keystate |= Mod4Mask;
        if(kev->state & XCB_MOD_MASK_SHIFT)
            keystate |= ShiftMask;
#endif
        activateShortcut(keycode,
            // Mod1Mask == Alt, Mod4Mask == Meta
            keystate & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    }
    return false;
}

quint32 QGlobalShortcutPrivate::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    // ShiftMask, LockMask, ControlMask, Mod1Mask, Mod2Mask, Mod3Mask, Mod4Mask, and Mod5Mask
    quint32 native = 0;
    if(modifiers & Qt::ShiftModifier)
        native |= ShiftMask;
    if(modifiers & Qt::ControlModifier)
        native |= ControlMask;
    if(modifiers & Qt::AltModifier)
        native |= Mod1Mask;
    if(modifiers & Qt::MetaModifier)
        native |= Mod4Mask;

    return native;
}

quint32 QGlobalShortcutPrivate::nativeKeycode(Qt::Key key)
{
    Display *display = X11Display();
    return display ? XKeysymToKeycode(display, XStringToKeysym(QKeySequence(key).toString().toLatin1().data())) : 0;
}

bool QGlobalShortcutPrivate::registerShortcut(quint32 nativeKey, quint32 nativeMods)
{
    Display *display = X11Display();
    if(!display)
    {
        m_error = true;
        return false;
    }

    Window window = X11RootWindow();
    Bool owner = True;
    int pointer = GrabModeAsync;
    int keyboard = GrabModeAsync;
    m_error = false;

//    original_x_errhandler = XSetErrorHandler(q_x_errhandler);
    XGrabKey(display, nativeKey, nativeMods, window, owner, pointer, keyboard);
    XGrabKey(display, nativeKey, nativeMods | Mod2Mask, window, owner, pointer, keyboard); // allow numlock
    XSync(display, False);
//    XSetErrorHandler(original_x_errhandler);
    return !m_error;
}

bool QGlobalShortcutPrivate::unregisterShortcut(quint32 nativeKey, quint32 nativeMods)
{
    Display *display = X11Display();
    if(!display)
    {
        m_error = true;
        return false;
    }

    Window window = X11RootWindow();
    m_error = false;

//    original_x_errhandler = XSetErrorHandler(q_x_errhandler);
    XUngrabKey(display, nativeKey, nativeMods, window);
    XUngrabKey(display, nativeKey, nativeMods | Mod2Mask, window); // allow numlock
    XSync(display, False);
//    XSetErrorHandler(original_x_errhandler);
    return !m_error;
}
