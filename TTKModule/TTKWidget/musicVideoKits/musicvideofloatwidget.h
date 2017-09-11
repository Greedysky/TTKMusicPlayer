#ifndef MUSICVIDEOFLOATWIDGET_H
#define MUSICVIDEOFLOATWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
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

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void resizeWindow(int width, int height) override;
    /*!
     * Resize window bound by given width and height.
     */

    void setText(Type type, const QString &text);
    /*!
     * Set tool button text by type.
     */
    QString getText(Type type) const;
    /*!
     * Get tool button text by type.
     */

Q_SIGNALS:
    void searchButtonClicked();
    /*!
     * Search button clicked.
     */
    void freshButtonClicked();
    /*!
     * Fresh button clicked.
     */
    void fullscreenButtonClicked();
    /*!
     * Fullscreen button clicked.
     */
    void downloadButtonClicked();
    /*!
     * Download button clicked.
     */
    void shareButtonClicked();
    /*!
     * Share button clicked.
     */

protected:
    QPushButton *m_search, *m_fresh, *m_fullscreen;
    QPushButton *m_download, *m_share;

};

#endif // MUSICVIDEOFLOATWIDGET_H
