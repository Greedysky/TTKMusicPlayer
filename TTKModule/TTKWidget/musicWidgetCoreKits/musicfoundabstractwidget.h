#ifndef MUSICFOUNDABSTRACTWIDGET_H
#define MUSICFOUNDABSTRACTWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicglobaldefine.h"

/*! @brief The class of the found query widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFoundAbstractWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFoundAbstractWidget(QWidget *parent = 0);

    ~MusicFoundAbstractWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name);

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() = 0;

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QLabel *m_statusLabel;
    QList<QLabel*> m_resizeWidget;

};

#endif // MUSICFOUNDABSTRACTWIDGET_H
