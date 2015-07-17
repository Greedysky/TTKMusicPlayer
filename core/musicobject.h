#ifndef MUSICOBJECT_H
#define MUSICOBJECT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QDir>
#include <QMap>
#include <QDebug>

#define LRC_DOWNLOAD   "MLrc/"
#define MUSIC_DOWNLOAD "Music/"
#define THEME_DOWNLOAD "MTheme/"
#define ART_DOWNLOAD   "MArt/"
#define TRANS_PLUGINS  "MPlugins/"
#define ART_BG         "MArt/background/"
#define TR_LANGUAGE    "MLanguage/"
#define MUSIC_FILE     ".mp3"
#define JPG_FILE       ".skn"
#define LRC_FILE       ".lrc"
#define MAKE_TRANSFORM "MPlugins/avconv.dll"
#define MAKE_RING      "MPlugins/avring.dll"
#define MAKE_RADIO     "MPlugins/avradio.dll"

#define COFIGPATH      "musicconfig.xml"
#define DATABASETYPE   "QSQLITE"
#define NETDADIOPATH   "musicradio.dll"
#define MUSICPATH      "music.dll"
#define DOWNLOADINFO   "music_1.dll"
#define MUSICSEARCH    "music_2.dll"
#define DARABASEPATH   "musicuser_1.dll"
#define USERPATH       "musicuser_2.dll"

#define USERID         20
#define PASSWD         100
#define EMAIL          20
#define USERNAME       20
#define LOGINTIME      20

#define kOptTopMusicURL "http://www.kugou.com/"
#define kOptTopRadioURL "http://www.kugou.com/fmweb/html/index.html"
#define kOptTopMvURL    "http://www.kugou.com/mvweb/html/"

typedef signed char MInt8;           /* 8 bit signed */
typedef unsigned char MUint8;        /* 8 bit unsigned */
typedef short MInt16;                /* 16 bit signed */
typedef unsigned short MUint16;      /* 16 bit unsigned */
typedef int MInt32;                  /* 32 bit signed */
typedef unsigned int MUint32;        /* 32 bit unsigned */
typedef long long MInt64;            /* 64 bit signed */
typedef unsigned long long MUint64;  /* 64 bit unsigned */

typedef double MDouble;              /* double */
typedef float MFloat;                /* float */
typedef bool MBool;                  /* bool */

typedef QList<QStringList> MStringLists;    /* stringlists */
typedef QList<int> MIntList;                /* intlist */
typedef QList<MIntList> MIntLists;          /* intlists */
typedef QSet<int> MIntSet;                  /* intset */
typedef QSet<MIntSet> MIntSets;             /* intsets */
typedef QPair<QStringList,QStringList> MStringListsPair; /* stringlistsPair */
typedef QMap<QString,QVariant> MStriantMap;    /* stringVariantMap */
typedef QMap<QString,QStringList> MStringsListMap; /* stringStrlistsMap */
typedef QMap<int,MIntList> MIntsListMap;        /* intIntlistMap */
typedef QMap<qint64,QString> MIntStringMap;      /* intStrMap */
typedef QMapIterator<QString,QVariant> MStriantMapIt;    /* stringVariantMapIt */
typedef QMapIterator<QString,QStringList> MStringsListMapIt; /* stringStrlistsMapIt */
typedef QMapIterator<int,MIntList> MIntsListMapIt;        /* intIntlistMapIt */
typedef QMapIterator<qint64,QString> MIntStringMapIt;      /* intStrMapIt */



namespace MusicObject
{
    enum XMLConfigType {
    //    FILENAMESLIST,
    //    FILEURLSLIST,
    //    FILENORMALNAMELIST,
    //    FILENORMALURLLIST,
    //    FILELOVESTNAMELIST,
    //    FILELOVESTURLLIST,
    //    FILENETNAMELIST,
    //    FILENETURLLIST,
        AUTOLOGINCHOICED,
        USERLOGINCHOICED,
        USERNAMECHOICED,
        USERPASSWDMD5

    //    CURRENTLANINDEXCHOICED,
    //    PLAY3DMUSICCHOICED,
    //    PLAYMODECHOICED,
    //    VOLUMECHOICED,
    //    AUTOPLAYCHOICED,
    //    CLOSEEVENTCHOICED,
    //    LASTPLAYINDEXCHOICED,
    //    BGTHEMECHOICED,
    //    BGTRANSPARENTCHOICED,

