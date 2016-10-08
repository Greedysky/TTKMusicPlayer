#ifndef MUSICNETWORKOPERATOR_H
#define MUSICNETWORKOPERATOR_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicglobaldefine.h"

/*! @brief The class of music network operator.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicNetworkOperator : public QObject
{
    Q_OBJECT
public:
    explicit MusicNetworkOperator(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicNetworkOperator();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void startToOperator();
    /*!
     * Start to get current ip operator from net.
     */

Q_SIGNALS:
    void getNetworkOperatorFinished(const QString &name);
    /*!
     * Get network operator is finished.
     */

public Q_SLOTS:
    void downLoadFinished(const QByteArray &data);
    /*!
     * Download data from ip net finished.
     */

};

#endif // MUSICNETWORKOPERATOR_H
