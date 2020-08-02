#ifndef MUSICIMAGERENDERER_H
#define MUSICIMAGERENDERER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QSize>
#include "musicabstractthread.h"

/*! @brief The class of the image render thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOLSET_EXPORT MusicImageRenderer : public MusicAbstractThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicImageRenderer)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicImageRenderer(QObject *parent = nullptr);

    ~MusicImageRenderer();

    /*!
     * Set input data array.
     */
    void setInputData(const QByteArray &data, const QSize &size);

Q_SIGNALS:
    /*!
     * Image render finished.
     */
    void renderFinished(const QImage &data);

protected:
    /*!
     * Thread run now.
     */
    virtual void run() override;

private:
    QSize m_size;
    QByteArray m_buffer;

};

#endif // MUSICIMAGERENDERER_H
