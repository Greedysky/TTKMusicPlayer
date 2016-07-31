#ifndef MUSICUIOBJECT_H
#define MUSICUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
///Custom
//////////////////////////////////////////////////////
    const QString MCustomStyle01 = " \
            background-color:transparent;";

    const QString MCustomStyle02 = "\
            background-color:rgba(255,255,255,125);";

    const QString MCustomStyle03 = "\
            background:rgba(0,0,0,100);";

    const QString MCustomStyle04 = "\
            background:white;border:3px solid rgb(173,216,230);";

    const QString MCustomStyle05 = MCustomStyle01 + "\
            border-image: url(:/tiny/btn_search_line_normal);";

    const QString MCustomStyle07 = "\
            text-align:left; font-weight:bold; font-size:15px;\
            color:red; height:20px; border:none; background-color:white;";

    const QString MCustomStyle08 = "\
            background:rgb(250,231,191);color:rgb(160,99,11);";

    const QString MCustomStyle09 = "\
            background-color:rgba(0, 0, 0, 50);";

    const QString MCustomStyle10 = "\
            border: 1px solid #555555;background-color:rgba(0,0,0,40);";
           
    const QString MCustomStyle11 = "\
            color:white;";

    const QString MCustomStyle12 = "\
            color:#888888;";

    const QString MCustomStyle13 = "\
            color:#AAAAAA;";

    const QString MCustomStyle14 = "\
            color:#FF0000;";

    const QString MCustomStyle17 = "\
            background-color:rgb(240,240,240);";

    const QString MCustomStyle18 = "\
            font-weight:bold;";

    const QString MCustomStyle19 = "\
            background-color:black;";
           
    const QString MCustomStyle20 = "\
            border:none;";
           
    const QString MCustomStyle21 = " \
            background-image:url(':/lrc/lb_film_bg');";
           
    const QString MCustomStyle22 = " \
            background:rgba(255, 255, 255, 50);" + MCustomStyle20;
           
    const QString MCustomStyle23 = " \
            color:white;image:url(:/lrc/lb_shadow);text-align:center;";

    const QString MCustomStyle27 = " \
            background:rgba(0,0,0,35);";

    const QString MCustomStyle28 = " \
            background:rgba(41,41,41);";
          
///ToolButton
//////////////////////////////////////////////////////
    const QString MToolButtonStyle01 = " \
            QToolButton{ background-color:transparent;}";

    const QString MToolButtonStyle02 = " \
            QToolButton::hover{ border:1px solid #000000;}";
            
    const QString MToolButtonStyle03 = MToolButtonStyle01 + " \
            QToolButton::hover{ background-color:rgba(255, 255, 255, 20)}";

    const QString MToolButtonStyle04 = MToolButtonStyle01 + " \
            QToolButton::menu-indicator{ image:None;}";

    const QString MToolButtonStyle07 = MToolButtonStyle01 + MToolButtonStyle02 + " \
            QToolButton{ image:url(:/contextMenu/btn_audition);}";

    const QString MToolButtonStyle08 = MToolButtonStyle01 + MToolButtonStyle02 + " \
            QToolButton{ image:url(:/contextMenu/btn_add);}";

    const QString MToolButtonStyle09 = MToolButtonStyle04 + " \
            QToolButton{ background-image:url(':/quality/lb_background'); border:none; \
            font-weight:bold; color:white}";

    const QString MToolButtonStyle10 = " \
            QToolButton{ border:1px solid #777777; background:#EEEEEE; color:#555555;} \
            QToolButton::hover{ border:1px solid #444444; color:#222222;} \
            QToolButton::disabled{ color:#999999;}";

///PushButton
//////////////////////////////////////////////////////
    const QString MPushButtonStyle02 = " \
            QPushButton::hover{ image:url(:/lrc/lb_shadow);}";

    const QString MPushButtonStyle03 = " \
            QPushButton{ background-color:transparent;}";

    const QString MPushButtonStyle04 = MPushButtonStyle03 + " \
            QPushButton{ border-none;}";
            
    const QString MPushButtonStyle05 = " \
            QPushButton{ border-radius:3px; background-color:#80B7F1; color:white;}";

    const QString MPushButtonStyle06 = " \
            QPushButton{ border-style:outset; border-width: 1px; border-radius: 2px; \
            border-color: #3BA1E6; padding: 1px;}";

    const QString MPushButtonStyle08 = " \
            QPushButton{ border:1px solid #777777; background:#EEEEEE; color:#555555;} \
            QPushButton::hover{ border:1px solid #444444; color:#222222;} \
            QPushButton::disabled{ color:#999999;}";

    const QString MPushButtonStyle09 = " \
            QPushButton{ border:none; color:#3333FF;} \
            QPushButton::hover{ color:#7777FF;}";

    const QString MPushButtonStyle10 = MPushButtonStyle05 + " \
            QPushButton{ font-size:17px; font-weight:bold; }";

    const QString MPushButtonStyle11 = MPushButtonStyle04 + " \
            QPushButton{ font-size:17px; font-weight:bold; color:#FFFFFF;}";

    const QString MPushButtonStyle14 = MPushButtonStyle02 + MPushButtonStyle04 + " \
            QPushButton{ color:white; font-size:15px;}";
           
    const QString MPushButtonStyle15 = MPushButtonStyle02 + MPushButtonStyle03 + " \
            QPushButton{ text-align:left; color:white;}";
           
    const QString MPushButtonStyle17 = MPushButtonStyle06 + " \
            QPushButton{ color:#3BA1E6;}";

    const QString MPushButtonStyle18 = MPushButtonStyle06 + " \
            QPushButton{ font-size:13px; color:white; font-weight:bold;}";


