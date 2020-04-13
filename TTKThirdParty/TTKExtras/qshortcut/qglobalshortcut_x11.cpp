#include "qglobalshortcut_p.h"
#include <QX11Info>
#include <X11/Xlib.h>
#include <xcb/xcb.h>

static int (*original_x_errhandler)(Display* display, XErrorEvent* event);

static int q_x_errhandler(Display* display, XErrorEvent *event)
{
    Q_UNUSED(display);
    switch(event->error_code)
    {
        case BadAccess:
        case BadValue:
        case BadWindow:
            if(event->request_code == 33 /* X_GrabKey */ || event->request_code == 34 /* X_UngrabKey */)
            {
                QGlobalShortcutPrivate::error = true;
                //TODO:
                //char errstr[256];
                //XGetErrorText(dpy, err->error_code, errstr, 256);
            }
        default: return 0;
    }
}

#ifndef TTK_GREATER_NEW
bool QGlobalShortcutPrivate::eventFilter(void* message)
{
    XEvent *event = TTKStatic_cast(XEvent*, message);
    if(event->type == KeyPress)
    {
        XKeyEvent* key = TTKReinterpret_cast(XKeyEvent*, event);
        unsigned int keycode = key->keycode;
        unsigned int keystate = key->state;
#else
bool QGlobalShortcutPrivate::nativeEventFilter(const QByteArray &type, void *message, long *)
{
    xcb_key_press_event_t *kev = nullptr;
    if(type == "xcb_generic_event_t")
    {
        xcb_generic_event_t* event = TTKStatic_cast(xcb_generic_event_t*, message);
        if((event->response_type & 127) == XCB_KEY_PRESS)
        {
            kev = TTKStatic_cast(xcb_key_press_event_t*, message);
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
    Display* display = QX11Info::display();
    return XKeysymToKeycode(display, XStringToKeysym(QKeySequence(key).toString().toLatin1().data()));
}

bool QGlobalShortcutPrivate::registerShortcut(quint32 nativeKey, quint32 nativeMods)
{
    Display* display = QX11Info::display();
    Window window = QX11Info::appRootWindow();
    Bool owner = True;
    int pointer = GrabModeAsync;
    int keyboard = GrabModeAsync;
    error = false;

    original_x_errhandler = XSetErrorHandler(q_x_errhandler);
    XGrabKey(display, nativeKey, nativeMods, window, owner, pointer, keyboard);
    XGrabKey(display, nativeKey, nativeMods | Mod2Mask, window, owner, pointer, keyboard); // allow numlock
    XSync(display, False);
    XSetErrorHandler(original_x_errhandler);
    return !error;
}

bool QGlobalShortcutPrivate::unregisterShortcut(quint32 nativeKey, quint32 nativeMods)
{
    Display* display = QX11Info::display();
    Window window = QX11Info::appRootWindow();
    error = false;

    original_x_errhandler = XSetErrorHandler(q_x_errhandler);
    XUngrabKey(display, nativeKey, nativeMods, window);
    XUngrabKey(display, nativeKey, nativeMods | Mod2Mask, window); // allow numlock
    XSync(display, False);
    XSetErrorHandler(original_x_errhandler);
    return !error;
}
