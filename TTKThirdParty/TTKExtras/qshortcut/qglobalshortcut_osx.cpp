#include <Carbon/Carbon.h>
#include "qglobalshortcut_p.h"
#include <QHash>
#include <QApplication>

typedef QPair<quint32, quint32> Identifier;
static QHash<Identifier, EventHotKeyRef> keyRefs;
static bool q_mac_handler_installed = false;

OSStatus q_mac_handle_hot_key(EventHandlerCallRef nextHandler, EventRef event, void *data)
{
    // pass event to the app event filter
    Q_UNUSED(nextHandler);
    Q_UNUSED(data);
#if !TTK_QT_VERSION_CHECK(5,0,0)
    QAbstractEventDispatcher::instance()->filterEvent((void*)event);
#else
    QAbstractEventDispatcher::instance()->filterNativeEvent({}, (void*)event, nullptr);
#endif
    return noErr;
}

#if !TTK_QT_VERSION_CHECK(5,0,0)
bool QGlobalShortcutPrivate::eventFilter(void *message)
#else
bool QGlobalShortcutPrivate::nativeEventFilter(const QByteArray &, void *message, qint *)
#endif
{
    EventRef event = (EventRef) message;
    if(GetEventClass(event) == kEventClassKeyboard && GetEventKind(event) == kEventHotKeyPressed)
    {
        EventHotKeyID keyID;
        GetEventParameter(event, kEventParamDirectObject, typeEventHotKeyID, nullptr, sizeof(keyID), nullptr, &keyID);
        activateShortcut(keyID.signature, keyID.id);
    }
    return false;
}

quint32 QGlobalShortcutPrivate::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    quint32 native = 0;
    if(modifiers & Qt::ShiftModifier)
        native |= shiftKey;
    if(modifiers & Qt::ControlModifier)
        native |= cmdKey;
    if(modifiers & Qt::AltModifier)
        native |= optionKey;
    if(modifiers & Qt::MetaModifier)
        native |= controlKey;
    if(modifiers & Qt::KeypadModifier)
        native |= kEventKeyModifierNumLockMask;
    return native;
}

