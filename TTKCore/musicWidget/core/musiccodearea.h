#ifndef MUSICCODEAREA_H
#define MUSICCODEAREA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicclickedlabel.h"

/*! @brief The class of the label show code area.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicCodeArea : public MusicClickedLabel
{
    Q_OBJECT
public:
    explicit MusicCodeArea(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicCodeArea();

    void renderPicture();
    /*!
     * Render the verify code picture.
     */
    inline QString getCheckCode() const { return m_sCode; }
    /*!
     * Get the verify code string.
     */

    void setCodeCount(int nCount);
    /*!
     * Set the verify code count length.
     */
    inline int getCodeCount() const { return m_nCodeCount; }
    /*!
     * Get the verify code count length.
     */

    void setNoisyPointCount(int nCount);
    /*!
     * Set the noisy point count.
     */
    inline int getNoisyPointCount() const { return m_nNoisyPointCount; }
    /*!
     * Get the noisy point count.
     */

protected:
    void drawOutline(QPainter &painter);
    /*!
     * Draw outline pie.
     */
    void drawNoisyPoint(QPainter &painter);
    /*!
     * Draw noisy point.
     */
    void drawConversion(QPainter &painter);
    /*!
     * Draw conversion.
     */
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_nNoisyPointCount, m_nConverseRotate;
    int m_nConverseScale, m_nCodeCount;
    QString m_sCode;
    QStringList m_slCodeRange;
    QList<QPainterPath> m_lCodePic;
    QList<Qt::GlobalColor> m_lCodeColor;

};

#endif // MUSICCODEAREA_H
