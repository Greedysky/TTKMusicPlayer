#ifndef MUSICOPENFILEWIDGET_H
#define MUSICOPENFILEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"

/*! @brief The class of the open file widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicOpenFileWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicOpenFileWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Adjust the widget rect in the parent widget.
     */
    void adjustRect(int w, int h);

Q_SIGNALS:
    /*!
     * Upload file button clicked.
     */
    void uploadFileClicked();
    /*!
     * Upload files(dir) button clicked.
     */
    void uploadFilesClicked();

};

#endif // MUSICOPENFILEWIDGET_H
