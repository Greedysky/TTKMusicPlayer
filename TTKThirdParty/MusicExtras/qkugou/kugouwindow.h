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
        KuGouSong,      ///*KuGou Song*/
        KuGouRadio,     ///*KuGou Radio*/
        KuGouList,      ///*KuGou List*/
        KugouMv,        ///*Kugou Mv*/
        KuGouLive,      ///*KuGou Live*/
        KuGouLrc,       ///*KuGou Lrc*/
        KuGouSingle     ///*KuGou Single*/
    };

    explicit KugouWindow(KuGouType type, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */

    void setUrl(const QString &url);
    /*!
     * Set current web viewer url to show.
     */

public Q_SLOTS:
    void goBack();
    /*!
     * Set web index go back.
     */
    void kugouSongIndexChanged(int index);
    /*!
     * Kugou song index changed.
     */
    void kugouRadioIndexChanged(int index);
    /*!
     * Kugou radio index changed.
     */
    void kugouMVIndexChanged(int index);
    /*!
     * Kugou MV index changed.
     */

protected:
    void createKugouSongWidget();
    /*!
     * Create kugou song widget.
     */
    void createKugouRadioWidget();
    /*!
     * Create kugou radio widget.
     */
    void createKugouListWidget();
    /*!
     * Create kugou list widget.
     */
    void createKugouMVWidget();
    /*!
     * Create kugou mv widget.
     */
    void createKugouSingleWidget();
    /*!
     * Create kugou single widget.
     */
    void changeClickedButtonStyle(int index);
    /*!
     * Change clicked button style by index.
     */

private:
    TTK_DECLARE_PRIVATE(KugouWindow)

};

#endif // KUGOUWINDOW_H
