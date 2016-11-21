#ifndef MUSICGLOBALHOTKEY_H
#define MUSICGLOBALHOTKEY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingleton.h"

#define M_HOTKEY_PTR (MusicSingleton<MusicGlobalHotKey>::createInstance())

class QxtGlobalShortcut;

/*! @brief The class of the global hotkey setting manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicGlobalHotKey : public QObject
{
    Q_OBJECT
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */
    void connectParentObject(QObject *object);
    /*!
     * To connect parent slot object.
     */

    void setHotKey(int index, const QString &key);
    /*!
     * Set hotKey by given index and string key.
     */
    void setHotKey(int index, int key);
    /*!
     * Set hotKey by given index and virtual key.
     */
    QString hotKey(int index);
    /*!
     * Get the string key by given hotKey index.
     */

    void setEnabled(int index, bool enabled);
    /*!
     * Enable or disable the hotkey by index.
     */
    bool enabled(int index);
    /*!
     * check the given hotkey is enabled or not.
     */
    void enabledAll(bool enabled);
    /*!
     * Enable or disable all hotkeys.
     */

    QString toString(int key, int modifiers);
    /*!
     * Mapping the virtual key to string key.
     */

protected:
    void setDefaultKey();
    /*!
     * Set default key.
     */

    QList<QxtGlobalShortcut*> m_hotkeys;

    DECLARE_SINGLETON_CLASS(MusicGlobalHotKey)
};

#endif // MUSICGLOBALHOTKEY_H
