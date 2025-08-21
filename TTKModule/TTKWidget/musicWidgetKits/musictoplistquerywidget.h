#ifndef MUSICTOPLISTQUERYWIDGET_H
#define MUSICTOPLISTQUERYWIDGET_H

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

#include "musicabstractitemquerywidget.h"
#include "musiccategoryconfigmanager.h"

class MusicToplistQueryCategoryPopWidget;

/*! @brief The class of the toplist music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicToplistQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicToplistQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicToplistQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void queryAllFinished();
    /*!
     * Create the current toplist info item.
     */
    void createToplistItem(const MusicResultDataItem &item);
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);

private:
    /*!
     * Create init interface lables.
     */
    void createLabels();

    MusicToplistQueryCategoryPopWidget *m_categoryButton;

};

#endif // MUSICTOPLISTQUERYWIDGET_H
