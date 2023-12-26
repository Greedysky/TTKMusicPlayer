#ifndef MUSICWEBMVRADIOQUERYWIDGET_H
#define MUSICWEBMVRADIOQUERYWIDGET_H

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

#include "musicabstractitemquerywidget.h"
#include "musiccategoryconfigmanager.h"

class MusicWebMVRadioInfoWidget;
class MusicWebMVRadioQueryCategoryPopWidget;

/*! @brief The class of the music mv query item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebMVRadioQueryItemWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioQueryItemWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWebMVRadioQueryItemWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicWebMVRadioQueryItemWidget();

    /*!
     * Set music results item.
     */
    void setResultDataItem(const MusicResultDataItem &item);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &bytes);
    /*!
     * Image render finished.
     */
    void renderFinished(const QPixmap &data);
    /*!
     * Current item clicked.
     */
    void currentItemClicked();

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;

    MusicResultDataItem m_itemData;
    QPushButton *m_playButton;
    QLabel *m_iconLabel, *m_nameLabel;

};



/*! @brief The class of the music mv query widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebMVRadioQueryWidget : public MusicAbstractItemQueryWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMVRadioQueryWidget)
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
    virtual void resizeWidget() override final;

public Q_SLOTS:
    /*!
     * Create the current category item.
     */
    void createMVRadioCategoryItem(const MusicResultDataItem &item);
    /*!
     * Current item clicked.
     */
    void currentRadioClicked(const MusicResultDataItem &item);
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
