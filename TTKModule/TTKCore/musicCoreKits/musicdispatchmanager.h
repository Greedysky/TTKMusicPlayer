#ifndef MUSICDISPATCHMANAGER_H
#define MUSICDISPATCHMANAGER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMutex>
#include "musicsingleton.h"

#define M_DISPATCH_PTR (MusicSingleton<MusicDispatchManager>::createInstance())

class QTimer;

/*! @brief The class of the functions dispatch item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicDispatchItem
{
    int m_type;
    int m_times;
    QList<QVariant> m_args;
    MusicDispatchItem()
    {
        m_times = 0;
        m_type = -1;
    }

    bool isEmpty() const
    {
        return m_args.isEmpty();
    }

    bool isValid() const
    {
        return m_times <= 5;
    }

}MusicDispatchItem;


/*! @brief The class of the functions dispatch manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicDispatchManager : public QObject
{
    Q_OBJECT
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */

    void dispatch(int type);
    void dispatch(int type, const QVariant &arg1);
    void dispatch(int type, const QVariant &arg1, const QVariant &arg2);
    void dispatch(int type, const QVariant &arg1, const QVariant &arg2,
                  const QVariant &arg3);
    void dispatch(int type, const QVariant &arg1, const QVariant &arg2,
                  const QVariant &arg3, const QVariant &arg4);
    void dispatch(int type, const QList<QVariant> &args);
    void dispatch(int type, void *funcs);
    /*!
     * Set dispatch.
     */

private Q_SLOTS:
    void activeFunctions();
    /*!
     * Active functions.
     */

protected:
    MusicDispatchManager();
    /*!
     * Object contsructor.
     */
    ~MusicDispatchManager();

    QMutex m_mutex;
    QTimer *m_timer;
    QList<MusicDispatchItem*> m_observer;

    DECLARE_SINGLETON_CLASS(MusicDispatchManager)

};

#endif // MUSICDISPATCHMANAGER_H
