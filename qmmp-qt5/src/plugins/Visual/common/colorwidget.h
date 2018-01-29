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
    explicit ColorWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~ColorWidget();

    static QList<QColor> readColorConfig(const QString &value);
    /*!
     * Read color config.
     */
    static QString writeColorConfig(const QList<QColor> &colors);
    /*!
     * Write color config.
     */

    void setColors(const QList<QColor> &colors);
    /*!
     * Set current colors.
     */
    QList<QColor> getColors() const;
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

protected:
    Ui::ColorWidget *m_ui;

};

#endif // ColorWidget_H
