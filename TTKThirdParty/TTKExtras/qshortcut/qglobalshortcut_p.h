#ifndef QGLOBALSHORTCUT_P_H
#define QGLOBALSHORTCUT_P_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "qglobalshortcut.h"
#include <QAbstractEventDispatcher>
#include <QKeySequence>
#include <QHash>
#if(QT_VERSION>=0x050000)
#include <QAbstractNativeEventFilter>
#endif

/*! @brief The namespace of the qglobal shortcut private.
 * @author libqxt <foundation@libqxt.org>
 */
class MUSIC_EXTRAS_EXPORT QGlobalShortcutPrivate : public TTKPrivate<QGlobalShortcut>
#if(QT_VERSION>=0x050000)
        , public QAbstractNativeEventFilter
#endif
{
public:
    TTK_DECLARE_PUBLIC(QGlobalShortcut)
    QGlobalShortcutPrivate();
    ~QGlobalShortcutPrivate();

    bool enabled;
    Qt::Key key;
    Qt::KeyboardModifiers mods;

    bool setShortcut(const QKeySequence& shortcut);
    bool unsetShortcut();

    static bool error;
    static int ref;
#if(QT_VERSION<0x050000)
    static QAbstractEventDispatcher::EventFilter prevEventFilter;
    static bool eventFilter(void* message);
#else
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif

private:
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
    static bool registerShortcut(quint32 nativeKey, quint32 nativeMods);
    static bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);
    static void activateShortcut(quint32 nativeKey, quint32 nativeMods);

    static QHash<QPair<quint32, quint32>, QGlobalShortcut*> shortcuts;
};

#endif // QGLOBALSHORTCUT_P_H
