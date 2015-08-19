#ifndef MUSICUIOBJECT_H
#define MUSICUIOBJECT_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include <QDebug>

namespace MusicUIObject
{
///Custom
//////////////////////////////////////////////////////
    const QString MCustomStyle20 = " \
            background-color:transparent";

    const QString MCustomStyle01 = "\
            background-color:rgba(255,255,255,125)";

    const QString MCustomStyle02 = "\
            background:rgba(0,0,0,100)";

    const QString MCustomStyle03 = "\
            background:white;border:3px solid rgb(173,216,230)";

    const QString MCustomStyle04 = MCustomStyle20 + "\
            border-image: url(:/share/searchlineleft);";

    const QString MCustomStyle05 = "\
            border-image: url(:/share/searchclosed);";

    const QString MCustomStyle06 = "\
            text-align:left; font-weight:bold; font-size:15px;\
            color:red; height:20px; border:none; background-color:white;";

    const QString MCustomStyle07 = "\
            background:rgb(250,231,191);color:rgb(160,99,11)";

    const QString MCustomStyle08 = "\
            background-color:rgba(0,0,0,155)";

    const QString MCustomStyle09 = "\
            border: 1px solid #555555;background-color:rgba(0,0,0,40)";
           
    const QString MCustomStyle10 = "\
            color:white;";

    const QString MCustomStyle11 = "\
            color:#888888;";

    const QString MCustomStyle12 = "\
            color:#AAAAAA;";

    const QString MCustomStyle13 = "\
            color:#FF0000;";
           
    const QString MCustomStyle18 = "\
            background-color:black";
           
    const QString MCustomStyle19 = "\
            border:none;";
           
    const QString MCustomStyle21 = " \
            background-image:url(':/lrc/film');";
           
    const QString MCustomStyle22 = " \
            background:rgba(255, 255, 255, 50);" + MCustomStyle19;
           
    const QString MCustomStyle23 = " \
            color:white;image:url(:/lrc/shadow);text-align:center";

    const QString MCustomStyle24 = " \
            border-image: url(:/image/soundmax);";

    const QString MCustomStyle25 = " \
            border-image: url(:/image/soundmin);";

    const QString MCustomStyle26 = MCustomStyle20 + MCustomStyle24;

    const QString MCustomStyle27 = " \
            background:rgba(0,0,0,35)";

    const QString MCustomStyle28 = " \
            image:url(:/share/defaultArt)";
          
///ToolButton
//////////////////////////////////////////////////////
    const QString MToolButtonStyle04 = " \
            QToolButton{background-color:transparent;}";

    const QString MToolButtonStyle08 = " \
            QToolButton::hover{ border:1px solid #000000;}";
            
    const QString MToolButtonStyle01 = MToolButtonStyle04 + " \
            QToolButton::hover{background-color:rgba(255,255,255,20)}";

    const QString MToolButtonStyle02 = MToolButtonStyle04 + " \
            QToolButton::menu-indicator{image:None;}";

    const QString MToolButtonStyle03 = MToolButtonStyle04 + " \
            QToolButton{border:none;}\
            QToolButton::hover{ \
            background:qlineargradient(x1:0,y1:0,x2:0,y2:1,\
            stop:0 #EEEEEE,stop: 0.5 #EEEEEE,stop: 1.0 #EEEEEE);\
            border:none;}";

    const QString MToolButtonStyle05 = MToolButtonStyle04 + " \
            QToolButton::hover{ image:url(:/image/shadow); border:none;}";

    const QString MToolButtonStyle06 = MToolButtonStyle04 + MToolButtonStyle08 + " \
            QToolButton{image: url(:/share/autionplay);}";

    const QString MToolButtonStyle07 = MToolButtonStyle04 + MToolButtonStyle08 + " \
            QToolButton{image: url(:/image/addtoplaylist);}";
           
///ToolBox
//////////////////////////////////////////////////////
    const QString MToolBoxStyle01 = " \
            QToolBox::tab:selected {font: italic; font-weight:bold; color: white;}\
            QToolBox::tab{ font-weight:bold; background-color:rgba(255,255,255,100);}";

///PushButton
//////////////////////////////////////////////////////
    const QString MPushButtonStyle13 = "\
            QPushButton::hover{image:url(:/image/shadow);}";

