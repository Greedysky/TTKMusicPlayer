#ifndef MUSICABSTRACTMOVEDIALOG_H
#define MUSICABSTRACTMOVEDIALOG_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include <QDialog>
#include "musicglobaldefine.h"

/*! @brief The class of the moving dialog base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAbstractMoveDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MusicAbstractMoveDialog(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicAbstractMoveDialog();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    inline void drawWindowShadow(bool show) { m_showShadow = show;}
    /*!
     * Set or not draw window bound shadow.
     */
    inline void blockMoveOption(bool block) { m_moveOption = block;}
    /*!
     * Set or not block widget moving.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    /*!
     * Override the widget event.
     */
    void setBackgroundPixmap(QLabel *label, const QSize &size);
    /*!
     * Set background pixmap.
     */

    QPoint m_pressAt;
    bool m_moveOption, m_showShadow;
    bool m_leftButtonPress;

};

#endif // MUSICABSTRACTMOVEDIALOG_H
