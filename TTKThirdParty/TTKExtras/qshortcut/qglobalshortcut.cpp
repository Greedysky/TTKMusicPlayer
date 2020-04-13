#include "qglobalshortcut_p.h"

bool QGlobalShortcutPrivate::error = false;
int QGlobalShortcutPrivate::ref = 0;

#ifndef TTK_GREATER_NEW
QAbstractEventDispatcher::EventFilter QGlobalShortcutPrivate::prevEventFilter = 0;
#endif

QHash<QPair<quint32, quint32>, QGlobalShortcut*> QGlobalShortcutPrivate::shortcuts;

QGlobalShortcutPrivate::QGlobalShortcutPrivate()
    : enabled(true),
      key(Qt::Key(0)),
      mods(Qt::NoModifier)
{
    if(!ref++)
    {
#ifndef TTK_GREATER_NEW
        prevEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(eventFilter);
#else
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
#endif
    }
}

QGlobalShortcutPrivate::~QGlobalShortcutPrivate()
{
    if(!--ref)
    {
#ifndef TTK_GREATER_NEW
        QAbstractEventDispatcher::instance()->setEventFilter(prevEventFilter);
#else
        QAbstractEventDispatcher::instance()->removeNativeEventFilter(this);
#endif
    }
}

bool QGlobalShortcutPrivate::setShortcut(const QKeySequence &shortcut)
{
    Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    key = shortcut.isEmpty() ? Qt::Key(0) : Qt::Key((shortcut[0] ^ allMods) & shortcut[0]);
    mods = shortcut.isEmpty() ? Qt::KeyboardModifiers(0) : Qt::KeyboardModifiers(shortcut[0] & allMods);

    const quint32 nativeKey = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);
    const bool res = registerShortcut(nativeKey, nativeMods);
    shortcuts.insert(qMakePair(nativeKey, nativeMods), ttk_q());

    if(!res)
    {
        qWarning() << "QGlobalShortcut failed to register:" << QKeySequence(key + mods).toString();
    }
    return res;
}

bool QGlobalShortcutPrivate::unsetShortcut()
{
    const quint32 nativeKey = nativeKeycode(key);
    const quint32 nativeMods = nativeModifiers(mods);
    const bool res = unregisterShortcut(nativeKey, nativeMods);
    shortcuts.remove(qMakePair(nativeKey, nativeMods));

    if(!res)
    {
        qWarning() << "QGlobalShortcut failed to unregister:" << QKeySequence(key + mods).toString();
    }

    key = Qt::Key(0);
    mods = Qt::KeyboardModifiers(0);
    return res;
}

void QGlobalShortcutPrivate::activateShortcut(quint32 nativeKey, quint32 nativeMods)
{
    QGlobalShortcut* shortcut = shortcuts.value(qMakePair(nativeKey, nativeMods));
    if(shortcut && shortcut->isEnabled())
    {
        Q_EMIT shortcut->activated();
    }
}

QGlobalShortcut::QGlobalShortcut(QObject* parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
}

QGlobalShortcut::QGlobalShortcut(const QKeySequence& shortcut, QObject* parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE;
    setShortcut(shortcut);
}

QGlobalShortcut::~QGlobalShortcut()
{
    TTK_D(QGlobalShortcut);
    if(d->key != 0)
    {
        d->unsetShortcut();
    }
}

QKeySequence QGlobalShortcut::shortcut() const
{
    TTK_D(QGlobalShortcut);
    return QKeySequence(d->key | d->mods);
}

bool QGlobalShortcut::setShortcut(const QKeySequence& shortcut)
{
    TTK_D(QGlobalShortcut);
    if(d->key != 0)
    {
        d->unsetShortcut();
    }
    return d->setShortcut(shortcut);
}

bool QGlobalShortcut::isEnabled() const
{
    TTK_D(QGlobalShortcut);
    return d->enabled;
}

void QGlobalShortcut::setEnabled(bool enabled)
{
    TTK_D(QGlobalShortcut);
    d->enabled = enabled;
}

void QGlobalShortcut::setDisabled(bool disabled)
{
    TTK_D(QGlobalShortcut);
    d->enabled = !disabled;
}
