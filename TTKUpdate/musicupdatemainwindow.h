#ifndef MUSICUPDATEMAINWINDOW_H
#define MUSICUPDATEMAINWINDOW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovewidget.h"

class MusicProgressWidget;

/*! @brief The class of the app update main window.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_UPDATE_EXPORT MusicUpdateMainWindow : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    explicit MusicUpdateMainWindow(QWidget *parent = 0);
    virtual ~MusicUpdateMainWindow();

    void setFilePath(const QString &in, const QString &out);

private Q_SLOTS:
    void process(float percent, const QString &file);

protected:
    MusicProgressWidget *m_progressBar;

};

#endif // MUSICUPDATEMAINWINDOW_H
