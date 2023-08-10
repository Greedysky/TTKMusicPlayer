#ifndef MUSICQUALITYCHOICEPOPWIDGET_H
#define MUSICQUALITYCHOICEPOPWIDGET_H

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

#include "musictoolmenuwidget.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the quality choice table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQualityChoiceTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQualityChoiceTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicQualityChoiceTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicQualityChoiceTableWidget();

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Create Table widget item.
     */
    void initialize();

};


/*! @brief The class of the quality choice pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQualityChoicePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQualityChoicePopWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicQualityChoicePopWidget(QWidget *parent = nullptr);

Q_SIGNALS:
    /*!
     * Research query by quality it changed.
     */
    void researchQueryByQuality(const QString &quality);

public Q_SLOTS:
    /*!
     * Current Table item clicked by index.
     */
    void itemCellClicked(int row);

private:
    /*!
     * Create all widget in layout.
     */
    void initialize();

};

#endif // MUSICQUALITYCHOICEPOPWIDGET_H
