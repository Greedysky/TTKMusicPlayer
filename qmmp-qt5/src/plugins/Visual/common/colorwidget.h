/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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
 ================================================= */

#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QDialog>

namespace Ui {
class ColorWidget;
}

/*! @brief The class of the color picker widget.
 * @author Greedysky <greedysky@163.com>
 */
class ColorWidget : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit ColorWidget(QWidget *parent = 0);

    virtual ~ColorWidget();

    /*!
     * Read color config.
     */
    static QList<QColor> readColorConfig(const QString &value);
    /*!
     * Write color config.
     */
    static QString writeColorConfig(const QList<QColor> &colors);

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

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    Ui::ColorWidget *m_ui;
    QPoint m_pressAt;
    bool m_leftButtonPress;

};

#endif // ColorWidget_H
