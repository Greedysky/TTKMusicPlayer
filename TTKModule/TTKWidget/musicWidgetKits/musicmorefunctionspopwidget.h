#ifndef MUSICMOREFUNCTIONSPOPWIDGET_H
#define MUSICMOREFUNCTIONSPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicMoreFunctionsPopWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set current song name.
     */
    void setCurrentSongName(const QString &name);

private Q_SLOTS:
    /*!
     * Music function button clicked.
     */
    void musicFunctionClicked(QAction *index);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    QString m_currentSongName;

};

#endif // MUSICMOREFUNCTIONSPOPWIDGET_H
