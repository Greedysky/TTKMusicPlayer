#ifndef MUSICCONNECTIONPOOL_H
#define MUSICCONNECTIONPOOL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMap>
#include <QObject>
#include "musicsingleton.h"

#define M_CONNECTION_PTR (MusicSingleton<MusicConnectionPool>::createInstance())

/*! @brief The class of the qt signal and slot connection pool.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicConnectionPool : public QObject
{
    Q_OBJECT
public:
    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set connection object and type name.
     */
    inline void setValue(const QString &type, QObject *object)
    {
        m_para[type] = object;
    }
    /*!
     * Get connection object by type name.
     */
    inline QObject* value(const QString &type) const
    {
        return m_para[type];
    }
    /*!
     * Get connection object by type name in operator[].
     */
    inline const QObject* operator[](const QString &type) const
    {
        return value(type);
    }
    /*!
     * Set connection object and type name in operator[].
     */
    inline QObject* &operator[](const QString &type)
    {
        return m_para[type];
    }

    /*!
     * Set mutiple network connection object.
     */
    void setNetworkMultiValue(QObject *object);
    /*!
     * Remove mutiple network connection object.
     */
    void removeNetworkMultiValue(QObject *object);
    /*!
     * Set music data network connection object.
     */
    void connectMusicDownload(QObject *object);

    /*!
     * Set connection by two object.
     */
    void poolConnect(const QObject *from, const QObject *to);
    /*!
     * Set connection by two object type name.
     */
    void poolConnect(const QString &from, const QString &to);
    /*!
     * Remove connection object by object.
     */
    void removeValue(const QObject *object);
    /*!
     * Remove connection object by object name.
     */
    void removeValue(const QString &name);

protected:
    QMap<QString, QObject*> m_para;
    QList<QObject*> m_queueList;

    /*!
     * Object contsructor.
     */
    MusicConnectionPool();

    DECLARE_SINGLETON_CLASS(MusicConnectionPool)

};

#endif // MUSICCONNECTIONPOOL_H
