#ifndef MUSICWEBMUSICRADIOWIDGET_H
#define MUSICWEBMUSICRADIOWIDGET_H

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

#include "musicabstracttablewidget.h"

class QNetworkCookieJar;
class MusicRadioChannelThread;
class MusicWebMusicRadioPlayWidget;

/*! @brief The class of the web music radio list widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMusicRadioWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebMusicRadioWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMusicRadioWidget(QWidget *parent = nullptr);

    ~MusicWebMusicRadioWidget();

    /*!
     * To init list items.
     */
    void initListItems(int index);

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell double click.
     */
    void listCellDoubleClicked(int row, int column);
    /*!
     * Add radio list into list widget.
     */
    void addListWidgetItem();
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &data, const QVariantMap &ext);
    /*!
     * Music item has been clicked.
     */
    void musicPlayClicked();
    /*!
     * Send to desktop link.
     */
    void sendToDesktopLink();

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    int m_outerIndex;
    QNetworkCookieJar *m_cookJar;
    MusicWebMusicRadioPlayWidget *m_musicRadio;
    MusicRadioChannelThread *m_getChannelThread;

};

#endif // MUSICWEBMUSICRADIOWIDGET_H
