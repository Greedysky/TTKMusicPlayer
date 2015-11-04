#ifndef MUSICLRCSEARCHWIDGET_H
#define MUSICLRCSEARCHWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcSearchWidget;
}

class MUSIC_LRC_EXPORT MusicLrcSearchWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcSearchWidget(QWidget *parent = 0);
    virtual ~MusicLrcSearchWidget();

public slots:
    void lrcSearchButtonClicked() const;
    void lrcSearchDownloadClicked();
    void lrcDownloadStateChanged(const QString &string);
    void setCurrentSongName(const QString &name) const;
    virtual int exec();

protected:
    Ui::MusicLrcSearchWidget *ui;

};

#endif // MUSICLRCSEARCHWIDGET_H
