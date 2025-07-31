#ifndef MUSICARTISTLISTQUERYWIDGET_H
#define MUSICARTISTLISTQUERYWIDGET_H

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

#include "ttkclickedlabel.h"
#include "musicabstractitemquerywidget.h"
#include "musiccategoryconfigmanager.h"

class QGridLayout;
class MusicPageQueryWidget;
class MusicArtistListQueryCategoryPopWidget;

/*! @brief The class of the artist list music item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicArtistListQueryItemWidget : public TTKClickedLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistListQueryItemWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicArtistListQueryItemWidget(QWidget *parent = nullptr);

    /*!
     * Set media results item.
     */
    void setResultDataItem(const MusicResultDataItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

public Q_SLOTS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

private:
    MusicResultDataItem m_itemData;

};



/*! @brief The class of the artist list music query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicArtistListQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicArtistListQueryWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicArtistListQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicArtistListQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value) override final;
    /*!
     * Set current id to search founds.
     */
    virtual void setCurrentID(const QString &id) override final;
    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

public Q_SLOTS:
    /*!
     * Query all quality musics is finished.
     */
    void createArtistListItem(const MusicResultDataItem &item);
    /*!
     * Current artist list clicked.
     */
    void currentArtistListClicked(const MusicResultDataItem &item);
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);
    /*!
     * Page widget button has changed.
     */
    void buttonClicked(int index);
    /*!
     * Number type button has changed.
     */
    void numberButtonClicked(int index);

private:
    bool m_initialized, m_categoryChanged;
    QString m_categoryId;
    QGridLayout *m_gridLayout;
    MusicPageQueryWidget *m_pageQueryWidget;
    MusicArtistListQueryCategoryPopWidget *m_categoryButton;

};

#endif // MUSICARTISTLISTQUERYWIDGET_H
