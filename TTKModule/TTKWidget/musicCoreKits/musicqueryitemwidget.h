#ifndef MUSICQUERYITEMWIDGET_H
#define MUSICQUERYITEMWIDGET_H

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

#include "ttkclickedlabel.h"
#include "musicresultinfo.h"
#include "musiccoversourcerequest.h"

class QPushButton;

/*! @brief The class of the square query item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSquareQueryItemWidget : public QLabel
{
    Q_OBJECT
public:
    static constexpr int WIDTH_LABEL_SIZE = 150;
    static constexpr int HEIGHT_LABEL_SIZE = 200;
    static constexpr int LINE_SPACING_SIZE = 150;

public:
    /*!
     * Object constructor.
     */
    explicit MusicSquareQueryItemWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSquareQueryItemWidget();

    /*!
     * Set show time label option.
     */
    inline void setShowTime(bool v) noexcept { m_showTime = v; }
    /*!
     * Set show count label option.
     */
    inline void setShowCount(bool v) noexcept { m_showCount = v; }
    /*!
     * Set show value label option.
     */
    inline void setValueLabel(bool v) noexcept { m_valueLabel->setVisible(v); }

    /*!
     * Set media results item.
     */
    void setResultDataItem(const MusicResultDataItem &item, MusicCoverRequest *cover);

Q_SIGNALS:
    /*!
     * Current item clicked.
     */
    void currentItemClicked(const MusicResultDataItem &item);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downloadFinished(const QByteArray &bytes);
    /*!
     * Image render finished.
     */
    void renderFinished(const QPixmap &data);
    /*!
     * Current item clicked.
     */
    void itemClicked();

private:
    bool m_showTime, m_showCount;
    TTKClickedLabel *m_iconLabel;
    QLabel *m_nameLabel, *m_valueLabel;
    QPushButton *m_listenButton, *m_playButton;
    MusicResultDataItem m_itemData;

};


/*! @brief The class of the rect query item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicRectQueryItemWidget : public QLabel
{
    Q_OBJECT
public:
    static constexpr int WIDTH_LABEL_SIZE = 150;
    static constexpr int HEIGHT_LABEL_SIZE = 100;
    static constexpr int LINE_SPACING_SIZE = 150;

public:
    /*!
     * Object constructor.
     */
    explicit MusicRectQueryItemWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicRectQueryItemWidget();

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
     * Download data from net finished.
     */
    void downloadFinished(const QByteArray &bytes);
    /*!
     * Image render finished.
     */
    void renderFinished(const QPixmap &data);
    /*!
     * Current item clicked.
     */
    void itemClicked();

private:
    /*!
     * Override the widget event.
     */
    virtual void enterEvent(QtEnterEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;

    TTKClickedLabel *m_iconLabel;
    QLabel *m_nameLabel;
    QPushButton *m_playButton;
    MusicResultDataItem m_itemData;

};


/*! @brief The class of the label query item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLabelQueryItemWidget : public TTKClickedLabel
{
    Q_OBJECT
public:
    static constexpr int WIDTH_LABEL_SIZE = 60;
    static constexpr int HEIGHT_LABEL_SIZE = 105;
    static constexpr int LINE_SPACING_SIZE = 105;

public:
    /*!
     * Object constructor.
     */
    explicit MusicLabelQueryItemWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLabelQueryItemWidget();

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
     * Download data from net finished.
     */
    void downloadFinished(const QByteArray &bytes);
    /*!
     * Current item clicked.
     */
    void itemClicked();

private:
    MusicResultDataItem m_itemData;
    QLabel *m_iconLabel, *m_nameLabel;

};


/*! @brief The class of the text query item widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicTextQueryItemWidget : public TTKClickedLabel
{
    Q_OBJECT
public:
    static constexpr int WIDTH_LABEL_SIZE = 150;
    static constexpr int HEIGHT_LABEL_SIZE = 25;
    static constexpr int LINE_SPACING_SIZE = 150;

public:
    /*!
     * Object constructor.
     */
    explicit MusicTextQueryItemWidget(QWidget *parent = nullptr);

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
    void itemClicked();

private:
    MusicResultDataItem m_itemData;

};

#endif // MUSICQUERYITEMWIDGET_H
