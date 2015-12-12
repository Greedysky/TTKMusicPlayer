#ifndef MUSICSONGSTOOLITEMRENAMEDWIDGET_H
#define MUSICSONGSTOOLITEMRENAMEDWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLineEdit>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicSongsToolItemRenamedWidget : public QLineEdit
{
    Q_OBJECT
public:
    MusicSongsToolItemRenamedWidget(int offset, const QString &originText,
                                    QWidget *parent = 0);

protected:
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

signals:
    void renameFinished(const QString &text);

public slots:
    void renameFinished()
    {
        emit renameFinished(text());
    }

};

#endif // MUSICSONGSTOOLITEMRENAMEDWIDGET_H
