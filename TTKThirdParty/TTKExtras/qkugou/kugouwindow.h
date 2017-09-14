#ifndef KUGOUWINDOW_H
#define KUGOUWINDOW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "kugouurl.h"
#include "musicprivate.h"

class KugouWindowPrivate;

/*! @brief The namespace of the kugou window widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT KugouWindow : public QWidget
{
    Q_OBJECT
public:
    enum KuGouType
    {
        KuGouSong,      /*!< KuGou Song*/
        KuGouRadio,     /*!< KuGou Radio*/
        KuGouList,      /*!< KuGou List*/
        KugouMv,        /*!< Kugou Mv*/
        KuGouLive,      /*!< KuGou Live*/
        KuGouLrc,       /*!< KuGou Lrc*/
        KuGouSingle     /*!< KuGou Single*/
    };

    /*!
     * Object contsructor.
     */
    explicit KugouWindow(KuGouType type, QWidget *parent = 0);

    /*!
     * Set current web viewer url to show.
     */
    void setUrl(const QString &url);

public Q_SLOTS:
    /*!
     * Set web index go back.
     */
    void goBack();
    /*!
     * Kugou song index changed.
     */
    void kugouSongIndexChanged(int index);
    /*!
     * Kugou radio index changed.
     */
    void kugouRadioIndexChanged(int index);
    /*!
     * Kugou MV index changed.
     */
    void kugouMVIndexChanged(int index);

protected:
    /*!
     * Create web view widget.
     */
    void createWebViewer();
    /*!
     * Create kugou song widget.
     */
    void createKugouSongWidget();
    /*!
     * Create kugou radio widget.
     */
    void createKugouRadioWidget();
    /*!
     * Create kugou list widget.
     */
    void createKugouListWidget();
    /*!
     * Create kugou mv widget.
     */
    void createKugouMVWidget();
    /*!
     * Create kugou single widget.
     */
    void createKugouSingleWidget();
    /*!
     * Change clicked button style by index.
     */
    void changeClickedButtonStyle(int index);

private:
    TTK_DECLARE_PRIVATE(KugouWindow)

};

#endif // KUGOUWINDOW_H
