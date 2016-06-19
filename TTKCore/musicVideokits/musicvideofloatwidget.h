#ifndef MUSICVIDEOFLOATWIDGET_H
#define MUSICVIDEOFLOATWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicfloatabstractwidget.h"

class QPushButton;

/*! @brief The class of the video float widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_VIDEO_EXPORT MusicVideoFloatWidget : public MusicFloatAbstractWidget
{
    Q_OBJECT
public:
    enum Type
    {
        SearchType,     ///*search text type*/
        FreshType,      ///*fresh text type*/
        FullscreenType, ///*fullscreen text type*/
        DownloadType,   ///*download text type*/
        ShareType       ///*share text type*/
    };

    explicit MusicVideoFloatWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicVideoFloatWidget();

    virtual void resizeWidth(int width) override;
    /*!
     * Resize width bound by given width.
     */

    void setText(Type type, const QString &text);
    QString getText(Type type) const;

Q_SIGNALS:
    void searchButtonClicked();
    void freshButtonClicked();
    void fullscreenButtonClicked();
    void downloadButtonClicked();
    void shareButtonClicked();

protected:
    QPushButton *m_search, *m_fresh, *m_fullscreen;
    QPushButton *m_download, *m_share;

};

#endif // MUSICVIDEOFLOATWIDGET_H
