#include "musicglobalhotkeyedit.h"
#include "musichotkeymanager.h"

#include <QKeyEvent>

MusicGlobalHotKeyEdit::MusicGlobalHotKeyEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setReadOnly(true);
}

void MusicGlobalHotKeyEdit::setHotKeyEnabled(bool state)
{
    setEnabled(state);
    setReadOnly(!state);
}

void MusicGlobalHotKeyEdit::keyPressEvent(QKeyEvent *event)
{
    setText(G_HOTKEY_PTR->toString(event->key(), event->modifiers()));
}
