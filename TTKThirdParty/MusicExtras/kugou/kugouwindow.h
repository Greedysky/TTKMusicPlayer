#ifndef KUGOUWINDOW_H
#define KUGOUWINDOW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "kugouurl.h"
#include "musicprivate.h"

class KugouWindowPrivate;

class MUSIC_EXTRAS_EXPORT KugouWindow : public QWidget
{
    Q_OBJECT
public:
    enum KuGouType
    {
        KuGouSong,
        KuGouRadio,
        KuGouList,
        KugouMv,
        KuGouLive,
        KuGouLrc
    };

    explicit KugouWindow(KuGouType type, QWidget *parent = 0);

    static QString getClassName();

public Q_SLOTS:
    void kugouSongIndexChanged(int index);
    void kugouRadioIndexChanged(int index);
    void kugouMVIndexChanged(int index);

protected:
    void createKugouSongWidget();
    void createKugouRadioWidget();
    void createKugouListWidget();
    void createKugouMVWidget();

    void changeClickedButtonStyle(int index);

private:
    TTK_DECLARE_PRIVATE(KugouWindow)

};

#endif // KUGOUWINDOW_H
