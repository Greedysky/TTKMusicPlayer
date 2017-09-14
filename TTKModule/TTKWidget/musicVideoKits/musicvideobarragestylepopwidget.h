#ifndef MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
#define MUSICVIDEOBARRAGESTYLEPOPWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musictoolmenuwidget.h"

/*! @brief The class of the video varrage style popup widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoBarrageStylePopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicVideoBarrageStylePopWidget(QWidget *MusicToolMenuWidget = 0);

    virtual ~MusicVideoBarrageStylePopWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Get barrage size.
     */
    int getBarrageSize() const;
    /*!
     * Get barrage color.
     */
    QColor getBarrageColor() const;

public Q_SLOTS:
    /*!
     * Barrage size button clicked by index.
     */
    void barrageSizeButtonClicked(int index);
    /*!
     * Barrage color button clicked by index.
     */
    void barrageColorButtonClicked(int index);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();

    int m_barrageSize;
    QColor m_barrageColor;
    QButtonGroup *m_sizeGroup;
    QButtonGroup *m_colorGroup;

};

#endif // MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