quint32 QGlobalShortcutPrivate::nativeKeycode(Qt::Key key)
{
    // Constants found in NSEvent.h from AppKit.framework
    if(key == Qt::Key_Return)          return kVK_Return;
    else if(key == Qt::Key_Enter)      return kVK_ANSI_KeypadEnter;
    else if(key == Qt::Key_Tab)        return kVK_Tab;
    else if(key == Qt::Key_Space)      return kVK_Space;
    else if(key == Qt::Key_Backspace)  return kVK_Delete;
    else if(key == Qt::Key_Escape)     return kVK_Escape;
    else if(key == Qt::Key_CapsLock)   return kVK_CapsLock;
    else if(key == Qt::Key_Option)     return kVK_Option;
    else if(key == Qt::Key_F17)        return kVK_F17;
    else if(key == Qt::Key_VolumeUp)   return kVK_VolumeUp;
    else if(key == Qt::Key_VolumeDown) return kVK_VolumeDown;
    else if(key == Qt::Key_F18)        return kVK_F18;
    else if(key == Qt::Key_F19)        return kVK_F19;
    else if(key == Qt::Key_F20)        return kVK_F20;
    else if(key == Qt::Key_F5)         return kVK_F5;
    else if(key == Qt::Key_F6)         return kVK_F6;
    else if(key == Qt::Key_F7)         return kVK_F7;
    else if(key == Qt::Key_F3)         return kVK_F3;
    else if(key == Qt::Key_F8)         return kVK_F8;
    else if(key == Qt::Key_F9)         return kVK_F9;
    else if(key == Qt::Key_F11)        return kVK_F11;
    else if(key == Qt::Key_F13)        return kVK_F13;
    else if(key == Qt::Key_F16)        return kVK_F16;
    else if(key == Qt::Key_F14)        return kVK_F14;
    else if(key == Qt::Key_F10)        return kVK_F10;
    else if(key == Qt::Key_F12)        return kVK_F12;
    else if(key == Qt::Key_F15)        return kVK_F15;
    else if(key == Qt::Key_Help)       return kVK_Help;
    else if(key == Qt::Key_Home)       return kVK_Home;
    else if(key == Qt::Key_PageUp)     return kVK_PageUp;
    else if(key == Qt::Key_Delete)     return kVK_ForwardDelete;
    else if(key == Qt::Key_F4)         return kVK_F4;
    else if(key == Qt::Key_End)        return kVK_End;
    else if(key == Qt::Key_F2)         return kVK_F2;
    else if(key == Qt::Key_PageDown)   return kVK_PageDown;
    else if(key == Qt::Key_F1)         return kVK_F1;
    else if(key == Qt::Key_Left)       return kVK_LeftArrow;
    else if(key == Qt::Key_Right)      return kVK_RightArrow;
    else if(key == Qt::Key_Down)       return kVK_DownArrow;
    else if(key == Qt::Key_Up)         return kVK_UpArrow;

    CFDataRef currentLayoutData;
    TISInputSourceRef currentKeyboard = TISCopyCurrentASCIICapableKeyboardLayoutInputSource();

    if(currentKeyboard == nullptr)
        return 0;

    currentLayoutData = (CFDataRef)TISGetInputSourceProperty(currentKeyboard, kTISPropertyUnicodeKeyLayoutData);
    CFRelease(currentKeyboard);

    if(currentLayoutData == nullptr)
        return 0;

    UCKeyboardLayout *header = (UCKeyboardLayout*)CFDataGetBytePtr(currentLayoutData);
    UCKeyboardTypeHeader *table = header->keyboardTypeList;

    UTF16Char ch = key;
    uint8_t *data = (uint8_t*)header;
    for(quint32 i=0; i < header->keyboardTypeCount; i++)
    {
        UCKeyStateRecordsIndex *stateRec = 0;
        if(table[i].keyStateRecordsIndexOffset != 0)
        {
            stateRec = TTKReinterpretCast(UCKeyStateRecordsIndex*, data + table[i].keyStateRecordsIndexOffset);
            if(stateRec->keyStateRecordsIndexFormat != kUCKeyStateRecordsIndexFormat) stateRec = 0;
        }

        UCKeyToCharTableIndex *charTable = TTKReinterpretCast(UCKeyToCharTableIndex*, data + table[i].keyToCharTableIndexOffset);
        if(charTable->keyToCharTableIndexFormat != kUCKeyToCharTableIndexFormat)
        {
            continue;
        }

        for(quint32 j=0; j < charTable->keyToCharTableCount; j++)
        {
            UCKeyOutput *keyToChar = TTKReinterpretCast(UCKeyOutput*, data + charTable->keyToCharTableOffsets[j]);
            for(quint32 k=0; k < charTable->keyToCharTableSize; k++)
            {
                if(keyToChar[k] & kUCKeyOutputTestForIndexMask)
                {
                    long idx = keyToChar[k] & kUCKeyOutputGetIndexMask;
                    if(stateRec && idx < stateRec->keyStateRecordCount)
                    {
                        UCKeyStateRecord *rec = TTKReinterpretCast(UCKeyStateRecord*, data + stateRec->keyStateRecordOffsets[idx]);
                        if(rec->stateZeroCharData == ch)
                        {
                            return k;
                        }
                    }
                }
                else if(!(keyToChar[k] & kUCKeyOutputSequenceIndexMask) && keyToChar[k] < 0xFFFE)
                {
                    if(keyToChar[k] == ch)
                    {
                        return k;
                    }
                }
            } // for k
        } // for j
    } // for i

    return 0;
}

bool QGlobalShortcutPrivate::registerShortcut(quint32 nativeKey, quint32 nativeMods)
{
    if(!q_mac_handler_installed)
    {
        EventTypeSpec t;
        t.eventClass = kEventClassKeyboard;
        t.eventKind = kEventHotKeyPressed;
        InstallApplicationEventHandler(&q_mac_handle_hot_key, 1, &t, nullptr, nullptr);
    }

    EventHotKeyID keyID;
    keyID.signature = nativeKey;
    keyID.id = nativeMods;

    EventHotKeyRef ref = 0;
    const bool rv = !RegisterEventHotKey(nativeKey, nativeMods, keyID, GetApplicationEventTarget(), 0, &ref);
    if(rv)
    {
        keyRefs.insert(Identifier(nativeMods, nativeKey), ref);
    }

    return rv;
}

bool QGlobalShortcutPrivate::unregisterShortcut(quint32 nativeKey, quint32 nativeMods)
{
    Identifier id(nativeMods, nativeKey);
    if(!keyRefs.contains(id))
        return false;

    return !UnregisterEventHotKey(keyRefs.take(id));
}
