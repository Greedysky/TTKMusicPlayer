#ifndef QRCODEWIDGET_H
#define QRCODEWIDGET_H

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

#include <QWidget>
#include "qrencode.h"
#include "ttkprivate.h"
#include "musicextrasglobaldefine.h"

class QRCodeQWidgetPrivate;

/*! @brief The namespace of the qr code widget.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QRCodeQWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    QRCodeQWidget(const QByteArray &text, const QSize &size, QWidget *parent = nullptr);

    /*!
     * Set qr margin.
     */
    void setMargin(const int margin);
    /*!
     * Get qr margin.
     */
    int getMargin() const;

    /*!
     * Set qr icon.
     */
    void setIcon(const QString &path, qreal percent);
    /*!
     * Get qr icon.
     */
    QString getIcon() const;

    /*!
     * Set qr icon percent.
     */
    void setIconPercent(qreal percent);
    /*!
     * Get qr icon percent.
     */
    qreal getIconPercent() const;

    /*!
     * Set case sensitive.
     */
    void setCaseSensitive(bool flag);
    /*!
     * Get case sensitive.
     */
    bool caseSensitive() const;

    /*!
     * Set qr text.
     */
    void setText(const QByteArray &text);
    /*!
     * Get qr text.
     */
    QByteArray getText() const;

    /*!
     * Set qr foreground color.
     */
    void setForegroundColor(const QColor &color);
    /*!
     * Get qr foreground color.
     */
    QColor getForegroundColor() const;

    /*!
     * Set qr background color.
     */
    void setBackgroundColor(const QColor &color);
    /*!
     * Get qr background color.
     */
    QColor getBackgroundColor() const;

    /*!
     * Set qr mode.
     */
    void setMode(QRencodeMode mode);
    /*!
     * Get qr mode.
     */
    QRencodeMode getMode() const;

    /*!
     * Set qr level.
     */
    void setLevel(QRecLevel level);
    /*!
     * Get qr level.
     */
    QRecLevel getLevel() const;

protected:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

private:
    TTK_DECLARE_PRIVATE(QRCodeQWidget)

};


#endif // QRCODEWIDGET_H
