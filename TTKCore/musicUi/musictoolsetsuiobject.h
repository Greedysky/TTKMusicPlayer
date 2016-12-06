#ifndef MUSICTOOLSETSUIOBJECT_H
#define MUSICTOOLSETSUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the music tool sets button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGSongsDetectStartBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_hover);}";

    const QString MKGSongsDetectStopBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_stop_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_stop_hover);}";

    const QString MKGSongsRedetectBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_redetect_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_redetect_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_redetect_hover);}";

    const QString MKGSongsDetectDownloadBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_download_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_download_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_download_disable);}";

    const QString MKGSongsDetectPlayBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_play_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_play_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_play_disable);}";

    const QString MKGSongsDetectShareBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_share_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_share_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_share_disable);}";

    const QString MKGSongsDetectLoveBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_love_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_love_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_love_disable);}";
            
    const QString MKGSongsDetectUnloveBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_unlove_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_unlove_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_detect_unlove_disable);}";
            
    const QString MKGCheckTestDuplicate = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_duplicate_normal);} \
            QToolButton:hover{ background-image: url(:/toolSets/btn_duplicate_hover);}";

    const QString MKGCheckTestDuplicateClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_duplicate_clicked);}";
            
    const QString MKGCheckTestQuality = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_quality_normal);} \
            QToolButton:hover{ background-image: url(:/toolSets/btn_quality_hover);}";
            
    const QString MKGCheckTestQualityClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_quality_clicked);}";
            
    const QString MKGCheckTestRename = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_rename_normal);} \
            QToolButton:hover{ background-image: url(:/toolSets/btn_rename_hover);}";
            
    const QString MKGCheckTestRenameClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/toolSets/btn_rename_clicked);}";
            
}

#endif // MUSICTOOLSETSUIOBJECT_H
