#ifndef MUSICMESSAGEBOX_H
#define MUSICMESSAGEBOX_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicMessageBox;
}

class MUSIC_WIDGET_EXPORT MusicMessageBox : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicMessageBox(QWidget *parent = 0);
    explicit MusicMessageBox(const QString &text, QWidget *parent = 0);
    MusicMessageBox(const QString &title, const QString &text,
                    QWidget *parent = 0);
    virtual ~MusicMessageBox();

    void setTitle(const QString &text) const;
    void setText(const QString &text) const;

public slots:
    void buttonClicked(int index);
    virtual int exec();

protected:
    Ui::MusicMessageBox *ui;
    int m_status;

};

#endif // MUSICMESSAGEBOX_H
