#ifndef MUSICLRCCOLORWIDGET_H
#define MUSICLRCCOLORWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicLrcColorWidget;
}

/*! @brief The class of the lrc color picker widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcColorWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicLrcColorWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicLrcColorWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    void setColors(const QList<QColor> &colors);
    /*!
     * Set current colors.
     */
    QList<QColor> getColors();
    /*!
     * Get selected colors.
     */

public Q_SLOTS:
    void addButtonClicked();
    /*!
     * Add button clicked.
     */
    void deleteButtonClicked();
    /*!
     * Delete button clicked.
     */
    void modifyButtonClicked();
    /*!
     * Modify button clicked.
     */
    void upButtonClicked();
    /*!
     * Up button clicked.
     */
    void downButtonClicked();
    /*!
     * Down button clicked.
     */
    virtual int exec();
    /*!
     * Override exec function.
     */

protected:
    Ui::MusicLrcColorWidget *ui;

};

#endif // MUSICLRCCOLORWIDGET_H
