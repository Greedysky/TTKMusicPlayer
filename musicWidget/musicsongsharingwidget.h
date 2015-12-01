#ifndef MUSICSONGSHARINGWIDGET_H
#define MUSICSONGSHARINGWIDGET_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractmovedialog.h"

#define QQ_SHARE       "http://connect.qq.com/widget/shareqq/index.html?" \
                       "url=www.baidu.com&desc=%1&title=%2&summary=%3"
#define RENREN_SHARE   "http://widget.renren.com/dialog/share?link=http:" \
                       "//www.baidu.com&title=%1"
#define QQ_SPACE_SHARE "http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey" \
                       "?url=http://www.baidu.com&title=%1&summary=%2"
#define QQ_MICBG_SHARE "http://share.v.t.qq.com/index.php?c=share&a=index" \
                       "&site=www.baidu.com&title=%1"
#define SINA_SHARE     "http://service.weibo.com/share/share.php?title=%1"

namespace Ui {
class MusicSongSharingWidget;
}

class MUSIC_WIDGET_EXPORT MusicSongSharingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    explicit MusicSongSharingWidget(QWidget *parent = 0);
    virtual ~MusicSongSharingWidget();

    void setSongName(const QString &name);

public slots:
    virtual int exec();
    void textAreaChanged();
    void confirmButtonClicked();

protected:
    Ui::MusicSongSharingWidget *ui;

};

#endif // MUSICSONGSHARINGWIDGET_H
