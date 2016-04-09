#ifndef MUSICCONNECTMOBILEWIDGET_H
#define MUSICCONNECTMOBILEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

class QStackedWidget;

/*! @brief The class of the connect mobile widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicConnectMobileWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicConnectMobileWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicConnectMobileWidget();

private Q_SLOTS:
    void changeStatckedWidgetFirst();
    /*!
     * Change to main widget.
     */
    void changeStatckedWidgetSecond();
    /*!
     * Change to wireless widget.
     */
    void changeStatckedWidgetThird();
    /*!
     * Change to wire widget.
     */
    void openTransferFilesWidget();
    /*!
     * Open transfer files widget.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */
    void initFirstWidget();
    /*!
     * Init main widget.
     */
    void initSecondWidget();
    /*!
     * Init connect to wireless widget.
     */
    void initThirdWidget();
    /*!
     * Init connect to wire widget.
     */

    QStackedWidget *m_stackedWidget;

};

#endif // MUSICCONNECTMOBILEWIDGET_H
