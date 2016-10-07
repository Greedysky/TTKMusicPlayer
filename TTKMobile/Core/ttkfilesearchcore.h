#ifndef TTKFILESEARCHCORE_H
#define TTKFILESEARCHCORE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QThread>
#include <QFileInfoList>
#include "musicmobileglobaldefine.h"

/*! @brief The class of the file search core.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKFileSearchCore : public QThread
{
    Q_OBJECT
public:
    explicit TTKFileSearchCore(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKFileSearchCore();

    Q_INVOKABLE void search(const QStringList &path);
    /*!
     * Search local item lists.
     */
    Q_INVOKABLE void search(const QString &path);
    /*!
     * Search local item.
     */

Q_SIGNALS:
    void finished(const QStringList &path);
    /*!
     * Search local item finished.
     */
    void findFilePath(const QString &path);
    /*!
     * Show current found path immediately.
     */

public Q_SLOTS:
    void start();
    /*!
     * Strat thread now.
     */
    void run();
    /*!
     * Thread run now.
     */

private:
    void stopAndQuitThread();
    /*!
     * Stop and quit current thread.
     */
    QFileInfoList findFile(const QString &path, const QStringList &filter);
    /*!
     * To find file by dir path.
     */

    bool m_run;
    QStringList m_path;

};

#endif // TTKFILESEARCHCORE_H