///RadioButton
//////////////////////////////////////////////////////
    const QString MRadioButtonStyle01 = " \
            QRadioButton{ background-color:transparent;} \
            QRadioButton::indicator::unchecked{ image:url(:/control/btn_radioButton_unchecked);} \
            QRadioButton::checked { color:rgb(230, 115, 0);} \
            QRadioButton::indicator:checked { image:url(:/control/btn_radiobutton_checked);}";


///CheckBox
//////////////////////////////////////////////////////
    const QString MCheckBoxStyle01 = " \
            QCheckBox{ background-color:transparent;} \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_checkBox_unchecked);} \
            QCheckBox::checked{ color:rgb(230, 115, 0);} \
            QCheckBox::indicator:checked { image:url(:/control/btn_checkBox_checked);}";
           
    const QString MCheckBoxStyle02 = MCheckBoxStyle01 + " \
            QCheckBox::checked{ color:rgb(0, 169, 236);}";


///ScrollBar
//////////////////////////////////////////////////////
    const QString MScrollBarStyle01 = " \
            QScrollBar{ background:#F0F0F0; width:8px; padding-top:0px; padding-bottom:0px;} \
            QScrollBar::handle:vertical{ border-radius:4px; background:#CFCFCF;} \
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB;} \
            QScrollBar::handle:vertical:hover{ background:#BBBBBB;} \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none;} \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none;}";


///LineEdit
//////////////////////////////////////////////////////
    const QString MLineEditStyle01 = " \
            QLineEdit{ font-size:12px; color:#666666; font-weight:normal; border:1px solid gray;} \
            QLineEdit::hover{ border:1px solid rgb(22, 154, 243);} \
            QLineEdit::disabled{ color:#BBBBBB;}";

    const QString MLineEditStyle02 = " \
            QLineEdit{ border-image:url(:/tiny/lb_transparent);}";

    const QString MLineEditStyle03 = MLineEditStyle02 + " \
            QLineEdit{ font-size:15px; color:#333333;}";

    const QString MLineEditStyle04 = MLineEditStyle02 + " \
            QLineEdit{ color:#6495ED;}";


///Menu
//////////////////////////////////////////////////////
    const QString MMenuStyle01 = " \
            QMenu{ padding: 5px;} \
            QMenu::icon{ margin-left: 10px;} \
            QMenu::right-arrow{ background: url(:/tiny/lb_arrow_up_normal);} \
            QMenu::item{ color:#666666; padding:6px 30px 6px 30px; border:1px solid transparent;} \
            QMenu::item:disabled{ color:#AAAAAA;} \
            QMenu::item:selected{ color:white; background: rgb(22, 154, 243);} \
            QMenu::separator{ height:1px; background:#DDDDDD; margin-top:5px; margin-bottom:5px;}";

    const QString MMenuStyle02 = MMenuStyle01 + " \
            QMenu{ background:rgba(255, 255, 255, 235);}";

    const QString MMenuStyle03 = MMenuStyle01 + " \
            QMenu{ border:none; background:rgba(0, 0, 0, 210); border-radius:4px;} \
            QMenu::item{ color:#BBBBBB;} \
            QMenu::item:disabled{ color:#555555;} \
            QMenu::item:selected{ background:rgba(0, 0, 0, 200);} \
            QMenu::separator{ background:#BBBBBB;}";