    const QString MPushButtonStyle14 = "\
            QPushButton::hover{image:url(:/lrc/shadow);}";

    const QString MPushButtonStyle15 = "\
            QPushButton{background-color:transparent;}";

    const QString MPushButtonStyle10 = MPushButtonStyle15 + "\
            QPushButton{border-none;}";
            
    const QString MPushButtonStyle01 = " \
            QPushButton{background-color:rgb(173,216,230);border:none;}\
            QPushButton::hover{\
            background:qlineargradient(x1:0,y1:0,x2:0,y2:1,\
            stop:0 #3BA1E6,stop: 0.5 #3BA1E6,stop: 1.0 #3BA1E6);\
            border:none;}";

    const QString MPushButtonStyle02 = " \
            QPushButton{border-style: outset;  \
            border-width: 1px;  \
            border-radius: 2px;  \
            border-color: #3BA1E6;  \
            padding: 1px;}";

    const QString MPushButtonStyle03 =  MPushButtonStyle10 + " \
            QPushButton{font-size:17px;font-weight:bold;color:#FFFFFF}\
            QPushButton::hover{\
            background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 rgba(255,255,255,100), \
            stop: 0.5 rgba(255,255,255,100),stop: 1.0 rgba(255,255,255,100));\
            border:none;}";

    const QString MPushButtonStyle04 = " \
            QPushButton{ border: 1px solid gray;background:#DDDDDD;}\
            QPushButton::hover{ border: 2px solid #AAAAFF;}";

    const QString MPushButtonStyle05 = " \
            QPushButton{ border:none;color: #3333FF;} \
            QPushButton::hover{ color: #7777FF;}";

    const QString MPushButtonStyle06 = " \
            QPushButton{border-image:url(:/usermanager/vCode); \
            font-size:30px;font-weight:bold;color:gray}";

    const QString MPushButtonStyle07 = MPushButtonStyle10 + " \
            QPushButton{font:italic;font-size:17px; font-weight:bold; color:orange}";

    const QString MPushButtonStyle08 = MPushButtonStyle13 + "\
            QPushButton{background-color:rgba(0,0,0,125);border-none;}";

    const QString MPushButtonStyle09 = MPushButtonStyle13 + MPushButtonStyle15 + "\
            QPushButton::hover{border:none;}";

    const QString MPushButtonStyle11 = MPushButtonStyle14 + MPushButtonStyle10 + "\
            QPushButton{color:white;font-size:15px;}";
           
    const QString MPushButtonStyle12 = MPushButtonStyle14 + MPushButtonStyle15 + "\
            QPushButton{text-align:left;color:white;}";
           
///RadioButton
//////////////////////////////////////////////////////
    const QString MRadioButtonStyle01 = " \
            QRadioButton{ background-color:transparent; } \
            QRadioButton::indicator::unchecked{ image: url(:/control/radiobutton_unchecked); } \
            QRadioButton::checked { color: rgb(230,115,0); } \
            QRadioButton::indicator:checked { image: url(:/control/radiobutton_checked); }";

///CheckBox
//////////////////////////////////////////////////////
    const QString MCheckBoxStyle01 = " \
            QCheckBox{ background-color:transparent; } \
            QCheckBox::indicator::unchecked{ image: url(:/control/checkbox_unchecked); } \
            QCheckBox::checked { color: rgb(230,115,0); } \
            QCheckBox::indicator:checked { image: url(:/control/checkbox_checked); }";
           
    const QString MCheckBoxStyle02 = MCheckBoxStyle01 + " \
            QCheckBox::checked { color: rgb(0,169,236); }";
           
///ScrollBar
//////////////////////////////////////////////////////
    const QString MScrollBarStyle01 = " \
            QScrollBar{ background: #494949;width: 13px;}\
            QScrollBar::handle{background: #DBDBDB; min-width: 20px;min-height: 20px;}\
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB;}\
            QScrollBar::handle:vertical:hover{ background:#DBDBDB; border:1px solid rgb(230,115,0);}\
            QScrollBar::add-line, QScrollBar::sub-line { background: none; border: none;}\
            QScrollBar::add-page, QScrollBar::sub-page { background: none; }";

///LineEdit
//////////////////////////////////////////////////////
    const QString MLineEditStyle02 = " \
            QLineEdit{ border: 1px solid gray;} \
            QLineEdit::hover{ border: 1px solid #AAAAFF;}";

