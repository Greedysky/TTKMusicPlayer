#ifndef MUSICUSERRECORDWIDGET_H
#define MUSICUSERRECORDWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicUserRecordWidget;
}

class MUSIC_USER_EXPORT MusicUserRecordWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicUserRecordWidget(QWidget *parent = 0);
    ~MusicUserRecordWidget();

signals:

public slots:
    virtual int exec();

protected:
    Ui::MusicUserRecordWidget *ui;

};

#endif // MUSICUSERRECORDWIDGET_H
