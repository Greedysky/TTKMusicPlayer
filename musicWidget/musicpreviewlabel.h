#ifndef MUSICPREVIEWLABEL_H
#define MUSICPREVIEWLABEL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musiclibexportglobal.h"

class MUSIC_WIDGET_EXPORT MusicPreviewLabel : public QLabel
{
public:
    explicit MusicPreviewLabel(QWidget *parent = 0);

    void setParameter(const QStringList &para);
    void setLinearGradient(QColor &fg, QColor &bg);
    void setTransparent(int trans) { m_transparent = trans;}

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    QFont m_font;
    QLinearGradient m_linearGradient;
    QLinearGradient m_maskLinearGradient;
    int m_transparent;

};

#endif // MUSICPREVIEWLABEL_H
