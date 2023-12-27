#ifndef QGLOBALSHORTCUT_P_H
#define QGLOBALSHORTCUT_P_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
 ***************************************************************************/

#include "qglobalshortcut.h"
#include <QHash>
#include <QAbstractEventDispatcher>
#if TTK_QT_VERSION_CHECK(5,0,0)
#  include <QAbstractNativeEventFilter>
#endif

/*! @brief The class of the qglobal shortcut private.
 * @author libqxt <foundation@libqxt.org>
 */
class TTK_MODULE_EXPORT QGlobalShortcutPrivate : public TTKPrivate<QGlobalShortcut>
#if TTK_QT_VERSION_CHECK(5,0,0)
        , public QAbstractNativeEventFilter
#endif
{
public:
    TTK_DECLARE_PUBLIC(QGlobalShortcut)
    QGlobalShortcutPrivate();
    ~QGlobalShortcutPrivate();

    bool m_enabled;
    Qt::Key m_key;
    Qt::KeyboardModifiers m_mods;

    bool setShortcut(const QKeySequence &shortcut);
    bool unsetShortcut();

    static bool m_error;
    static int m_ref;
#if !TTK_QT_VERSION_CHECK(5,0,0)
    static QAbstractEventDispatcher::EventFilter m_prevEventFilter;
    static bool eventFilter(void* message);
#else
    virtual bool nativeEventFilter(const QByteArray &type, void *message, qint *result) override final;
#endif

private:
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
    static bool registerShortcut(quint32 nativeKey, quint32 nativeMods);
    static bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);
    static void activateShortcut(quint32 nativeKey, quint32 nativeMods);

    static QHash<QPair<quint32, quint32>, QGlobalShortcut*> m_shortcuts;
};

#endif // QGLOBALSHORTCUT_P_H
