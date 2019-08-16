#ifndef MUSICHOTKEYMANAGER_H
#define MUSICHOTKEYMANAGER_H

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

#include "musicsingleton.h"

#define M_HOTKEY_PTR (MusicSingleton<MusicHotKeyManager>::createInstance())

class QGlobalShortcut;

/*! @brief The class of the global hotkey setting manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicHotKeyManager : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicHotKeyManager)
public:
    /*!
     * To connect parent slot object.
     */
    void connectParentObject(QObject *object);

    /*!
     * Set hotKey by given string list keys.
     */
    void setHotKeys(const QStringList &keys);
    /*!
     * Set hotKey by given index and string key.
     */
    void setHotKey(int index, const QString &key);
    /*!
     * Set hotKey by given index and virtual key.
     */
    void setHotKey(int index, int key);
    /*!
     * Get the string key by given hotKey index.
     */
    QString hotKey(int index);

    /*!
     * Get the object key by given hotKey index.
     */
    QObject* getHotKey(int index);
    /*!
     * Add hotKey by given index and string key.
     */
    void addHotKey(const QString &key);
    /*!
     * Set hotKey by given index and virtual key.
     */
    void addHotKey(int key);

    /*!
     * Enable or disable the hotkey by index.
     */
    void setEnabled(int index, bool enabled);
    /*!
     * check the given hotkey is enabled or not.
     */
    bool enabled(int index);
    /*!
     * Enable or disable all hotkeys.
     */
    void enabledAll(bool enabled);

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
    QStringList getDefaultKeys() const;
    /*!
     * Get key string.
     */
    QStringList getKeys() const;

protected:
    /*!
     * Set default key.
     */
    void setDefaultKey();

    QList<QGlobalShortcut*> m_hotkeys;

    DECLARE_SINGLETON_CLASS(MusicHotKeyManager)
};

#endif // MUSICHOTKEYMANAGER_H