    //    SHOWINLINELRCCHOICED,
    //    SHOWDESKTOPLRCCHOICED,
    //    LRCCOLORCHOICED,
    //    LRCSIZECHOICED,
    //    LRCTYPECHOICED,
    //    LRCFAMILYCHOICED,
    //    LRCFGCOLORCHOICED,
    //    LRCBGCOLORCHOICED,
    //    LRCCOLORTRANSCHOICED,
    //    DLRCCOLORCHOICED,
    //    DLRCSIZECHOICED,
    //    DLRCTYPECHOICED,
    //    DLRCFAMILYCHOICED,
    //    DLRCFGCOLORCHOICED,
    //    DLRCBGCOLORCHOICED,
    //    DLRCCOLORTRANSCHOICED,
    //    DLRCLOCKEDCHOICED,
    //    DLRCGEOMETRYCHOICED,

    //    EQUALIZERENABLECHOICED,
    //    EQUALIZERVALUECHOICED,
    //    EQUALIZERINDEXCHOICED,
    //    SEQUALIZERINDEXCHOICED,

    //    TIMERAUTOINDEXCHOICED,
    //    TIMERAUTOPLAYCHOICED,
    //    TIMERAUTOPLAYHOURCHOICED,
    //    TIMERAUTOPLAYSECONDCHOICED,
    //    TIMERAUTOPLAYREPEATCHOICED,
    //    TIMERAUTOPLAYITEMINDEXCHOICED,
    //    TIMERAUTOPLAYSONGINDEXCHOICED,
    //    TIMERAUTOSTOPCHOICED,
    //    TIMERAUTOSTOPHOURCHOICED,
    //    TIMERAUTOSTOPSECONDCHOICED,
    //    TIMERAUTOSTOPREPEATCHOICED,
    //    TIMERAUTOSHUTDOWNCHOICED,
    //    TIMERAUTOSHUTDOWNHOURCHOICED,
    //    TIMERAUTOSHUTDOWNSECONDCHOICED,
    //    TIMERAUTOSHUTDOWNREPEATCHOICED,
    };

    enum SongPlayType{ MC_PlayOrder = 1, MC_PlayRandom,  MC_PlayListLoop,
                       MC_PlayOneLoop, MC_PlayOnce };

    enum SpecialEQ { EQ_NULL = -1, EQ_EchoEffect, EQ_MixChannelEffect, EQ_ReverseEffect,
                     EQ_SideCutEffect, EQ_CenterCutEffect, EQ_RateUpEffect, EQ_RateDownEffect,
                     EQ_PitchUpEffect, EQ_PitchDownEffect, EQ_TempoUpEffect, EQ_TempoDownEffect,
                     EQ_FadeOutEffect, EQ_FadeInEffect };

    static void dirIsExist(const QString& name)
    {
        QDir dir;
        if(!dir.exists(name)) dir.mkdir(name);
    }

    static void fileIsExist(const QString& name)
    {
        QFile file(name);
        if(!file.exists())
        {
            file.open( QIODevice::WriteOnly );
            file.close();
        }
    }

    static void checkTheDirectoryExist()
    {
        dirIsExist(LRC_DOWNLOAD);
        dirIsExist(MUSIC_DOWNLOAD);
        dirIsExist(THEME_DOWNLOAD);
        dirIsExist(ART_DOWNLOAD);
        dirIsExist(TRANS_PLUGINS);
        dirIsExist(ART_BG);
        dirIsExist(TR_LANGUAGE);

        fileIsExist(DOWNLOADINFO);
        fileIsExist(DARABASEPATH);
        fileIsExist(USERPATH);
        fileIsExist(COFIGPATH);
        fileIsExist(MUSICPATH);
        fileIsExist(MUSICSEARCH);
    }

    static QString getLanguageName(int index)
    {
        QString lan(TR_LANGUAGE);
        switch(index)
        {
            case 0 : return lan.append("cn.ln");
            case 1 : return lan.append("cn_c.ln");
            case 2 : return lan.append("en.ln");
            default: return QString();
        }
    }

    const QString QToolButtonStyle = " \
           QToolButton{background-color:transparent;}\
           QToolButton::hover{background-color:rgba(255,255,255,20)}";

    const QString QToolButtonMenuPopStyle = QToolButtonStyle + " \
           QToolButton::menu-indicator{image:None;}";

    const QString QToolButtonStyleDesktopTool = " \
           QToolButton{border:none;background-color:transparent;}\
           QToolButton::hover{ \
           background:qlineargradient(x1:0,y1:0,x2:0,y2:1,\
           stop:0 #EEEEEE,stop: 0.5 #EEEEEE,stop: 1.0 #EEEEEE);\
           border:none;}";

    const QString QToolBoxItemStyle = " \
           QToolBox::tab:selected {font: italic; font-weight:bold; color: white;}\
           QToolBox::tab{ font-weight:bold; background-color:rgba(255,255,255,100);}";

    const QString MusicSettingMainButton = " \
           QPushButton{background-color:rgb(173,216,230);border:none;}\
           QPushButton::hover{\
           background:qlineargradient(x1:0,y1:0,x2:0,y2:1,\
           stop:0 #3BA1E6,stop: 0.5 #3BA1E6,stop: 1.0 #3BA1E6);\
           border:none;}";

