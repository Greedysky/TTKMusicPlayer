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
    MusicCodeArea(QWidget *parent = 0);
    ~MusicCodeArea();

	void replaceCodePic();
    inline QString getCheckCode() const { return m_sCode; }

    void setCodeCount(int nCount);
    inline int getCodeCount() const { return m_nCodeCount; }

    void setNoisyPointCount(int nCount);
    inline int getNoisyPointCount() const { return m_nNoisyPointCount; }

protected:
    void drawOutline(QPainter &painter);
    void drawNoisyPoint(QPainter &painter);
    void drawConversion(QPainter &painter);
    void paintEvent(QPaintEvent *event);

    int m_nNoisyPointCount, m_nConverseRotate;
    int m_nConverseScale, m_nCodeCount;
    QString m_sCode;
    QStringList m_slCodeRange;
    QList<QPainterPath> m_lCodePic;
    QList<Qt::GlobalColor> m_lCodeColor;

};

#endif // MUSICCODEAREA_H
