#ifndef MUSICQUALITYCHOICEPOPWIDGET_H
#define MUSICQUALITYCHOICEPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musictoolmenuwidget.h"
#include "musicabstracttablewidget.h"

/*! @brief The class of the quality choice table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQualityChoiceTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQualityChoiceTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQualityChoiceTableWidget(QWidget *parent = nullptr);

    virtual ~MusicQualityChoiceTableWidget();

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column);
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Create Table widget item.
     */
    void createItems();

};


/*! @brief The class of the quality choice pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicQualityChoicePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQualityChoicePopWidget)
public:
    /*!
     * Object contsructor.
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
    void listCellClicked(int row);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    QString m_currentQuality;

};

#endif // MUSICQUALITYCHOICEPOPWIDGET_H
