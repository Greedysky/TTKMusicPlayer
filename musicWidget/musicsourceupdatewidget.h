#ifndef MUSICSOURCEUPDATEWIDGET_H
#define MUSICSOURCEUPDATEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicSourceUpdateWidget;
}

class MUSIC_WIDGET_EXPORT MusicSourceUpdateWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSourceUpdateWidget(QWidget *parent = 0);
    virtual ~MusicSourceUpdateWidget();

signals:

public slots:
    virtual int exec();
    void upgradeButtonClicked();

protected:
    Ui::MusicSourceUpdateWidget *ui;

};

#endif // MUSICSOURCEUPDATEWIDGET_H
