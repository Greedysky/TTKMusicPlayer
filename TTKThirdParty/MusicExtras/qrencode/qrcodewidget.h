#ifndef QRCODEWIDGET_H
#define QRCODEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "qrencode.h"
#include "musicextrasglobaldefine.h"

class MUSIC_EXTRAS_EXPORT QRCodeQWidget : public QWidget
{
    Q_OBJECT
public:
    QRCodeQWidget(const QByteArray &text, const QSize &size, QWidget *parent = 0);
    
    static QString getClassName();

    void setMargin(const int margin);
    int getMargin() const;

    void setIcon(const QString &path, qreal percent);
    QString getIcon() const;

    void setIconPercent(qreal percent);
    qreal getIconPercent() const;

    void setCaseSensitive(bool flag);
    bool caseSensitive() const;
 
    void setText(const QByteArray &text);
    QByteArray getText() const;

    void setForegroundColor(const QColor &color);
    QColor getForegroundColor() const;

    void setBackgroundColor(const QColor &color);
    QColor getBackgroundColor() const;

    void setMode(QRencodeMode mode);
    QRencodeMode getMode() const;

    void setLevel(QRecLevel level);
    QRecLevel getLevel() const;


protected:
    virtual void paintEvent(QPaintEvent *event) override;
    
private:
    bool m_casesen;
    int m_margin;
    QString m_iconPath;
    qreal m_percent;
    QByteArray m_text;
    QColor m_foreground, m_background;
    QRencodeMode m_mode;
    QRecLevel m_level;

};


#endif // QRCODEWIDGET_H
