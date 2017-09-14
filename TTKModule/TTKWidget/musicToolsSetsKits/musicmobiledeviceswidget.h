#ifndef MUSICMOBILEDEVICESWIDGET_H
#define MUSICMOBILEDEVICESWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QLabel>
#include "musicobject.h"
#include "musicglobaldefine.h"

class QToolButton;

/*! @brief The class of the mobile devices widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicMobileDevicesWidget : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicMobileDevicesWidget(QWidget *parent = 0);

    ~MusicMobileDevicesWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

public Q_SLOTS:
    /*!
     * Show mobile manager widget.
     */
    void showMobileManager();

protected:
    QToolButton *m_closeButton;
    QToolButton *m_openButton;

};

#endif // MUSICMOBILEDEVICESWIDGET_H
