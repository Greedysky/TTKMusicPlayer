#ifndef TTKTOASTLABEL_H
#define TTKTOASTLABEL_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QLabel>
#include <QTimer>
#include "ttkmoduleexport.h"

/*! @brief The class of the toast widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKToastLabel : public QLabel
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKToastLabel(QWidget *parent = nullptr);
    explicit TTKToastLabel(const QString &text, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~TTKToastLabel();

    /*!
     * Set text margin in the body.
     */
    void setFontMargin(int height, int width) noexcept;
    /*!
     * Get timer interval.
     */
    int timerInterval() const noexcept;
    /*!
     * Set context font size.
     */
    void setFontSize(int size);

    /*!
     * Popup the toast widget in parent widget.
     */
    void popup(QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Animation close changed.
     */
    void finished();

public Q_SLOTS:
    /*!
     * Set context text.
     */
    void setText(const QString &text);

private Q_SLOTS:
    /*!
     * Close animation when tiem out.
     */
    void closeAnimation();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    QFont m_font;
    QPoint m_margin;
    QTimer m_timer;

};

#endif // TTKTOASTLABEL_H