///Slider
//////////////////////////////////////////////////////
    const QString MSliderStyle01 = " \
            QSlider::groove:horizontal{ \
            background:#FFFEA1; height:3px; border-radius:1px;} \
            QSlider::sub-page:horizontal{ background:#FFFEA1;} \
            QSlider::add-page:horizontal{ background:#4E4F4F;} \
            QSlider::handle:horizontal{ background:#FFFEA1; \
            width:9px; margin-top:-3px; margin-bottom:-3px; border-radius:4px;}";

    const QString MSliderStyle02 = " \
            QSlider::groove:vertical{ \
            background: #FFFEA1; width:3px; border-radius:1px;} \
            QSlider::sub-page:vertical{ background:#4E4F4F;} \
            QSlider::add-page:vertical{ background:#FFFEA1;} \
            QSlider::handle:vertical{ background:#FFFEA1; \
            height:9px; margin-left:-3px; margin-right:-3px; border-radius:4px;}";

    const QString MSliderStyle03 = " \
            QSlider::groove:vertical{ background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \
            stop:0 #B5B4B4, stop:1 #FCFCFC); width:3px;} \
            QSlider::handle:vertical{ border-image:url(:/control/lb_slider_handle_normal); \
            height:21px; margin-left:-8px; margin-right:-8px;} \
            QSlider::handle:vertical:disabled{ border-image:url(:/control/lb_slider_disable);} \
            QSlider::handle:vertical:hover{ border-image:url(:/control/lb_slider_handle_hover); border:none;}";

    const QString MSliderStyle04 = MSliderStyle03 + " \
            QSlider::groove:vertical{ background:white;} \
            QSlider::sub-page:vertical{ background:#AFC7F8;} \
            QSlider::add-page:vertical{ background:white; border:1px solid #ACF;}";

    const QString MSliderStyle05 = " \
            QSlider::add-page:Horizontal{ background-color:rgb(87, 97, 106); height:4px;} \
            QSlider::sub-page:Horizontal{ background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, \
            y2:0, stop:0 rgba(231, 80, 229, 255), stop:1 rgba(7, 208, 255, 255)); height:4px;} \
            QSlider::groove:Horizontal{ background:transparent;height:4px;} \
            QSlider::handle:Horizontal{ border-image:url(':/image/lb_slider'); \
            width:8px;height:6px; margin:-2px -3px -2px 0px;}";

    const QString MSliderStyle06 = " \
            QSlider::groove:horizontal{ background-image:url(:/control/lb_volume_back);} \
            QSlider::sub-page:horizontal{ border-image:url(:/control/lb_volume_fore);} \
            QSlider::add-page:horizontal{ background-image:url(:/control/lb_volume_back);} \
            QSlider::handle:horizontal{ margin:0px -2px 0px -2px; \
            background-image:url(:/control/lb_volume_handle_normal); width:13px;} \
            QSlider::handle:horizontal:hover{ background-image:url(:/control/lb_volume_handle_hover);}";


///ComboBox
//////////////////////////////////////////////////////
    const QString MComboBoxStyle01 = " \
            QComboBox{ border: 1px solid gray;} \
            QComboBox::hover{ border: 1px solid rgb(22, 154, 243);} \
            QComboBox::drop-down{ subcontrol-origin:padding; \
            subcontrol-position:top right; width:20px; border-left-width:1px; \
            border-left-color:darkgray; border-left-style:solid;} \
            QComboBox::down-arrow{ image:url(:/usermanager/btn_down_arrow);}";


///TableWidget
//////////////////////////////////////////////////////
    const QString MTableWidgetStyle01 = " \
            QTableWidget{ selection-background-color:rgba(20, 20, 20, 40);}";

    const QString MTableWidgetStyle02 = " \
            QTableWidget{ background:rgba(0, 0, 0, 255);}";


///TextEdit
//////////////////////////////////////////////////////
    const QString MTextEditStyle01 = " \
            QTextEdit{ border:1px solid gray; color:#666666;} \
            QTextEdit::hover{ border:1px solid rgb(22, 154, 243);} \
            QTextEdit::disabled{ color:#BBBBBB;}";


///Widget
//////////////////////////////////////////////////////
    const QString MWidgetStyle01 = " \
            QWidget{ background:rgba(255, 255, 255, 50); color:white; font-weight:bold;}";


///DateEdit
//////////////////////////////////////////////////////
    const QString MDateEditStyle01 = " \
            QDateEdit{ font-size:12px; color:#666666; font-weight:normal; border: 1px solid gray;} \
            QDateEdit::hover{ border:1px solid rgb(22, 154, 243);} \
            QDateEdit::up-button{ image:url(:/usermanager/btn_up_arrow);} \
            QDateEdit::down-button{ image:url(:/usermanager/btn_down_arrow);} \
            QDateEdit::disabled{ color:#BBBBBB;}";


///SpinBox
//////////////////////////////////////////////////////
    const QString MSpinBoxStyle01 = " \
            QSpinBox{ font-size:12px; color:#666666; font-weight:normal; border: 1px solid gray;} \
            QSpinBoxt::hover{ border:1px solid rgb(22, 154, 243);} \
            QSpinBox::up-button{ image:url(:/usermanager/btn_up_arrow);} \
            QSpinBox::down-button{ image:url(:/usermanager/btn_down_arrow);} \
            QSpinBox::disabled{ color:#BBBBBB;}";


///ProgressBar
//////////////////////////////////////////////////////
    const QString MProgressBar01 = " \
            QProgressBar{ border:none; background:rgb(210, 225, 240); text-align:center;} \
            QProgressBar::chunk{ background:rgb(60, 140, 220);}";

    const QString MProgressBar02 = " \
            QProgressBar{ border:2px solid grey; border-radius: 5px;} \
            QProgressBar::chunk{ background-color: #05B8CC;}";


///ItemView
//////////////////////////////////////////////////////
    const QString MItemView01 = " \
            QAbstractItemView::item { height:20px; }";

}

#endif // MUSICUIOBJECT_H
