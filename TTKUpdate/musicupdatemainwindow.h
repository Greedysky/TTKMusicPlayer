#ifndef MUSICUPDATEMAINWINDOW_H
#define MUSICUPDATEMAINWINDOW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicprogresswidget.h"

class MusicUpdateUnzipThread;

/*! @brief The class of the app update main window.
 * @author Greedysky <greedysky@163.com>
 */
class /*MUSIC_UPDATE_EXPORT*/ MusicUpdateMainWindow : public MusicProgressWidget
{
    Q_OBJECT
public:
    explicit MusicUpdateMainWindow(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicUpdateMainWindow();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setFilePath(const QString &in, const QString &out);
    /*!
     * Set input and output file path.
     */

private Q_SLOTS:
    void process(float percent, const QString &file);
    /*!
     * Show current unzip progress and unzip name.
     */
    void finished();
    /*!
     * Unzip given file finished.
     */

protected:
    MusicUpdateUnzipThread *m_thread;

};

#endif // MUSICUPDATEMAINWINDOW_H
