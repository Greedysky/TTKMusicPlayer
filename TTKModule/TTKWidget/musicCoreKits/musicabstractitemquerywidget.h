#ifndef MUSICABSTRACTITEMQUERYWIDGET_H
#define MUSICABSTRACTITEMQUERYWIDGET_H

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

#include <QButtonGroup>
#include "musicwidgetheaders.h"
#include "musicimagerenderer.h"
#include "musicsongsharingwidget.h"
#include "musicitemquerytablewidget.h"

/*! @brief The class of the item query widget base.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractItemQueryWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractItemQueryWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAbstractItemQueryWidget(QWidget *parent = nullptr);
    ~MusicAbstractItemQueryWidget();

    /*!
     * Set current name to search founds.
     */
    virtual void setSongName(const QString &name);
    /*!
     * Set current id to search founds.
     */
    virtual void setSongNameById(const QString &id) = 0;

    /*!
     * Resize window bound by widget resize called.
     */
    virtual void resizeWindow() = 0;

    /*!
     * Init widget interface.
     */
    void initialize();

public Q_SLOTS:
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &bytes);
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
     * Init the first widget.
     */
    void initFirstWidget();
    /*!
     * Init the second widget.
     */
    void initSecondWidget();
    /*!
     * Set song label count text.
     */
    void setSongCountText();

    QString m_songNameFull;
    QWidget *m_mainWindow;
    QPushButton *m_songButton;
    QLabel *m_iconLabel, *m_statusLabel, *m_infoLabel;
    QStackedWidget *m_container;
    MusicResultsItem m_currentPlaylistItem;
    MusicItemQueryTableWidget *m_queryTableWidget;
    MusicAbstractQueryRequest *m_networkRequest;
    MusicSongSharingWidget::Type m_shareType;

    struct TTKResizeWidget
    {
        QLabel *m_label;
        QFont m_font;
    };
    QList<TTKResizeWidget> m_resizeWidgets;
};

#endif // MusicAbstractItemQueryWidget_H
