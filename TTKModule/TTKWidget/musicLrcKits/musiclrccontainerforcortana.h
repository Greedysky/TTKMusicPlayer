#ifndef MUSICLRCCONTAINERFORCORTANA_H
#define MUSICLRCCONTAINERFORCORTANA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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

#include "musiclrccontainer.h"

class MusicVLayoutAnimationWidget;
class MusicTransitionAnimationLabel;

/*! @brief The class of the wall paper lrc container.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcContainerForCortana : public MusicLrcContainer
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcContainerForCortana)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcContainerForCortana(QWidget *parent = nullptr);

    virtual ~MusicLrcContainerForCortana();

    /*!
     * Start timer clock to draw lrc.
     */
    virtual void startTimerClock() override;
    /*!
     * Stop timer clock to draw lrc.
     */
    virtual void stopLrcMask() override;
    /*!
     * Set setting parameter.
     */
    virtual void applySettingParameter() override;

    /*!
     * Init current lrc when the first show.
     */
    virtual void initCurrentLrc() const;
    /*!
     * Update current lrc by first and second and time.
     */
    void updateCurrentLrc(const QString &first, const QString &second, qint64 time);

protected:
    bool m_reverse;
    QPoint m_offset, m_geometry;

};

#endif // MUSICLRCCONTAINERFORCORTANA_H
