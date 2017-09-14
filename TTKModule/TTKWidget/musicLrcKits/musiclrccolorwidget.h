#ifndef MUSICLRCCOLORWIDGET_H
#define MUSICLRCCOLORWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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
    /*!
     * Object contsructor.
     */
    explicit MusicLrcColorWidget(QWidget *parent = 0);

    virtual ~MusicLrcColorWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Set current colors.
     */
    void setColors(const QList<QColor> &colors);
    /*!
     * Get selected colors.
     */
    QList<QColor> getColors() const;

public Q_SLOTS:
    /*!
     * Add button clicked.
     */
    void addButtonClicked();
    /*!
     * Delete button clicked.
     */
    void deleteButtonClicked();
    /*!
     * Modify button clicked.
     */
    void modifyButtonClicked();
    /*!
     * Up button clicked.
     */
    void upButtonClicked();
    /*!
     * Down button clicked.
     */
    void downButtonClicked();
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicLrcColorWidget *m_ui;

};

#endif // MUSICLRCCOLORWIDGET_H
