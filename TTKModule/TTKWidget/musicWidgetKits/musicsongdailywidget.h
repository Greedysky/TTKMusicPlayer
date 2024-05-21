#ifndef MUSICSONGDAILYWIDGET_H
#define MUSICSONGDAILYWIDGET_H

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

#include <QLabel>
#include "ttkabstractresizeinterface.h"

class QToolButton;
class MusicCiBaRequest;

/*! @brief The class of the song daily widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongDailyWidget : public QFrame, public TTKAbstractResizeInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongDailyWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongDailyWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongDailyWidget();

    /*!
     * Resize widget bound by resize called.
     */
    virtual void resizeWidget() override final;

private Q_SLOTS:
    /*!
     * Download image data from net finished.
     */
    void downLoadImageFinished(const QByteArray &bytes);

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;

    QLabel *m_container;
    QLabel *m_note, *m_content;
    MusicCiBaRequest *m_networkRequest;

};

#endif // MUSICSONGDAILYWIDGET_H
