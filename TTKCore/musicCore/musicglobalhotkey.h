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

#define M_HOTKEY (MusicSingleton<MusicGlobalHotKey>::createInstance())

class QxtGlobalShortcut;

/*! @brief The class of the global hotkey setting manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicGlobalHotKey : public QObject
{
    Q_OBJECT
public:
    void connectParentObject(QObject *object);

    void setHotKey(int index, const QString &key);
    void setHotKey(int index, int key);
    QString hotKey(int index);

    void setEnabled(int index, bool enabled);
    bool enabled(int index);
    void enabledAll(bool enabled);

    QString toString(int key, int modifiers);

protected:
    MusicGlobalHotKey();
    ~MusicGlobalHotKey();

    QList<QxtGlobalShortcut*> m_hotkeys;

    DECLARE_SINGLETON_CLASS(MusicGlobalHotKey)
};

#endif // MUSICGLOBALHOTKEY_H
