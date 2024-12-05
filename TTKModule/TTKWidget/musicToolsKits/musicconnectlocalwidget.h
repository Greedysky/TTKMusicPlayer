#ifndef MUSICCONNECTLOCALWIDGET_H
#define MUSICCONNECTLOCALWIDGET_H

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

#include "musicwidgetheaders.h"
#include "musicdeviceinfomodule.h"

class MusicTextSliderWidget;
class MusicLocalManagerWidget;
class MusicAnimationStackedWidget;

/*! @brief The class of the connect local widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConnectLocalWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicConnectLocalWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicConnectLocalWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicConnectLocalWidget();

private Q_SLOTS:
    /*!
     * Change to main widget.
     */
    void changeStatckedWidgetFirst();
    /*!
     * Change to wireless widget.
     */
    void changeStatckedWidgetSecond();
    /*!
     * Change to media library widget.
     */
    void changeStatckedWidgetThird();
    /*!
     * Open transfer files to mobile widget.
     */
    void openTransferFilesToMobile();
    /*!
     * Device type changed.
     */
    void deviceTypeChanged(QAction *action);
    /*!
     * Update device info.
     */
    void updateDeviceInfo();

private:
    /*!
     * Init main widget.
     */
    void initFirstWidget();
    /*!
     * Init connect to wireless widget.
     */
    void initSecondWidget();
    /*!
     * Update device label display.
     */
    void updateDeviceDisplay(const QString &text);

    QLabel *m_deviceInfoLabel;
    QPushButton *m_deviceButton;
    MusicTextSliderWidget *m_deviceSizeLabel;
    MusicDeviceInfoItem m_currentDeviceItem;
    MusicDeviceInfoModule *m_deviceInfo;
    MusicLocalManagerWidget *m_mediaWidget;
    MusicAnimationStackedWidget *m_stackedWidget;

};

#endif // MUSICCONNECTLOCALWIDGET_H
