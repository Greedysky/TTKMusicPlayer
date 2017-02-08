#include "musicglobalhotkey.h"
#include "qxtglobalshortcut.h"

#include <QStringList>

QString MusicGlobalHotKey::getClassName()
{
    return staticMetaObject.className();
}

void MusicGlobalHotKey::connectParentObject(QObject *object)
{
    for(int i=0; i<7; ++i)
    {
        m_hotkeys << (new QxtGlobalShortcut(object));
    }

    connect(m_hotkeys[0], SIGNAL(activated()), object, SLOT(musicStatePlay()));
    connect(m_hotkeys[1], SIGNAL(activated()), object, SLOT(musicPlayPrevious()));
    connect(m_hotkeys[2], SIGNAL(activated()), object, SLOT(musicPlayNext()));
    connect(m_hotkeys[3], SIGNAL(activated()), object, SLOT(musicActionVolumePlus()));
    connect(m_hotkeys[4], SIGNAL(activated()), object, SLOT(musicActionVolumeSub()));
    connect(m_hotkeys[5], SIGNAL(activated()), object, SLOT(musicSetting()));
    connect(m_hotkeys[6], SIGNAL(activated()), object, SLOT(musicImportSongs()));

    setDefaultKey();
}

void MusicGlobalHotKey::setDefaultKey()
{
    QStringList keys;
    keys << "Ctrl+B" << "Ctrl+Left" << "Ctrl+Right" << "Ctrl+Up"
         << "Ctrl+Down" << "Ctrl+S" << "Ctrl+I";
    for(int i=0; i<m_hotkeys.count(); ++i)
    {
        setHotKey(i, keys[i]);
        setEnabled(i, false);
    }
}

void MusicGlobalHotKey::setHotKey(int index, const QString &key)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }
    m_hotkeys[index]->setShortcut(QKeySequence(key));
}

void MusicGlobalHotKey::setHotKey(int index, int key)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }
    m_hotkeys[index]->setShortcut(QKeySequence(key));
}

QString MusicGlobalHotKey::hotKey(int index)
{
    if(index >= m_hotkeys.count())
    {
        return QString();
    }
    return m_hotkeys[index]->shortcut().toString();
}

void MusicGlobalHotKey::setEnabled(int index, bool enabled)
{
    if(index >= m_hotkeys.count())
    {
        return;
    }
    m_hotkeys[index]->setEnabled(enabled);
}

bool MusicGlobalHotKey::enabled(int index)
{
    if(index >= m_hotkeys.count())
    {
        return false;
    }
    return m_hotkeys[index]->isEnabled();
}

void MusicGlobalHotKey::enabledAll(bool enabled)
{
    foreach(QxtGlobalShortcut *key, m_hotkeys)
    {
        key->setEnabled(enabled);
    }
}

QString MusicGlobalHotKey::toString(int key, int modifiers)
{
    QString strModList[] = { "Ctrl", "Shift", "Alt"};
    quint32 modList[] = { Qt::ControlModifier, Qt::ShiftModifier, Qt::AltModifier};

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
