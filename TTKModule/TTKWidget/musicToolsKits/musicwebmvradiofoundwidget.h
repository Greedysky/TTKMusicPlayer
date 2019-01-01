#ifndef MUSICWEBMVRADIOFOUNDWIDGET_H
#define MUSICWEBMVRADIOFOUNDWIDGET_H

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

#include "musicfoundabstractwidget.h"
#include "musiccategoryconfigmanager.h"

class MusicWebMVRadioInfoWidget;
class MusicWebMVRadioFoundCategoryPopWidget;

/*! @brief The class of the music mv found item widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMVRadioFoundItemWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioFoundItemWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMVRadioFoundItemWidget(QWidget *parent = nullptr);

    virtual ~MusicWebMVRadioFoundItemWidget();

    /*!
     * Set music results item.
     */
    void setMusicResultsItem(const MusicResultsItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultsItem &item);

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    MusicResultsItem m_itemData;
    QPushButton *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel;

};



/*! @brief The class of the music mv found widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMVRadioFoundWidget : public MusicFoundAbstractWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioFoundWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMVRadioFoundWidget(QWidget *parent = nullptr);

    virtual ~MusicWebMVRadioFoundWidget();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name) override;
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) override;

    /*!
     * Resize window bound by widgte resize called.
     */
    virtual void resizeWindow() override;

public Q_SLOTS:
    /*!
     * Create the current category item.
     */
    void createCategoryItem(const MusicResultsItem &item);
    /*!
     * Current item clicked.
     */
    void currentRadioClicked(const MusicResultsItem &item);
    /*!
     * Set current index to main menu page.
     */
    void backToMainMenuClicked();
    /*!
     * Current category changed.
     */
    void categoryChanged(const MusicResultsCategoryItem &category);

protected:
    bool m_firstInit;
    QGridLayout *m_gridLayout;
    MusicWebMVRadioInfoWidget *m_infoWidget;
    MusicWebMVRadioFoundCategoryPopWidget *m_categoryButton;

};

#endif // MUSICWEBMVRADIOFOUNDWIDGET_H