    const QString MusicSettingSubmitButton = " \
           QPushButton{border-style: outset;  \
           border-width: 1px;  \
           border-radius: 2px;  \
           border-color: #3BA1E6;  \
           padding: 1px;}";

    const QString MusicSettingRadioButton = " \
           QRadioButton{ background-color:transparent; } \
           QRadioButton::indicator::unchecked{ image: url(:/control/radiobutton_unchecked); } \
           QRadioButton::checked { color: rgb(230,115,0); } \
           QRadioButton::indicator:checked { image: url(:/control/radiobutton_checked); }";

    const QString MusicSettingCheckButton = " \
           QCheckBox{ background-color:transparent; } \
           QCheckBox::indicator::unchecked{ image: url(:/control/checkbox_unchecked); } \
           QCheckBox::checked { color: rgb(230,115,0); } \
           QCheckBox::indicator:checked { image: url(:/control/checkbox_checked); }";

    const QString MusicScrollBarStyle = " \
           QScrollBar{ background: #494949;width: 13px;}\
           QScrollBar::handle{background: #DBDBDB; min-width: 20px;min-height: 20px;}\
           QScrollBar::handle:vertical::disabled{ background:#DBDBDB;}\
           QScrollBar::handle:vertical:hover{ background:#DBDBDB; border:1px solid rgb(230,115,0);}\
           QScrollBar::add-line, QScrollBar::sub-line { background: none; border: none;}\
           QScrollBar::add-page, QScrollBar::sub-page { background: none; }";

    const QString MusicListEditStyle = " \
           QLineEdit{border: 2px solid gray; font-size:17px; \
           font-weight:bold; color:AAAAFF } \
           QLineEdit::hover{ border: 2px solid #AAAAFF;}";

    const QString MusicMainFunctionButtonForStackWidget = " \
           QPushButton{background-color:transparent;border:none; \
           font-size:17px;font-weight:bold;color:#FFFFFF}\
           QPushButton::hover{\
           background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 rgba(255,255,255,100), \
           stop: 0.5 rgba(255,255,255,100),stop: 1.0 rgba(255,255,255,100));\
           border:none;}";

    const QString MusicSystemTrayMenu = " \
           QMenu { background-color: white; border: 1px solid gray;padding: 5px; }\
           QMenu::item {padding: 5px 25px 5px 30px;border: 1px solid transparent; }\
           QMenu::item:disabled {color: #666666;}\
           QMenu::item:selected { color: white; background: #BBBBBB;}\
           QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

    const QString MusicSystemTrayToolButtonStyle = " \
           QToolButton{background-color:transparent;}";

    const QString MusicBestLoveToolButtonStyle = " \
           QToolButton{background-color:transparent;}\
           QToolButton::hover{ \
           image:url(:/image/shadow);\
           border:none;}";

