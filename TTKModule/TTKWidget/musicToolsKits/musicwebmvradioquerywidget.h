#ifndef MUSICWEBMVRADIOQUERYWIDGET_H
#define MUSICWEBMVRADIOQUERYWIDGET_H

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

#include "musiccategoryconfigmanager.h"
#include "musicabstractitemquerywidget.h"

class MusicWebMVRadioInfoWidget;
class MusicWebMVRadioQueryCategoryPopWidget;

/*! @brief The class of the music mv query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebMVRadioQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebMVRadioQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebMVRadioQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeGeometry() override final;

public Q_SLOTS:
    /*!
     * Create the current category item.
     */
    void createMVRadioCategoryItem(const MusicResultDataItem &item);
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenuClicked();
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);

private:
    bool m_initialized;
    QGridLayout *m_gridLayout;
    MusicWebMVRadioInfoWidget *m_infoWidget;
    MusicWebMVRadioQueryCategoryPopWidget *m_categoryButton;

};

#endif // MUSICWEBMVRADIOFOUNDWIDGET_H
