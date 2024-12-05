#ifndef MUSICEMOJILABELWIDGET_H
#define MUSICEMOJILABELWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include <QLabel>
#include "musicglobaldefine.h"

class QToolButton;

/*! @brief The class of the emoji label widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicEMOJILabelWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicEMOJILabelWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicEMOJILabelWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicEMOJILabelWidget();

Q_SIGNALS:
    /*!
     * Get current label emoji.
     */
    void dataChanged(const QString &data);

private Q_SLOTS:
    /*!
     * Page button changed.
     */
    void buttonClicked(int index);
    /*!
     * Label selected state changed.
     */
    void labelClicked(int index);

private:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override final;

    int m_currentPage;
    QList<QLabel*> m_labelItems;
    QList<QToolButton*> m_buttonItems;
    QStringList m_datas;

};

#endif // MUSICEMOJILABELWIDGET_H
