#include "qglobalshortcut_p.h"

bool QGlobalShortcutPrivate::m_error = false;
int QGlobalShortcutPrivate::m_ref = 0;

#if !TTK_QT_VERSION_CHECK(5,0,0)
QAbstractEventDispatcher::EventFilter QGlobalShortcutPrivate::m_prevEventFilter = 0;
#endif

QHash<QPair<quint32, quint32>, QGlobalShortcut*> QGlobalShortcutPrivate::m_shortcuts;

QGlobalShortcutPrivate::QGlobalShortcutPrivate()
    : m_enabled(true),
      m_key(Qt::Key(0)),
      m_mods(Qt::NoModifier)
{
    if(!m_ref++)
    {
#if !TTK_QT_VERSION_CHECK(5,0,0)
        m_prevEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(eventFilter);
#else
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
#endif
    }
}

QGlobalShortcutPrivate::~QGlobalShortcutPrivate()
{
    if(!--m_ref)
    {
#if !TTK_QT_VERSION_CHECK(5,0,0)
        QAbstractEventDispatcher::instance()->setEventFilter(m_prevEventFilter);
#else
        QAbstractEventDispatcher::instance()->removeNativeEventFilter(this);
#endif
    }
}

bool QGlobalShortcutPrivate::setShortcut(const QKeySequence &shortcut)
{
    Qt::KeyboardModifiers allMods = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    m_key = shortcut.isEmpty() ? Qt::Key(0) : Qt::Key((QtKeyCombine(shortcut[0]) ^ allMods) & QtKeyCombine(shortcut[0]));
    m_mods = shortcut.isEmpty() ? Qt::KeyboardModifiers() : Qt::KeyboardModifiers(QtKeyCombine(shortcut[0]) & allMods);

    const quint32 nativeKey = nativeKeycode(m_key);
    const quint32 nativeMods = nativeModifiers(m_mods);
    const bool res = registerShortcut(nativeKey, nativeMods);
    m_shortcuts.insert(qMakePair(nativeKey, nativeMods), ttk_q());

    if(!res)
    {
        const QKeySequence sequence = QtKeySequence(m_key, m_mods);
        qWarning() << "QGlobalShortcut failed to register:" << sequence.toString();
    }
    return res;
}

bool QGlobalShortcutPrivate::unsetShortcut()
{
    const quint32 nativeKey = nativeKeycode(m_key);
    const quint32 nativeMods = nativeModifiers(m_mods);
    const bool res = unregisterShortcut(nativeKey, nativeMods);
    m_shortcuts.remove(qMakePair(nativeKey, nativeMods));

    if(!res)
    {
        const QKeySequence sequence = QtKeySequence(m_key, m_mods);
        qWarning() << "QGlobalShortcut failed to unregister:" << sequence.toString();
    }

    m_key = Qt::Key(0);
    m_mods = Qt::KeyboardModifiers();
    return res;
}

void QGlobalShortcutPrivate::activateShortcut(quint32 nativeKey, quint32 nativeMods)
{
    QGlobalShortcut* shortcut = m_shortcuts.value(qMakePair(nativeKey, nativeMods));
    if(shortcut && shortcut->isEnabled())
    {
        Q_EMIT shortcut->activated();
    }
}

QGlobalShortcut::QGlobalShortcut(QObject* parent)
    : QObject(parent)
{
    TTK_INIT_PRIVATE(QGlobalShortcut);
}

QGlobalShortcut::QGlobalShortcut(const QKeySequence& shortcut, QObject* parent)
    : QGlobalShortcut(parent)
{
    setShortcut(shortcut);
}

QGlobalShortcut::~QGlobalShortcut()
{
    TTK_D(QGlobalShortcut);
    if(d->m_key != 0)
    {
        d->unsetShortcut();
    }
}

QKeySequence QGlobalShortcut::shortcut() const
{
    TTK_D(QGlobalShortcut);
    return QKeySequence(d->m_key | d->m_mods);
}

bool QGlobalShortcut::setShortcut(const QKeySequence& shortcut)
{
    TTK_D(QGlobalShortcut);
    unsetShortcut();
    return d->setShortcut(shortcut);
}

void QGlobalShortcut::unsetShortcut()
{
    TTK_D(QGlobalShortcut);
    if(d->m_key != 0)
    {
        d->unsetShortcut();
    }
}

bool QGlobalShortcut::isEnabled() const
{
    TTK_D(QGlobalShortcut);
    return d->m_enabled;
}

void QGlobalShortcut::setEnabled(bool enabled)
{
    TTK_D(QGlobalShortcut);
    d->m_enabled = enabled;
}

void QGlobalShortcut::setDisabled(bool disabled)
{
    TTK_D(QGlobalShortcut);
    d->m_enabled = !disabled;
}
