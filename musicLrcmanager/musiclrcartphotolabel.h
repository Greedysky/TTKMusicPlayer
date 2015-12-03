#ifndef MUSICLRCARTPHOTOLABEL_H
#define MUSICLRCARTPHOTOLABEL_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musiclibexportglobal.h"

#define PIX_HEIGHT 16
#define PIX_WIDTH  16

class MUSIC_LRC_EXPORT MusicLrcArtPhotoLabel : public QWidget
{
    Q_OBJECT
public:
    explicit MusicLrcArtPhotoLabel(QWidget *parent = 0);

    void setImagePath(const QString &path);
    void saveImagePath(const QString &path) const;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    QString m_path;
    QPixmap m_showPix;
    int m_width, m_height;

};

#endif // MUSICLRCARTPHOTOLABEL_H
