#ifndef MUSICTTKUIOBJECT_H
#define MUSICTTKUIOBJECT_H

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicTTKUIObject
{
///left item list style
//////////////////////////////////////////////////////
    const QString MKGItemMusic = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_music_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_music_hover);}";

    const QString MKGItemMusicClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_music_clicked);}";

    const QString MKGItemFavourite = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_favorite_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_favorite_hover);}";

    const QString MKGItemFavouriteClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_favorite_clicked);}";

    const QString MKGItemRadio = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_radio_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_radio_hover);}";

    const QString MKGItemRadioClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_radio_clicked);}";

    const QString MKGItemMobile = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_mobile_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_mobile_hover);}";

    const QString MKGItemMobileClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_mobile_clicked);}";

    const QString MKGItemDownload = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_download_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_download_hover);}";

    const QString MKGItemDownloadClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_download_clicked);}";

    const QString MKGItemMore = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_more_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_more_hover);}";

    const QString MKGItemMoreClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_more_clicked);}";


///function list style
//////////////////////////////////////////////////////
    const QString MKGFuncSongFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/song_fore_hover);}";

    const QString MKGFuncSongForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_fore_clicked);}";

    const QString MKGFuncSongBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/song_back_hover);}";

    const QString MKGFuncRadioFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/radio_fore_hover);}";

    const QString MKGFuncRadioForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_fore_clicked);}";

    const QString MKGFuncRadioBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/radio_back_hover);}";

    const QString MKGFuncListFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/list_fore_hover);}";

    const QString MKGFuncListForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_fore_clicked);}";

    const QString MKGFuncListBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/list_back_hover);}";

    const QString MKGFuncMVFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/mv_fore_hover);}";

    const QString MKGFuncMVForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_fore_clicked);}";

    const QString MKGFuncMVBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/mv_back_hover);}";

    const QString MKGFuncLiveFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/live_fore_hover);}";

    const QString MKGFuncLiveForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_fore_clicked);}";

    const QString MKGFuncLiveBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/live_back_hover);}";

    const QString MKGFuncLrcFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/lrc_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/lrc_fore_hover);}";

    const QString MKGFuncLrcForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/lrc_back_clicked);}";


///music magic widget style
//////////////////////////////////////////////////////
    const QString MKGEnhanceOn = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_on_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_on_hover);}";

    const QString MKGEnhanceOff = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_off_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_off_hover);}";

    const QString MKGEnhanceClose = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_close_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_close_hover);}";

    const QString MKGEnhanceHelp = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_help_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_help_hover);}";

    const QString MKGEnhanceShare = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_share_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_share_hover);}";

    const QString MKGEnhanceTitle = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/lb_title_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/lb_title_hover);}";

    const QString MKGEqualizerOn = " \
            QPushButton{ border:none; \
            background-image: url(:/enhance/btn_equalizer_on_normal);} \
            QPushButton:hover{ background-image: url(:/enhance/btn_equalizer_on_hover);}";

    const QString MKGEqualizerOff = " \
            QPushButton{ border:none; \
            background-image: url(:/enhance/btn_equalizer_off_normal);} \
            QPushButton:hover{ background-image: url(:/enhance/btn_equalizer_off_hover);}";


///inline lrc button style
//////////////////////////////////////////////////////
    const QString MKGInlineTranslation = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_translation_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_translation_hover);}";

    const QString MKGInlineMicrophone = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_microphone_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_microphone_hover);}";

    const QString MKGInlineMessage = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_message_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_message_hover);}";

    const QString MKGInlineMovie = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_mv_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_mv_hover);}";


///desktop lrc button style
//////////////////////////////////////////////////////
    const QString MKGDeskTopPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_play_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_play_hover);}";

    const QString MKGDeskTopPause = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_pause_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_pause_hover);}";

    const QString MKGDeskTopNext = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_next_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_next_hover);}";

    const QString MKGDeskTopPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_previous_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_previous_hover);}";

    const QString MKGDeskTopSizeDown = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_size-_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_size-_hover);}";

    const QString MKGDeskTopSizeUp = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_size+_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_size+_hover);}";

    const QString MKGDeskTopSetting = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_setting_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_setting_hover);}";

    const QString MKGDeskTopLock = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_lock_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_lock_hover);}";

    const QString MKGDeskTopStyle= " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_style_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_style_hover);}";

    const QString MKGDeskTopErrorLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_error_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_error_hover);}";

    const QString MKGDeskTopMakeLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_make_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_make_hover);}";

    const QString MKGDeskTopSearchLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_search_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_search_hover);}";

    const QString MKGDeskTopUpdateLrc = " \
            QToolButton{ border:none; \
            background-image: url(:/desktopTool/btn_update_normal);} \
            QToolButton:hover{ background-image: url(:/desktopTool/btn_update_hover);}";


///context button style
//////////////////////////////////////////////////////
    const QString MKGContextPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_play_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_play_normal);}";

    const QString MKGContextPause = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_pause_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_pause_normal);}";

    const QString MKGContextNext = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_next_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_next_normal);}";

    const QString MKGContextPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_previous_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_previous_normal);}";


