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
    explicit MusicVideoBarrageStylePopWidget(QWidget *MusicToolMenuWidget = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVideoBarrageStylePopWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    int getBarrageSize() const;
    /*!
     * Get barrage size.
     */
    QColor getBarrageColor() const;
    /*!
     * Get barrage color.
     */

public Q_SLOTS:
    void barrageSizeButtonClicked(int index);
    /*!
     * Barrage size button clicked by index.
     */
    void barrageColorButtonClicked(int index);
    /*!
     * Barrage color button clicked by index.
     */

protected:
    void initWidget();
    /*!
     * Create all widget in layout.
     */

    int m_barrageSize;
    QColor m_barrageColor;
    QButtonGroup *m_sizeGroup;
    QButtonGroup *m_colorGroup;

};

#endif // MUSICVIDEOBARRAGESTYLEPOPWIDGET_H
