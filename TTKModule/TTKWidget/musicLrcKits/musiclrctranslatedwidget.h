#ifndef MUSICLRCTRANSLATEDWIDGET_H
#define MUSICLRCTRANSLATEDWIDGET_H

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

#include "musicglobaldefine.h"
#include "musicwidgetheaders.h"

/*! @brief The class of the lrc translate widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicLrcTranslatedWidget : public QLabel
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicLrcTranslatedWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicLrcTranslatedWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicLrcTranslatedWidget();

    /*!
     * Set current translated string.
     */
    void setPlainText(const QString &name, const QString &text);

private:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    virtual void wheelEvent(QWheelEvent *event) override final;

    QLabel *m_titleName, *m_plainText;

};

#endif // MUSICLRCTRANSLATEDWIDGET_H
