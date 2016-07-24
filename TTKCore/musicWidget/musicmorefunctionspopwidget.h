#ifndef MUSICMOREFUNCTIONSPOPWIDGET_H
#define MUSICMOREFUNCTIONSPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

/*! @brief The class of the more functions popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicMoreFunctionsPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    explicit MusicMoreFunctionsPopWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

};

#endif // MUSICMOREFUNCTIONSPOPWIDGET_H
