#ifndef MUSICBARRAGEWIDGET_H
#define MUSICBARRAGEWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musiclibexportglobal.h"

#include <QPropertyAnimation>
#include <QLabel>
#include <QLineEdit>

class MUSIC_WIDGET_EXPORT MusicBarrageAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    explicit MusicBarrageAnimation(QObject *parent = 0);
    MusicBarrageAnimation(QObject *target, const QByteArray &propertyName, QObject *parent = 0);
    ~MusicBarrageAnimation() = default;

    void setSize(const QSize &size);

public Q_SLOTS:
    void animationFinished();

protected:
    void init();
    QSize m_parentSize;

};


class MUSIC_WIDGET_EXPORT MusicBarrageWidget : public QObject
{
    Q_OBJECT
public:
    explicit MusicBarrageWidget(QObject *parent = 0);
    ~MusicBarrageWidget();

    void start();
    void pause();
    void stop();

    void barrageStateChanged(bool on);
    void setSize(const QSize &size);

    void setLabelBackground(const QColor &color);
    void setLabelTextSize(int size);
    void addBarrage(const QString &string);

protected:
    void deleteItems();
    void createLabel();
    void createLabel(QLabel *label);
    void createAnimation();
    void createAnimation(QLabel *label);
    void setLabelBackground(QLabel *label);
    void setLabelTextSize(QLabel *label);
    void readBarrage();
    void writeBarrage();

    QWidget *m_parentClass;
    bool m_barrageState;
    QSize m_parentSize;
    int m_fontSize;
    QColor m_backgroundColor;
    QList<QLabel*> m_labels;
    QList<MusicBarrageAnimation*> m_animations;
    QStringList m_barrageLists;

};

#endif // MUSICBARRAGEWIDGET_H
