#ifndef MUSICNETWORKCONNECTIONTESTWIDGET_H
#define MUSICNETWORKCONNECTIONTESTWIDGET_H

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

#include "musicabstractmovewidget.h"

class MusicNetworkTestThread;

/*! @brief The class of the network connection item Widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNetworkConnectionItem : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicNetworkConnectionItem(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicNetworkConnectionItem();

    /*!
     * Set label text name.
     */
    void setText(const QString &text);
    /*!
     * Set current test url.
     */
    void setUrl(const QString &url);

    /*!
     * Start to test network.
     */
    void start();
    /*!
     * Stop to test network.
     */
    void stop();

Q_SIGNALS:
    /*!
     * Network connection test changed.
     */
    void networkConnectionTestChanged();

private Q_SLOTS:
    /*!
     * Test network finished.
     */
    void testFinshed(bool state);

private:
    MusicNetworkTestThread *m_thread;
    QLabel *m_iconLabel, *m_nameText, *m_stateText;

};


namespace Ui {
class MusicNetworkConnectionTestWidget;
}

/*! @brief The class of the network connection test widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicNetworkConnectionTestWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicNetworkConnectionTestWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicNetworkConnectionTestWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicNetworkConnectionTestWidget();

public Q_SLOTS:
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Button state changed.
     */
    void buttonStateChanged();
    /*!
     * Test network finished.
     */
    void testFinshed();

private:
    Ui::MusicNetworkConnectionTestWidget *m_ui;

    int m_index;
    QList<MusicNetworkConnectionItem*> m_connectionItems;

};

#endif // MUSICNETWORKCONNECTIONTESTWIDGET_H
