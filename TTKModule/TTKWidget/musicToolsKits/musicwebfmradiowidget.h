#ifndef MUSICWEBFMRADIOWIDGET_H
#define MUSICWEBFMRADIOWIDGET_H

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

#include "musicabstracttablewidget.h"

class MusicFMRadioChannelRequest;
class MusicWebFMRadioPlayWidget;

/*! @brief The class of the web music radio list widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebFMRadioWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebFMRadioWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebFMRadioWidget(QWidget *parent = nullptr);
    ~MusicWebFMRadioWidget();

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
     * Table widget item cell double clicked.
     */
    void itemDoubleClicked(int row, int column);
    /*!
     * Add radio list into list widget.
     */
    void addListWidgetItem();
    /*!
     * Send recieved data from net.
     */
    void downLoadFinished(const QByteArray &bytes);
    /*!
     * Music item has been clicked.
     */
    void playClicked();

private:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

    MusicWebFMRadioPlayWidget *m_fmRadio;
    MusicFMRadioChannelRequest *m_channelThread;

};

#endif // MUSICWEBFMRADIOWIDGET_H