    const QString MLineEditStyle01 = MLineEditStyle02 + " \
            QLineEdit{font-size:17px; font-weight:bold; color:AAAAFF }";

    const QString MLineEditStyle03 = " \
            QLineEdit{border-image: url(:/image/searchline); \
            font-size:15px; color:#6495ED;}";

///Menu
//////////////////////////////////////////////////////
    const QString MMenuStyle03 = " \
            QMenu {border: 1px solid gray; padding: 5px; }\
            QMenu::item {padding: 5px 25px 5px 30px;border: 1px solid transparent; }\
            QMenu::item:disabled {color: #666666;}\
            QMenu::item:selected { color: white; background: #BBBBBB;}\
            QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

    const QString MMenuStyle01 = MMenuStyle03 + " \
            QMenu { background-color: white;}";

    const QString MMenuStyle02 = MMenuStyle03 + " \
            QMenu { color:white; background-color: rgba(0,0,0,50);}";

///Slider
//////////////////////////////////////////////////////
    const QString MSliderStyle01 = " \
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
            QSlider::handle:horizontal:disabled{background: #eee;border: 1px solid #aaa;border-radius: 4px;}";

    const QString MSliderStyle02 = " \
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

    const QString MSliderStyle03 =  MSliderStyle02 + "\
            QSlider::sub-page:vertical { \
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,  \
            stop:0 #c4c4c4, stop:1 #B1B1B1);  \
            background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \
            stop: 0 #5DCCFF, stop: 1 #1874CD);  \
            border: 0px solid #777;  width: 10px;border-radius: 2px;} \
            QSlider::handle:vertical:hover{ image:url(:/image/shadow);border:none;} ";

    const QString MSliderStyle04 = "\
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

    const QString MSliderStyle05 = "\
            QSlider::add-page:Horizontal{background-color: rgb(87, 97, 106);height:4px;}\
            QSlider::sub-page:Horizontal{background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1,\
            y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));height:4px;}\
            QSlider::groove:Horizontal{background:transparent;height:4px;} \
            QSlider::handle:Horizontal{border-image:url(':/image/musicslider');\
            width:8px;height: 6px;margin: -2px -3px -2px 0px;}";

///ComboBox
//////////////////////////////////////////////////////
    const QString MComboBoxStyle01 = " \
            QComboBox{ border: 1px solid gray;} \
            QComboBox::hover{ border: 1px solid #AAAAFF;} \
            QComboBox::drop-down { subcontrol-origin: padding; \
            subcontrol-position: top right;width: 20px;border-left-width: 1px;\
            border-left-color: darkgray; border-left-style: solid;}\
            QComboBox::down-arrow {image:url(:/usermanager/downarrow);}";

    const QString MComboBoxStyle02 = MComboBoxStyle01 + "\
            QComboBox::drop-down { width: 14px; }";

    const QString MComboBoxStyle03 = MComboBoxStyle01 + "\
            QComboBox::down-arrow {image:url(:/equalizer/arrow);}";

///TableWidget
//////////////////////////////////////////////////////
    const QString MTableWidgetStyle01 = " \
            QTableWidget{selection-background-color: rgba(20,20,20,100);}";
           
///TextEdit
//////////////////////////////////////////////////////
    const QString MTextEditStyle01 = " \
            QTextEdit{ border: 1px solid gray;} \
            QTextEdit::hover{ border: 1px solid #AAAAFF;}";
           
///Label
//////////////////////////////////////////////////////
    const QString MLabelStyle01 = " \
            QLabel{ background:rgba(255,255,255,50); color:white; font-weight:bold; }";

}

#endif // MUSICUIOBJECT_H
