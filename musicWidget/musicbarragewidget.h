#ifndef MUSICBARRAGEWIDGET_H
#define MUSICBARRAGEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclibexportglobal.h"
#include <QPropertyAnimation>
#include <QLabel>

#define NUMBER 2

class MusicBarrageAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    explicit MusicBarrageAnimation(QObject *parent = 0);
    MusicBarrageAnimation(QObject *target, const QByteArray &propertyName, QObject *parent = 0);
    ~MusicBarrageAnimation() = default;

public slots:
    void animationFinished();

protected:
    void init();

};


class MusicBarrageWidget : public QObject
{
    Q_OBJECT
public:
    explicit MusicBarrageWidget(QObject *parent = 0);
    ~MusicBarrageWidget();

    void start();
    void pause();
    void stop();

    void setSize(const QSize &size);

signals:

public slots:

protected:
    void createLabel();
    void createAnimation();

    QWidget *m_parentClass;
    QSize m_parentSize;
    QList<QLabel*> m_labels;
    QList<MusicBarrageAnimation*> m_animations;

};

#endif // MUSICBARRAGEWIDGET_H
