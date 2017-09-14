#ifndef MUSICTOOLMENUWIDGET_H
#define MUSICTOOLMENUWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMenu>
#include <QToolButton>
#include "musicglobaldefine.h"

/*! @brief The class of the tool menu.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicToolMenu : public QMenu
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicToolMenu(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

Q_SIGNALS:
    /*!
     * Window state changed.
     */
    void windowStateChanged(bool state);

protected:
    /*!
     * Override the widget event.
     */
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

};


/*! @brief The class of the tool menu base widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicToolMenuWidget : public QToolButton
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicToolMenuWidget(QWidget *parent = 0);

    ~MusicToolMenuWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set background translucent.
     */
    void setTranslucentBackground();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    void popupMenu();

protected:
    MusicToolMenu *m_menu;
    QWidget *m_containWidget;

};

#endif // MUSICTOOLMENUWIDGET_H
