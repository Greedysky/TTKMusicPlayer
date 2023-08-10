#ifndef MUSICTIMERWIDGET_H
#define MUSICTIMERWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
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

#include "musicabstractmovedialog.h"

namespace Ui {
class MusicTimerWidget;
}

/*! @brief The class of the timer widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTimerWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTimerWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicTimerWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicTimerWidget();

    /*!
     * Set current songs path list.
     */
    void setSongStringList(const QStringList &list);

public Q_SLOTS:
    /*!
     * Change to index widget.
     */
    void changeWidgetIndex(int index);
    /*!
     * Save the parameter setting results.
     */
    void saveParameterSettings();
    /*!
     * Widget interface control enable or not.
     */
    void buttonClicked(int index);

private:
    /*!
     * Init time to play widget.
     */
    void initFirstWidget();
    /*!
     * Init time to stop widget.
     */
    void initSecondWidget();
    /*!
     * Init time to shutdown widget.
     */
    void initThreeWidget();
    /*!
     * Init play widget control enable or not.
     */
    void setEnabledFirstControl(bool enabled);
    /*!
     * Init stop widget control enable or not..
     */
    void setEnabledSecondControl(bool enabled);
    /*!
     * Init shutdown widget control enable or not..
     */
    void setEnabledThreeControl(bool enabled);
    /*!
     * Init combobox parameter.
     */
    void initComboParameter();
    /*!
     * Init widget interface parameter.
     */
    void initParemeter();
    /*!
     * Init widget interface parameter.
     */
    void writeParemeter() const;

    Ui::MusicTimerWidget *m_ui;
    QStringList m_hour;
    QStringList m_second;
    QStringList m_repeat;

};

#endif // MUSICTIMERWIDGET_H