    const QString MusicVolumeStyleHorizontal = " \
           QSlider::groove:horizontal{ border: 1px solid #4A708B;\
           background: #C0C0C0;height: 5px;border-radius: 1px;\
           padding-left:-1px;padding-right:-1px;}\
           QSlider::sub-page:horizontal{\
           background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
           stop:0 #B1B1B1, stop:1 #c4c4c4);\
           background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\
           stop: 0 #5DCCFF, stop: 1 #1874CD);\
           border: 1px solid #4A708B;height: 10px;border-radius: 2px;}\
           QSlider::add-page:horizontal{background-color:white;\
           border: 0px solid #777;height: 10px;border-radius: 2px;}\
           QSlider::handle:horizontal{\
           background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,\
           stop:0.6 #45ADED, stop:0.778409 rgba(255, 255, 255, 255));\
           width: 11px;margin-top: -3px;\
           margin-bottom: -3px;border-radius: 5px;}\
           QSlider::handle:horizontal:hover{\
           background: qradialgradient(spread:pad, cx:0.5, cy:0.5, \
           radius:0.5, fx:0.5, fy:0.5, stop:0.6 #2A8BDA,\
           stop:0.778409 rgba(255, 255, 255, 255));\
           width: 11px;margin-top: -3px;margin-bottom: -3px;border-radius: 5px;}\
           QSlider::sub-page:horizontal:disabled{background-color:white;border-color: #999;}\
           QSlider::add-page:horizontal:disabled{background: #eee;border-color: #999;}\
           QSlider::handle:horizontal:disabled{\
           background: #eee;border: 1px solid #aaa;border-radius: 4px;}";

    const QString MusicVolumeStyleVertical = " \
           QSlider::groove:vertical {border: 1px solid #4A708B; \
           background: #C0C0C0;  width: 5px;  border-radius: 1px;\
           padding-left:-1px;padding-right:-1px;  padding-top:-1px;  padding-bottom:-1px;}  \
           QSlider::sub-page:vertical {background: #575757;border:1px solid #4A708B;border-radius: 2px;}\
           QSlider::add-page:vertical {  \
           background: qlineargradient(x1:0, y1:0, x2:0, y2:1,  \
           stop:0 #c4c4c4, stop:1 #B1B1B1);  \
           background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \
           stop: 0 #5DCCFF, stop: 1 #1874CD);  \
           border: 0px solid #777;  width: 10px;border-radius: 2px;}  \
           QSlider::handle:vertical{\
           background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, \
           fx:0.5, fy:0.5, stop:0.6 #45ADED,stop:0.778409 rgba(255, 255, 255, 255));\
           height: 11px;margin-left: -3px;margin-right: -3px;border-radius: 5px;}\
           QSlider::handle:vertical:disabled {background: #eee;border: 1px solid #aaa;border-radius: 4px;}";

    const QString MusicEQStyleVertical =  MusicVolumeStyleVertical + "\
           QSlider::sub-page:vertical { \
           background: qlineargradient(x1:0, y1:0, x2:0, y2:1,  \
           stop:0 #c4c4c4, stop:1 #B1B1B1);  \
           background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \
           stop: 0 #5DCCFF, stop: 1 #1874CD);  \
           border: 0px solid #777;  width: 10px;border-radius: 2px;} \
           QSlider::handle:vertical:hover{ image:url(:/image/shadow);border:none;} ";

    const QString MusicLoginEditStyle = " \
           QLineEdit{ border: 1px solid gray;} \
           QLineEdit::hover{ border: 1px solid #AAAAFF;}";

    const QString MusicLoginComboboxEditStyle = " \
           QComboBox{ border: 1px solid gray;} \
           QComboBox::hover{ border: 1px solid #AAAAFF;} \
           QComboBox::drop-down { subcontrol-origin: padding; \
           subcontrol-position: top right;width: 20px;border-left-width: 1px;\
           border-left-color: darkgray; border-left-style: solid;}\
           QComboBox::down-arrow {image:url(:/usermanager/downarrow);}";

    const QString MusicLrcComboboxEditStyle = MusicLoginComboboxEditStyle + "\
           QComboBox::drop-down { subcontrol-origin: padding; \
           subcontrol-position: top right;width: 14px;border-left-width: 1px;\
           border-left-color: darkgray; border-left-style: solid;}";

    const QString MusicEqComboboxEditStyle = MusicLoginComboboxEditStyle + "\
           QComboBox::down-arrow {image:url(:/equalizer/arrow);}";

    const QString MusicLoginPushButtonStyle = " \
           QPushButton{ border: 1px solid gray;background:#DDDDDD;}\
           QPushButton::hover{ border: 2px solid #AAAAFF;}";

    const QString MusicPushButtonTextStyle = " \
           QPushButton{ border:none;color: #3333FF;} \
           QPushButton::hover{ color: #7777FF;}";

    const QString MusicVCodePushButtonStyle = " \
           QPushButton{border-image:url(:/usermanager/vCode); \
           font-size:30px;font-weight:bold;color:gray}";

    const QString MusicUserWindowButtonStyle = " \
           QPushButton{background-color:transparent; border:none; \
           font:italic;font-size:17px; font-weight:bold; color:orange}";

    const QString MusicRemoteControlButton = "\
           QPushButton{background-color:rgba(0,0,0,125);border-none;}\
           QPushButton::hover{image:url(:/image/shadow);}";

    const QString MusicRemoteControlSlider = "\
           QSlider::groove:horizontal{ border: 1px solid #4A708B;\
           background: #C0C0C0;height: 5px;border-radius: 1px;\
           padding-left:-1px;padding-right:-1px;}\
           QSlider::sub-page:horizontal{background-color:rgba(0,0,0,150)}\
           QSlider::add-page:horizontal{background-color:white;\
           border: 0px solid #777;height: 10px;border-radius: 2px;}\
           QSlider::handle:horizontal{\
           background-color:rgba(0,0,0,150);\
           width: 11px;margin-top: -3px;\
           margin-bottom: -3px;border-radius: 5px;}\
           QSlider::handle:horizontal:hover{\
           background-color:rgba(0,0,0,150);\
           width: 11px;margin-top: -3px;margin-bottom: -3px;border-radius: 5px;}";

    const QString MusicRemoteControlMenu = " \
           QMenu {color:white; background-color: rgba(0,0,0,50); border: 1px solid gray;padding: 5px; }\
           QMenu::item {padding: 5px 25px 5px 30px;border: 1px solid transparent; }\
           QMenu::item:disabled {color: #666666;}\
           QMenu::item:selected { color: white; background: #BBBBBB;}\
           QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

}

#endif // MUSICOBJECT_H
