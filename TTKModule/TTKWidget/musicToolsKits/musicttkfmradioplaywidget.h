#ifndef MUSICTTKFMRADIOPLAYWIDGET_H
#define MUSICTTKFMRADIOPLAYWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musicabstractxml.h"
#include "musicabstractmovewidget.h"

class QTreeWidgetItem;
class MusicCoreMPlayer;

namespace Ui {
class MusicTTKFMRadioPlayWidget;
}

/*! @brief The class of the fm category item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFMCategoryItem
{
    QString m_name;
    QString m_location;
    QString m_url;
};
TTK_DECLARE_LIST(MusicFMCategoryItem);


/*! @brief The class of the fm category core.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFMCategory
{
    QString m_category;
    MusicFMCategoryItemList m_items;
};
TTK_DECLARE_LIST(MusicFMCategory);


/*! @brief The class of the fm config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFMConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFMConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFMConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from xml file.
     */
    inline bool fromFile() { return MusicAbstractXml::fromFile(":/data/fmlist"); }

    /*!
     * Read datas from config file.
     */
    void readBuffer(MusicFMCategoryList &items);

};


/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTTKFMRadioPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicTTKFMRadioPlayWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicTTKFMRadioPlayWidget(QWidget *parent = nullptr);
    ~MusicTTKFMRadioPlayWidget();

public Q_SLOTS:
    /*!
     * Radio item clicked changed.
     */
    void radioItemChanged(QTreeWidgetItem *item, int column);
    /*!
     * Media aution play error.
     */
    void mediaAutionPlayError(int code);
    /*!
     * Set radio to play.
     */
    void radioPlay();
    /*!
     * Set radio to previous.
     */
    void radioPrevious();
    /*!
     * Set radio to next.
     */
    void radioNext();
    /*!
     * Set radio volume.
     */
    void radioVolume(int num);
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override final;

    /*!
     * Init parameters.
     */
    void initialize();
    /*!
     * Create core module.
     */
    void createCoreModule();

    Ui::MusicTTKFMRadioPlayWidget *m_ui;
    bool m_isPlaying;
    MusicCoreMPlayer *m_player;
    MusicFMCategoryItemList m_items;

};

#endif // MUSICTTKFMRADIOPLAYWIDGET_H
