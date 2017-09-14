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
        SearchType,     /*!< search text type*/
        FreshType,      /*!< fresh text type*/
        FullscreenType, /*!< fullscreen text type*/
        DownloadType,   /*!< download text type*/
        ShareType       /*!< share text type*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicVideoFloatWidget(QWidget *parent = 0);

    virtual ~MusicVideoFloatWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Resize window bound by given width and height.
     */
    virtual void resizeWindow(int width, int height) override;

    /*!
     * Set tool button text by type.
     */
    void setText(Type type, const QString &text);
    /*!
     * Get tool button text by type.
     */
    QString getText(Type type) const;

Q_SIGNALS:
    /*!
     * Search button clicked.
     */
    void searchButtonClicked();
    /*!
     * Fresh button clicked.
     */
    void freshButtonClicked();
    /*!
     * Fullscreen button clicked.
     */
    void fullscreenButtonClicked();
    /*!
     * Download button clicked.
     */
    void downloadButtonClicked();
    /*!
     * Share button clicked.
     */
    void shareButtonClicked();

protected:
    QPushButton *m_search, *m_fresh, *m_fullscreen;
    QPushButton *m_download, *m_share;

};

#endif // MUSICVIDEOFLOATWIDGET_H
