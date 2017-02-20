#ifndef QRCODEWIDGET_H
#define QRCODEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "qrencode.h"
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QRCodeQWidgetPrivate;

/*! @brief The namespace of the qr code widget.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QRCodeQWidget : public QWidget
{
    Q_OBJECT
public:
    QRCodeQWidget(const QByteArray &text, const QSize &size, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    void setMargin(const int margin);
    /*!
     * Set qr margin.
     */
    int getMargin() const;
    /*!
     * Get qr margin.
     */

    void setIcon(const QString &path, qreal percent);
    /*!
     * Set qr icon.
     */
    QString getIcon() const;
    /*!
     * Get qr icon.
     */

    void setIconPercent(qreal percent);
    /*!
     * Set qr icon percent.
     */
    qreal getIconPercent() const;
    /*!
     * Get qr icon percent.
     */

    void setCaseSensitive(bool flag);
    /*!
     * Set case sensitive.
     */
    bool caseSensitive() const;
    /*!
     * Get case sensitive.
     */

    void setText(const QByteArray &text);
    /*!
     * Set qr text.
     */
    QByteArray getText() const;
    /*!
     * Get qr text.
     */

    void setForegroundColor(const QColor &color);
    /*!
     * Set qr foreground color.
     */
    QColor getForegroundColor() const;
    /*!
     * Get qr foreground color.
     */

    void setBackgroundColor(const QColor &color);
    /*!
     * Set qr background color.
     */
    QColor getBackgroundColor() const;
    /*!
     * Get qr background color.
     */

    void setMode(QRencodeMode mode);
    /*!
     * Set qr mode.
     */
    QRencodeMode getMode() const;
    /*!
     * Get qr mode.
     */

    void setLevel(QRecLevel level);
    /*!
     * Set qr level.
     */
    QRecLevel getLevel() const;
    /*!
     * Get qr level.
     */

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

private:
    TTK_DECLARE_PRIVATE(QRCodeQWidget)

};


#endif // QRCODEWIDGET_H
