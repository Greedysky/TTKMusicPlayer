#ifndef MUSICCODEAREAWIDGET_H
#define MUSICCODEAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicclickedlabel.h"

/*! @brief The class of the label show code area widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicCodeAreaWidget : public MusicClickedLabel
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicCodeAreaWidget(QWidget *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Render the verify code picture.
     */
    void renderPicture();
    /*!
     * Get the verify code string.
     */
    inline QString getCheckCode() const { return m_sCode; }

    /*!
     * Set the verify code count length.
     */
    void setCodeCount(int nCount);
    /*!
     * Get the verify code count length.
     */
    inline int getCodeCount() const { return m_nCodeCount; }

    /*!
     * Set the noisy point count.
     */
    void setNoisyPointCount(int nCount);
    /*!
     * Get the noisy point count.
     */
    inline int getNoisyPointCount() const { return m_nNoisyPointCount; }

protected:
    /*!
     * Draw outline pie.
     */
    void drawOutline(QPainter &painter);
    /*!
     * Draw noisy point.
     */
    void drawNoisyPoint(QPainter &painter);
    /*!
     * Draw conversion.
     */
    void drawConversion(QPainter &painter);
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override;

    int m_nNoisyPointCount, m_nConverseRotate;
    int m_nConverseScale, m_nCodeCount;
    QString m_sCode;
    QStringList m_slCodeRange;
    QList<QPainterPath> m_lCodePic;
    QList<Qt::GlobalColor> m_lCodeColor;

};

#endif // MUSICCODEAREAWIDGET_H
