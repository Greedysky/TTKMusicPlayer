#ifndef MUSICPROGRESSWIDGET_H
#define MUSICPROGRESSWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#define WIDTH  20
#define HEIGHT 20

#include <QProgressDialog>
#include "musiclibexportglobal.h"

class QLabel;

class MUSIC_WIDGET_EXPORT MusicProgressWidget : public QProgressDialog
{
    Q_OBJECT
public:
    explicit MusicProgressWidget(QWidget *parent = 0);
    ~MusicProgressWidget();

    void setTitle(const QString &name);

public slots:
    void show();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    void initWidget();

    QLabel *m_background, *m_textLabel;
    QProgressBar *m_progressBar;

};

#endif // MUSICPROGRESSWIDGET_H
