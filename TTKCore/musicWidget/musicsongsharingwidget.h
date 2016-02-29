#ifndef MUSICSONGSHARINGWIDGET_H
#define MUSICSONGSHARINGWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"


#define QQ_SHARE       "http://connect.qq.com/widget/shareqq/index.html?" \
                       "url=www.baidu.com&desc=%1&pics=%2&title=%3&summary=%4"
#define RENREN_SHARE   "http://widget.renren.com/dialog/share?link=http:" \
                       "//www.baidu.com&title=%1&image_src=%2"
#define QQ_SPACE_SHARE "http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey" \
                       "?url=http://www.baidu.com&title=%1&pics=%2&summary=%3"
#define QQ_MICBG_SHARE "http://share.v.t.qq.com/index.php?c=share&a=index" \
                       "&site=www.baidu.com&title=%1&pic=%2"
#define SINA_SHARE     "http://service.weibo.com/share/share.php?pic=%1&title=%2"


namespace Ui {
class MusicSongSharingWidget;
}

/*! @brief The class of the song share widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongSharingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSongSharingWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongSharingWidget();

    void setSongName(const QString &name);
    /*!
     * Set current name to share.
     */

public Q_SLOTS:
    virtual int exec();
    /*!
     * Override exec function.
     */
    void textAreaChanged();
    /*!
     * User custom the current share text.
     */
    void confirmButtonClicked();
    /*!
     * Confirm button clicked.
     */
    void data2urlHasChanged(const QString &imageUrl);
    /*!
     * Data2 download finished and send to shared on web.
     */
    void queryUrlTimeout();
    /*!
     * Query url time out, that means share failed.
     */
    void close();
    /*!
     * Override close function.
     */

protected:
    Ui::MusicSongSharingWidget *ui;

};

#endif // MUSICSONGSHARINGWIDGET_H
