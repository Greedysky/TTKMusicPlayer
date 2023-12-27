#ifndef MUSICLEFTAREAWIDGET_H
#define MUSICLEFTAREAWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QWidget>
#include "musicglobaldefine.h"

class MusicConnectLocalWidget;
class MusicCloudSharedSongWidget;

namespace Ui {
    class MusicApplication;
}

/*! @brief The class of the app left area widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLeftAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLeftAreaWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLeftAreaWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLeftAreaWidget();

    /*!
     * Get class object instance.
     */
    static MusicLeftAreaWidget *instance();

    /*!
     * Set up app ui.
     */
    void setupUi(Ui::MusicApplication *ui);

    /*!
     * Reset current music love icon state.
     */
    void setLoveState(bool state);

Q_SIGNALS:
    /*!
     * Current music love icon state changed.
     */
    void currentLoveStateChanged();
    /*!
     * Current music download icon state changed.
     */
    void currentDownloadStateChanged();

public Q_SLOTS:
    /*!
     * To download music data from net to load.
     */
    void downloadSongToLocal();
    /*!
     * To download music data from net finished.
     */
    void downloadSongFinished();
    /*!
     * Switch to selected item style.
     */
    void switchToSelectedItemStyle(int index);
    /*!
     * Change to song list widget.
     */
    void stackedSongListWidgetChanged();
    /*!
     * Change to shared local widget.
     */
    void stackedLocalWidgetChanged();
    /*!
     * Change to shared cloud widget.
     */
    void stackedCloudWidgetChanged();
    /*!
     * Change to radio widget.
     */
    void stackedRadioWidgetChanged();
    /*!
     * Change to my download widget.
     */
    void stackedMyDownWidgetChanged();

private:
    Ui::MusicApplication *m_ui;
    int m_currentIndex;
    QWidget *m_stackedWidget;
    MusicConnectLocalWidget *m_localSharedSongWidget;
    MusicCloudSharedSongWidget *m_cloudSharedSongWidget;

    static MusicLeftAreaWidget *m_instance;

};

#endif // MUSICLEFTAREAWIDGET_H
