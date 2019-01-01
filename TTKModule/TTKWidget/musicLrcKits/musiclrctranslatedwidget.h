#ifndef MUSICLRCTRANSLATEDWIDGET_H
#define MUSICLRCTRANSLATEDWIDGET_H

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

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

/*! @brief The class of the lrc translate widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_LRC_EXPORT MusicLrcTranslatedWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcTranslatedWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicLrcTranslatedWidget(QWidget *parent = nullptr);

    ~MusicLrcTranslatedWidget();

    /*!
     * Set current translated string.
     */
    void setPlainText(const QString &name, const QString &text);

protected:
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    QLabel *m_titleName, *m_plainText;

};

#endif // MUSICLRCTRANSLATEDWIDGET_H
