#ifndef MUSICPROGRESSWIDGET_H
#define MUSICPROGRESSWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QProgressDialog>
#include "musicglobaldefine.h"

class QLabel;

/*! @brief The class of the show progress widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicProgressWidget : public QProgressDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicProgressWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicProgressWidget();

    /*!
     * Set widget title.
     */
    void setTitle(const QString &name);

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;
    virtual void resizeEvent(QResizeEvent *event) override final;
    /*!
     * Init widget and parameter.
     */
    void initialize();

    QLabel *m_background, *m_textLabel;
    QProgressBar *m_progressBar;

};

#endif // MUSICPROGRESSWIDGET_H
