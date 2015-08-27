#ifndef MUSICMARQUEE_H
#define MUSICMARQUEE_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicMarquee : public QWidget
{
    Q_OBJECT
public:
    explicit MusicMarquee(QWidget *parent = 0);

    void setText(const QString &newText);
    inline QString text() const { return m_myText; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    QString m_myText;
    int m_offset;
    int m_myTimerId;

};

#endif // MUSICMARQUEE_H
