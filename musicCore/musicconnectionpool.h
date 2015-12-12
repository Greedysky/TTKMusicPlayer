#ifndef MUSICCONNECTIONPOOL_H
#define MUSICCONNECTIONPOOL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMap>
#include "musicsingleton.h"

#define M_CONNECTION (MusicSingleton<MusicConnectionPool>::createInstance())

class MUSIC_CORE_EXPORT MusicConnectionPool : public QObject
{
    Q_OBJECT
public:
    inline void setValue(const QString &type, QObject *object)
    {
        m_para[type] = object;
    }

    inline QObject* value(QString type) const
    {
        return m_para[type];
    }

    void setNetworkMultiValue(QObject *object);
    void removeNetworkMultiValue(QObject *object);

    void connectMusicDownload(QObject *object);
    void connect(const QString &from, const QString &to);
    void disConnect(const QString &name);

protected:
    QMap<QString, QObject*> m_para;
    QList<QObject*> m_queueList;

    MusicConnectionPool();
    ~MusicConnectionPool() = default;

    DECLARE_SINGLETON_CLASS(MusicConnectionPool)

};

#endif // MUSICCONNECTIONPOOL_H