///tiny button style
//////////////////////////////////////////////////////
    const QString MKGTinyBtnPlay = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_play_hover);}";

    const QString MKGTinyBtnPause = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_pause_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_pause_hover);}";

    const QString MKGTinyBtnNext = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_next_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_next_hover);}";

    const QString MKGTinyBtnPrevious = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_previous_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_previous_hover);}";

    const QString MKGTinyBtnSetting = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_setting_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_setting_hover);}";

    const QString MKGTinyBtnUnDownload = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_download_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_download_hover);}";

    const QString MKGTinyBtnDownload = MKGTinyBtnUnDownload + " \
            QPushButton{ margin-left:-20px; }";

    const QString MKGTinyBtnUnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_unloved_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_unloved_hover);}";

    const QString MKGTinyBtnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_loved_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_loved_hover);}";

    const QString MKGTinyBtnDelete = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_delete_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_delete_hover);}";

    const QString MKGTinyBtnMV = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_mv_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_mv_hover);}";

    const QString MKGTinyBtnPlayLater = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_later_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_play_later_hover);}";

    const QString MKGTinyBtnMore = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_more_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_more_hover);}";

    const QString MKGTinyBtnLocalSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_hover);}";

    const QString MKGTinyBtnMainSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_main_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_main_hover);}";

    const QString MKGTinyBtnLocation = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_location_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_location_hover);}";

    const QString MKGTinyBtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound_hover);}";

    const QString MKGTinyBtnSoundWhite = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound);}";

    const QString MKGTinyBtnClose = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_close_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_close_hover);}";

    const QString MKGTinyBtnLrcExpand = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_collapse_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_collapse_hover);}";

    const QString MKGTinyBtnLrcCollapse = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_expand_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_expand_hover);}";

///function button style
//////////////////////////////////////////////////////
    const QString MKGBtnUnDownload = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_download_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_download_hover);}";

    const QString MKGBtnDownload = MKGBtnUnDownload + " \
            QToolButton{ margin-left:-20px; }";

    const QString MKGBtnSimilar = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_similar_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_similar_hover);}";

    const QString MKGBtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_sound_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_sound_hover);}";

    const QString MKGBtnMore = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_more_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_more_hover);}";

    const QString MKGBtnUnLove = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_unloved_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_unloved_hover);}";

    const QString MKGBtnLove = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_loved_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_loved_hover);}";

    const QString MKGBtnDKLrc = " \
            QCheckBox{ background-color:transparent; } \
            QCheckBox:hover{ image: url(:/functions/btn_lrc_hover);} \
            QCheckBox::indicator::unchecked{ image: url(:/functions/btn_lrc_normal); } \
            QCheckBox::indicator:checked { image: url(:/functions/btn_lrc_hover); }";

    const QString MKGBtnMinimum = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_min_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_min_hover);}";

    const QString MKGBtnTClose = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_close_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_close_hover);}";

    const QString MKGBtnPClose = " \
            QPushButton{ border:none; \
            background-image: url(:/functions/btn_close_normal);} \
            QPushButton:hover{ background-image: url(:/functions/btn_close_hover);}";

    const QString MKGBtnSkin = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_skin_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_skin_hover);}";

    const QString MKGBtnSetting = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_setting_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_setting_hover);}";

    const QString MKGBtnRemote = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_remote_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_remote_hover);}";

    const QString MKGBtnConciseOut = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_show_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_show_hover);}";

    const QString MKGBtnConciseIn = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_unshow_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_unshow_hover);}";


    const QString MKGBtnListLoop = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_listLoop_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_listLoop_hover);}";

    const QString MKGBtnOnce = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_once_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_once_hover);}";

    const QString MKGBtnOneLoop = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_oneLoop_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_oneLoop_hover);}";

    const QString MKGBtnOrder = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_order_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_order_hover);}";

    const QString MKGBtnRandom = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_random_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_random_hover);}";

    const QString MKGBtnPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_play_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_play_hover);}";

    const QString MKGBtnPause = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_pause_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_pause_hover);}";

    const QString MKGBtnNext = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_next_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_next_hover);}";

    const QString MKGBtnPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_previous_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_previous_hover);}";

    const QString MKGBtnMagic = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_hover);}";

    const QString MKGBtnForeBack = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_back_fore_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_back_fore_hover);}";

    const QString MKGBtnBackBack = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_back_back_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_back_back_hover);}";

    const QString MKGBtnForeFresh = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_fresh_fore_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_fresh_fore_hover);}";

    const QString MKGBtnBackFresh = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_fresh_back_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_fresh_back_hover);}";

    const QString MKGBtnPlayedList = " \
            QToolButton{ border:none; \
            background-image: url(:/functions/btn_queue_normal);} \
            QToolButton:hover{ background-image: url(:/functions/btn_queue_hover);}";


}

#endif // MUSICTTKUIOBJECT_H
