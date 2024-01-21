#ifndef MUSICVIDEOQUALITYPOPWIDGET_H
#define MUSICVIDEOQUALITYPOPWIDGET_H

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

#include "musictoolmenuwidget.h"

/*! @brief The class of the video quality popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicVideoQualityPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicVideoQualityPopWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicVideoQualityPopWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicVideoQualityPopWidget();

    /*!
     * Set quality action state enable or disable.
     */
    void setQualityActionState();
    /*!
     * Set current media quality text.
     */
    void setQualityText(const QString &url);

Q_SIGNALS:
    /*!
     * Query music media information data.
     */
    void queryMediaProps(TTK::MusicSongPropertyList &props);
    /*!
     * Set current media url by selected quality.
     */
    void mediaUrlChanged(const QString &data);

public Q_SLOTS:
    /*!
     * Movie quality choiced by index.
     */
    void movieQualityChoiced(QAction *action);

private:
    /*!
     * Create all widget in layout.
     */
    void initialize();
    /*!
     * Find mv url by given bitrate.
     */
    QString findUrlByBitrate(int bitrate);
    /*!
     * Find mv bitrate by given url.
     */
    int findBitrateByUrl(const QString &url);
    /*!
     * Check given bitrate is valid or not.
     */
    bool bitrateIsValid(int bitrate);

    QActionGroup *m_actionGroup;

};

#endif // MUSICVIDEOQUALITYPOPWIDGET_H
