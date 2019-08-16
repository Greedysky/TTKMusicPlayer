#include "musichotkeymanager.h"
#include "qshortcut/qglobalshortcut.h"

#include <QStringList>

void MusicHotKeyManager::connectParentObject(QObject *object)
{
    for(int i=0; i<8; ++i)
    {
        m_hotkeys << (new QGlobalShortcut(object));
    }

    connect(m_hotkeys[0], SIGNAL(activated()), object, SLOT(musicStatePlay()));
    connect(m_hotkeys[1], SIGNAL(activated()), object, SLOT(musicPlayPrevious()));
    connect(m_hotkeys[2], SIGNAL(activated()), object, SLOT(musicPlayNext()));
    connect(m_hotkeys[3], SIGNAL(activated()), object, SLOT(musicActionVolumePlus()));
    connect(m_hotkeys[4], SIGNAL(activated()), object, SLOT(musicActionVolumeSub()));
    connect(m_hotkeys[5], SIGNAL(activated()), object, SLOT(musicSetting()));
    connect(m_hotkeys[6], SIGNAL(activated()), object, SLOT(musicImportSongs()));
    connect(m_hotkeys[7], SIGNAL(activated()), object, SLOT(musicVolumeMute()));

    setDefaultKey();
}

void MusicHotKeyManager::setDefaultKey()
{
    const QStringList &keys = getDefaultKeys();
    for(int i=0; i<m_hotkeys.count(); ++i)
    {
        setHotKey(i, keys[i]);
        setEnabled(i, false);
    }
}

void MusicHotKeyManager::setHotKeys(const QStringList &keys)
{
    for(int i=0; i<m_hotkeys.count(); ++i)
    {
        setHotKey(i, keys[i]);
        setEnabled(i, false);
    }
}

void MusicHotKeyManager::setHotKey(int index, const QString &key)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }
    m_hotkeys[index]->setShortcut(QKeySequence(key));
}

void MusicHotKeyManager::setHotKey(int index, int key)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }
    m_hotkeys[index]->setShortcut(QKeySequence(key));
}

QString MusicHotKeyManager::hotKey(int index)
{
    if(index >= m_hotkeys.count())
    {
        return QString();
    }
    return m_hotkeys[index]->shortcut().toString();
}

QObject* MusicHotKeyManager::getHotKey(int index)
{
    if(index >= m_hotkeys.count())
    {
        return nullptr;
    }
    return m_hotkeys[index];
}

void MusicHotKeyManager::addHotKey(const QString &key)
{
    m_hotkeys << (new QGlobalShortcut(QKeySequence(key)));
}

void MusicHotKeyManager::addHotKey(int key)
{
    m_hotkeys << (new QGlobalShortcut(QKeySequence(key)));
}

void MusicHotKeyManager::setEnabled(int index, bool enabled)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }
    m_hotkeys[index]->setEnabled(enabled);
}

bool MusicHotKeyManager::enabled(int index)
{
    if(index >= m_hotkeys.count())
    {
        return false;
    }
    return m_hotkeys[index]->isEnabled();
}

void MusicHotKeyManager::enabledAll(bool enabled)
{
    foreach(QGlobalShortcut *key, m_hotkeys)
    {
        key->setEnabled(enabled);
    }
}

QString MusicHotKeyManager::toString(int key, int modifiers)
{
    const QString strModList[] = {"Ctrl", "Shift", "Alt"};
    const quint32 modList[] = {Qt::ControlModifier, Qt::ShiftModifier, Qt::AltModifier};

    QString keyStr;
    for(int j=0; j<3; j++)
    {
        if(modifiers & modList[j])
        {
            keyStr.append(strModList[j] + "+");
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

QStringList MusicHotKeyManager::getDefaultKeys() const
{
    QStringList keys;
    keys << "Ctrl+B" << "Ctrl+Left" << "Ctrl+Right" << "Ctrl+Up"
         << "Ctrl+Down" << "Ctrl+S" << "Ctrl+I" << "Ctrl+M";
    return keys;
}

QStringList MusicHotKeyManager::getKeys() const
{
    QStringList keys;
    foreach(QGlobalShortcut *key, m_hotkeys)
    {
        keys << key->shortcut().toString();
    }
    return keys;
}
