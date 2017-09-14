#ifndef MUSICCONNECTMOBILEWIDGET_H
#define MUSICCONNECTMOBILEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class MusicAnimationStackedWidget;

/*! @brief The class of the connect mobile widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectMobileWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicConnectMobileWidget(QWidget *parent = 0);

    ~MusicConnectMobileWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

private Q_SLOTS:
    /*!
     * Change to main widget.
     */
    void changeStatckedWidgetFirst();
    /*!
     * Change to wireless widget.
     */
    void changeStatckedWidgetSecond();
    /*!
     * Change to wire widget.
     */
    void changeStatckedWidgetThird();
    /*!
     * Open transfer files to mobile widget.
     */
    void openTransferFiles2Mobile();
    /*!
     * Open transfer files to wifi widget.
     */
    void openTransferFiles2Wifi();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Init main widget.
     */
    void initFirstWidget();
    /*!
     * Init connect to wireless widget.
     */
    void initSecondWidget();
    /*!
     * Init connect to wire widget.
     */
    void initThirdWidget();

    MusicAnimationStackedWidget *m_stackedWidget;

};

#endif // MUSICCONNECTMOBILEWIDGET_H
