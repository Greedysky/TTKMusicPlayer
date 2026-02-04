#ifndef MUSICABSTRACTITEMQUERYWIDGET_H
#define MUSICABSTRACTITEMQUERYWIDGET_H

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

#include <QButtonGroup>
#include "musicwidgetheaders.h"
#include "musicimagerenderer.h"
#include "musicsongsharingwidget.h"
#include "musicitemquerytablewidget.h"

#define QUERY_WIDGET_WIDTH  (G_SETTING_PTR->value(MusicSettingManager::WidgetSize).toSize().width() - LEFT_SIDE_WIDTH_MIN)

/*! @brief The class of the item query widget base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractItemQueryWidget : public QWidget, public TTKAbstractResizeInterface
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractItemQueryWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractItemQueryWidget();

    /*!
     * Set current value to search founds.
     */
    virtual void setCurrentValue(const QString &value);
    /*!
     * Set current key to search founds.
     */
    virtual void setCurrentKey(const QString &id);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downloadFinished(const QByteArray &bytes);
    /*!
     * Play all button clicked now.
     */
    void playAllButtonClicked();
    /*!
     * Share button clicked now.
     */
    void shareButtonClicked();
    /*!
     * Play button clicked now.
     */
    void playButtonClicked();
    /*!
     * Download button clicked now.
     */
    void downloadButtonClicked();
    /*!
     * Add button clicked now.
     */
    void addButtonClicked();

protected:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    /*!
     * Remove all items.
     */
    void removeItems(QLayout *layout);
    /*!
     * Create the first widget.
     */
    void createFirstWidget();
    /*!
     * Create the second widget.
     */
    void createSecondWidget();
    /*!
     * Set song label count text.
     */
    void setSongCountText();

    QString m_value;
    QWidget *m_mainWidget;
    QPushButton *m_songButton;
    QLabel *m_iconLabel, *m_statusLabel, *m_infoLabel;
    QStackedWidget *m_container;
    MusicResultDataItem m_currentPlaylistItem;
    MusicItemQueryTableWidget *m_tableWidget;
    MusicAbstractQueryRequest *m_networkRequest;
    MusicSongSharingWidget::Module m_shareType;

    struct Data
    {
        QLabel *m_label;
        QFont m_font;
    };
    QList<Data> m_resizeWidgets;

};

#endif // MusicAbstractItemQueryWidget_H
