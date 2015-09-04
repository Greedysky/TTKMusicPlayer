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
#include "musicsingletone.h"

#define M_Connection (MusicSingleton<MusicConnectionPool>::createInstance())

class MUSIC_CORE_EXPORT MusicConnectionPool : public QObject
{
    Q_OBJECT
public:
    explicit MusicConnectionPool(QObject *parent = 0);

    inline void setValue(const QString &type, QObject *object)
    {
        m_para[type] = object;
    }

    inline QObject* value(QString type) const
    {
        return m_para[type];
    }

    inline void setMultiValue(const QString &type, QObject *object)
    {
        QList<QObject*> l = m_multiPara[type];
        l.append(object);
        m_multiPara[type] = l;
    }

    inline QList<QObject*>  multiValue(QString type) const
    {
        return m_multiPara[type];
    }

    void connect(const QString &from, const QString &to);
    void disConnect(const QString &name);

    void multiConnect(const QString &from, const QString &to);
    void disMultiConnect(const QString &name, QObject *object);

protected:
    QMap<QString, QObject*> m_para;
    QMap<QString, QList<QObject*> > m_multiPara;

    DECLARE_SINGLETON_CLASS(MusicConnectionPool)

};

#endif // MUSICCONNECTIONPOOL_H
