#ifndef MUSICWEBDJRADIOQUERYWIDGET_H
#define MUSICWEBDJRADIOQUERYWIDGET_H

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

#include "musicabstractitemquerywidget.h"

class MusicPageQueryWidget;
class MusicWebDJRadioInfoWidget;

/*! @brief The class of the dj radio music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebDJRadioQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebDJRadioQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebDJRadioQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Set current key to search founds.
     */
    virtual void setCurrentKey(const QString &id) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

Q_SIGNALS:
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenu();

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createProgramItem(const MusicResultDataItem &item);
    /*!
     * Current radio clicked.
     */
    void currentRadioClicked(const MusicResultDataItem &item);
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenuClicked();
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);

private:
    bool m_initialized;
    QGridLayout *m_gridLayout;
    MusicPageQueryWidget *m_pageQueryWidget;
    MusicWebDJRadioInfoWidget *m_infoWidget;

};

#endif // MUSICWEBDJRADIOQUERYWIDGET_H
