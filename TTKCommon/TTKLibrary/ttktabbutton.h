#ifndef TTKTABBUTTON_H
#define TTKTABBUTTON_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QPushButton>
#include <QButtonGroup>
#include "ttkglobaldefine.h"

/*! @brief The class of the tab button module.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKTabButton : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(TTKTabButton)
public:
    /*!
     * Object contsructor.
     */
    explicit TTKTabButton(QWidget *parent = nullptr);
    ~TTKTabButton();

    /*!
     * Add button by input titles.
     */
    void addButtons(const QStringList &titles);
    /*!
     * Set button enable or disable.
     */
    void setButtonEnabled(bool enable);

    /*!
     * Get button current index.
     */
    inline int currentIndex() const { return m_currentIndex; }
    /*!
     * Get tab buttons count.
     */
    inline int count() const { return m_buttonGroup->buttons().count(); }

Q_SIGNALS:
    /*!
     * Index button click.
     */
    void clicked(int index);

private Q_SLOTS:
    /*!
     * Set diff button clicked by index.
     */
    void buttonClicked(int index);

private:
    int m_currentIndex;
    QButtonGroup *m_buttonGroup;

};

#endif // TTKTABBUTTON_H
