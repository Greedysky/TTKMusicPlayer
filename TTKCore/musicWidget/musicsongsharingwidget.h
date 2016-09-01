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

#define WEB_PLAYER     "http://music.daoapp.io/player?song="

#define QQ_SHARE       "http://connect.qq.com/widget/shareqq/index.html?" \
                       "url=%1&desc=%2&pics=%3&title=%4&summary=%5"
#define RENREN_SHARE   "http://widget.renren.com/dialog/share?link=%1" \
                       "&title=%2&image_src=%3"
#define QQ_SPACE_SHARE "http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?" \
                       "url=%1&title=%2&pics=%3&summary=%4"
#define QQ_MICBG_SHARE "http://share.v.t.qq.com/index.php?c=share&a=index" \
                       "&site=%1&title=%2&pic=%3"
#define SINA_SHARE     "http://service.weibo.com/share/share.php?" \
                       "url=%1&pic=%2&title=%3"

class QRCodeQWidget;

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

    static QString getClassName();
    /*!
     * Get class object name.
     */
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
    void downLoadDataChanged(const QString &playUrl, const QString &imageUrl);
    /*!
     * Data download finished and send to shared on web.
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
    QRCodeQWidget *m_qrCodeWidget;

};

#endif // MUSICSONGSHARINGWIDGET_H
