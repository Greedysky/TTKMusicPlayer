#ifndef MUSICTOASTLABEL_H
#define MUSICTOASTLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QTimer>
#include "musicglobaldefine.h"

class MUSIC_LRC_EXPORT MusicToastLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MusicToastLabel(QWidget *parent = 0);
    explicit MusicToastLabel(const QString &text, QWidget *parent = 0);
    ~MusicToastLabel();

    void setTimerInterval(int msecond);
    int getTimerInterval() const;

    void setFontSize(int size);
    int getFontSize() const;

    void setBold(bool bold);
    bool bold() const;

public Q_SLOTS:
    void setText(const QString &text);

private Q_SLOTS:
    void closeAnimation();

protected:
    QTimer m_timer;
    QFont m_font;

};

#endif // MUSICTOASTLABEL_H
