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
    explicit MusicToolMenu(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

Q_SIGNALS:
    void windowStateChanged(bool state);
    /*!
     * Window state changed.
     */

protected:
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    /*!
     * Override the widget event.
     */

};


/*! @brief The class of the tool menu base widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicToolMenuWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit MusicToolMenuWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicToolMenuWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void setTranslucentBackground();
    /*!
     * Set background translucent.
     */

public Q_SLOTS:
    void popupMenu();
    /*!
     * To popup menu.
     */

protected:
    MusicToolMenu *m_menu;
    QWidget *m_containWidget;

};

#endif // MUSICTOOLMENUWIDGET_H
