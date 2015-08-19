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
///ToolButton
//////////////////////////////////////////////////////
    const QString MToolButtonStyle01 = " \
           QToolButton{background-color:transparent;}\
           QToolButton::hover{background-color:rgba(255,255,255,20)}";

    const QString MToolButtonStyle02 = MToolButtonStyle01 + " \
           QToolButton::menu-indicator{image:None;}";

    const QString MToolButtonStyle03 = " \
           QToolButton{border:none;background-color:transparent;}\
           QToolButton::hover{ \
           background:qlineargradient(x1:0,y1:0,x2:0,y2:1,\
           stop:0 #EEEEEE,stop: 0.5 #EEEEEE,stop: 1.0 #EEEEEE);\
           border:none;}";

    const QString MToolButtonStyle04 = " \
           QToolButton{background-color:transparent;}";

    const QString MToolButtonStyle05 = " \
           QToolButton{background-color:transparent;}\
           QToolButton::hover{ \
           image:url(:/image/shadow);\
           border:none;}";

///ToolBox
//////////////////////////////////////////////////////
    const QString MToolBoxStyle01 = " \
           QToolBox::tab:selected {font: italic; font-weight:bold; color: white;}\
           QToolBox::tab{ font-weight:bold; background-color:rgba(255,255,255,100);}";

///PushButton
//////////////////////////////////////////////////////
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

    const QString MPushButtonStyle03 = " \
           QPushButton{background-color:transparent;border:none; \
           font-size:17px;font-weight:bold;color:#FFFFFF}\
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

    const QString MPushButtonStyle07 = " \
           QPushButton{background-color:transparent; border:none; \
           font:italic;font-size:17px; font-weight:bold; color:orange}";

    const QString MPushButtonStyle08 = "\
           QPushButton{background-color:rgba(0,0,0,125);border-none;}\
           QPushButton::hover{image:url(:/image/shadow);}";

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
    const QString MLineEditStyle01 = " \
           QLineEdit{border: 2px solid gray; font-size:17px; \
           font-weight:bold; color:AAAAFF } \
           QLineEdit::hover{ border: 2px solid #AAAAFF;}";

    const QString MLineEditStyle02 = " \
           QLineEdit{ border: 1px solid gray;} \
           QLineEdit::hover{ border: 1px solid #AAAAFF;}";

///Menu
//////////////////////////////////////////////////////
    const QString MMenuStyle01 = " \
           QMenu { background-color: white; border: 1px solid gray;padding: 5px; }\
           QMenu::item {padding: 5px 25px 5px 30px;border: 1px solid transparent; }\
           QMenu::item:disabled {color: #666666;}\
           QMenu::item:selected { color: white; background: #BBBBBB;}\
           QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

    const QString MMenuStyle02 = " \
           QMenu {color:white; background-color: rgba(0,0,0,50); border: 1px solid gray;padding: 5px; }\
           QMenu::item {padding: 5px 25px 5px 30px;border: 1px solid transparent; }\
           QMenu::item:disabled {color: #666666;}\
           QMenu::item:selected { color: white; background: #BBBBBB;}\
           QMenu::separator {height: 1px;background: #BBBBBB;margin-top: 5px; margin-bottom: 5px;}";

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
           QSlider::handle:horizontal:disabled{\
           background: #eee;border: 1px solid #aaa;border-radius: 4px;}";

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
           QComboBox::drop-down { subcontrol-origin: padding; \
           subcontrol-position: top right;width: 14px;border-left-width: 1px;\
           border-left-color: darkgray; border-left-style: solid;}";

    const QString MComboBoxStyle03 = MComboBoxStyle01 + "\
           QComboBox::down-arrow {image:url(:/equalizer/arrow);}";

}

#endif // MUSICUIOBJECT_H
