#ifndef MUSICHOTKEYMANAGER_H
#define MUSICHOTKEYMANAGER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "ttksingleton.h"

class QGlobalShortcut;

/*! @brief The class of the global hotkey setting manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicHotKeyManager : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicHotKeyManager)
public:
    /*!
     * Add hotKey by given object.
     */
    void addHotKey(QObject *object);
    /*!
     * Add hotKey by given object and slot.
     */
    void addHotKey(QObject *object, const char *slot);
    /*!
     * Add hotKey by given object key and slot.
     */
    void addHotKey(QObject *object, const QString &key, const char *slot);

    /*!
     * Set hotKey by given index and string key.
     */
    void setHotKey(int index, const QString &key);
    /*!
     * Get the object key by given hotKey index.
     */
    QObject* hotKey(int index);

    /*!
     * Unset registered hotKeys.
     */
    void unsetShortcut();

    /*!
     * Enable or disable all hotkeys.
     */
    void setEnabled(bool enabled);
    /*!
     * Enable or disable the hotkey by index.
     */
    void setEnabled(int index, bool enabled);
    /*!
     * check the given hotkey is enabled or not.
     */
    bool isEnabled(int index);

    /*!
     * Mapping the virtual key to string key.
     */
    QString toString(int key, int modifiers);
    /*!
     * Get hotkey count.
     */
    int count() const;

    /*!
     * Get default key string.
     */
    QStringList defaultKeys() const;
    /*!
     * Get key string.
     */
    QStringList keys() const;

private:
    QList<QGlobalShortcut*> m_hotkeys;

    TTK_DECLARE_SINGLETON_CLASS(MusicHotKeyManager)

};

#define G_HOTKEY_PTR makeMusicHotKeyManager()
TTK_MODULE_EXPORT MusicHotKeyManager* makeMusicHotKeyManager();

#endif // MUSICHOTKEYMANAGER_H
