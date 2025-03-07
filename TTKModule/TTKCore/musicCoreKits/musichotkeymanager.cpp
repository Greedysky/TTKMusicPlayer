#include "musichotkeymanager.h"
#include "qshortcut/qglobalshortcut.h"

#include <QStringList>

void MusicHotKeyManager::addHotKey(QObject *object)
{
    QGlobalShortcut *shortcut = new QGlobalShortcut(object);
    m_hotkeys << shortcut;
}

void MusicHotKeyManager::addHotKey(QObject *object, const char *slot)
{
    QGlobalShortcut *shortcut = new QGlobalShortcut(object);
    m_hotkeys << shortcut;

    connect(shortcut, SIGNAL(activated()), object, slot);
}

void MusicHotKeyManager::addHotKey(QObject *object, const QString &key, const char *slot)
{
    QGlobalShortcut *shortcut = new QGlobalShortcut(key, object);
    m_hotkeys << shortcut;

    connect(shortcut, SIGNAL(activated()), object, slot);
}

void MusicHotKeyManager::setHotKey(int index, const QString &key)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }

    m_hotkeys[index]->setShortcut(QKeySequence(key));
}

QObject* MusicHotKeyManager::hotKey(int index)
{
    if(index >= m_hotkeys.count())
    {
        return nullptr;
    }

    return m_hotkeys[index];
}

void MusicHotKeyManager::unsetShortcut()
{
    for(QGlobalShortcut *key : qAsConst(m_hotkeys))
    {
        key->unsetShortcut();
        key->setEnabled(false);
    }
}

void MusicHotKeyManager::setEnabled(bool enabled)
{
    for(QGlobalShortcut *key : qAsConst(m_hotkeys))
    {
        key->setEnabled(enabled);
    }
}

void MusicHotKeyManager::setEnabled(int index, bool enabled)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }

    m_hotkeys[index]->setEnabled(enabled);
}

bool MusicHotKeyManager::isEnabled(int index)
{
    if(index >= m_hotkeys.count())
    {
        return false;
    }

    return m_hotkeys[index]->isEnabled();
}

QString MusicHotKeyManager::toString(int key, int modifiers)
{
    const QString strModList[] = {"Ctrl", "Shift", "Alt"};
    const quint32 modList[] = {Qt::ControlModifier, Qt::ShiftModifier, Qt::AltModifier};

    QString keyStr;
    for(int i = 0; i < 3; ++i)
    {
        if(modifiers & modList[i])
        {
            keyStr.append(strModList[i] + "+");
        }
    }

    if(key == Qt::Key_Shift || key == Qt::Key_Control || key == Qt::Key_Alt)
    {
        keyStr.chop(1);
        return keyStr;
    }

    return keyStr + QKeySequence(key).toString();
}

int MusicHotKeyManager::count() const
{
    return m_hotkeys.count();
}

QStringList MusicHotKeyManager::defaultKeys() const
{
    QStringList keys;
    keys << "Ctrl+B" << "Ctrl+S" << "Ctrl+Left" << "Ctrl+Right"
         << "Ctrl+Up" << "Ctrl+Down" << "Ctrl+M" << "Ctrl+I"
         << "Ctrl+L" << "Ctrl+W" << "Ctrl+Alt+I" << "Ctrl+Alt+O"
         << "Ctrl+Alt+S" << "Ctrl+N" << "Ctrl+A" << "Ctrl+Q";
    return keys;
}

QStringList MusicHotKeyManager::keys() const
{
    QStringList keys;
    for(QGlobalShortcut *key : qAsConst(m_hotkeys))
    {
        keys << key->shortcut().toString();
    }
    return keys;
}
