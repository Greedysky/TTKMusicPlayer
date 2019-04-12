#ifndef MUSICUIOBJECT_H
#define MUSICUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
///Color
    const QString MColorStyle01 = " \
            color:white;";

    const QString MColorStyle02 = " \
            color:rgb(230, 115, 0);";

    const QString MColorStyle03 = " \
            color:#888888;";

    const QString MColorStyle04 = " \
            color:#AAAAAA;";

    const QString MColorStyle05 = " \
            color:#FF0000;";

    const QString MColorStyle06 = " \
            color:#F4F4F4;";

    const QString MColorStyle07 = " \
            color:rgb(160, 99, 11);";

    const QString MColorStyle08 = " \
            color:#158FE1;";

    const QString MColorStyle09 = " \
            color:#666666;";

    const QString MColorStyle10 = " \
            color:#444444;";

    const QString MColorStyle11 = " \
            color:#0172BF;";

    const QString MColorStyle12 = " \
            color:#323232;";

    const QString MColorStyle12_S = "#323232";

    const QString MColorStyle13 = " \
            color:black;";

///Background
    const QString MBackgroundStyle01 = " \
            background-color:transparent;"
#ifdef Q_OS_UNIX
            + QString(" border-style:falt;")
#endif
            ;

    const QString MBackgroundStyle02 = " \
            background-color:black;";

    const QString MBackgroundStyle03 = " \
            background-color:rgb(240, 240, 240);";

    const QString MBackgroundStyle04 = " \
            background-color:rgba(0, 0, 0, 50);";

    const QString MBackgroundStyle05 = " \
            background:rgb(250, 231, 191);";

    const QString MBackgroundStyle06 = " \
            background:rgba(41, 41, 41);";

    const QString MBackgroundStyle07 = " \
            background:rgba(0, 0, 0, 35);";

    const QString MBackgroundStyle08 = " \
            background:rgba(0, 0, 0, 100);";

    const QString MBackgroundStyle09 = " \
            background:rgba(255, 255, 255, 25);";

    const QString MBackgroundStyle10 = " \
            background:rgba(255 ,255, 255, 50);";

    const QString MBackgroundStyle11 = " \
            background:rgba(255, 255, 255, 125);";

    const QString MBackgroundStyle12 = " \
            background:rgba(255, 255, 255, 180);";

    const QString MBackgroundStyle13 = " \
            background:rgba(255, 255, 255, 200);";

    const QString MBackgroundStyle14 = " \
            background:#158FE1;";

    const QString MBackgroundStyle15 = " \
            background:#BFBFBF;";

    const QString MBackgroundStyle16 = " \
            background:#F9D982;";

    const QString MBackgroundStyle17 = " \
            background:#FFFFFF;";

    const QString MBackgroundStyle18 = " \
            background:rgb(254, 249, 229);";

    const QString MBackgroundStyle19 = " \
            background:rgba(0, 0, 0, 175);";

    const QString MBackgroundStyle20 = " \
            background:#EDF8FE;";


///border
    const QString MBorderStyle01 = " \
            border:none;";

    const QString MBorderStyle02 = " \
            border:1px solid #666666;";

    const QString MBorderStyle03 = " \
            border:1px solid #555555;";

    const QString MBorderStyle04 = " \
            border:1px solid gray;";

    const QString MBorderStyle05 = " \
            border:1px solid #0095CD;";

    const QString MBorderStyle06 = " \
            border-top:1px dashed gray;";

    const QString MBorderStyle07 = " \
            border-radius:3px;";


///Font
    const QString MFontStyle01 = " \
            font-weight:bold;";

    const QString MFontStyle02 = " \
            font-size:12px;";

    const QString MFontStyle03 = " \
            font-size:15px;";

    const QString MFontStyle04 = " \
            font-size:18px;";

    const QString MFontStyle05 = " \
            font-size:20px;";

    const QString MFontStyle06 = " \
            font-size:25px;";


///Custom
    const QString MCustomStyle01 = " \
            text-align:left;";

    const QString MCustomStyle02 = " \
            text-align:center;";


///ToolButton
    const QString MToolButtonStyle01 = " \
            QToolButton{  background-color:transparent;  " +
#ifdef Q_OS_UNIX
            QString("border-style:falt;") +
 #endif
            QString("}");

    const QString MToolButtonStyle02 = " \
            QToolButton::hover{ border:1px solid #000000;}";

    const QString MToolButtonStyle03= " \
            QToolButton{ border:none;}";

    const QString MToolButtonStyle04 = MToolButtonStyle01 + MToolButtonStyle03 + " \
            QToolButton::hover{ background-color:rgba(255, 255, 255, 20);}";

    const QString MToolButtonStyle05 = MToolButtonStyle01 + " \
            QToolButton::menu-indicator{ image:None;}";

    const QString MToolButtonStyle06 = " \
            QToolButton{ border:1px solid #777777; background:#FFFFFF; color:#555555;} \
            QToolButton::hover{ border:1px solid #444444; color:#222222;} \
            QToolButton::disabled{ color:#999999;}";


///PushButton
    const QString MPushButtonStyle01 = " \
            QPushButton{ background-color:transparent;  " +
#ifdef Q_OS_UNIX
            QString("border-style:falt;") +
 #endif
            QString("}");

    const QString MPushButtonStyle02 = MPushButtonStyle01 + " \
            QPushButton{ border-none;}";

    const QString MPushButtonStyle03 = " \
            QPushButton{ border-radius:3px; background-color:#158FE1; color:white;}";

    const QString MPushButtonStyle04 = " \
            QPushButton{ border:1px solid #AAAAAA; background:#FFFFFF; color:#777777;} \
            QPushButton::hover{ border:1px solid #555555; color:#444444;} \
            QPushButton::disabled{ color:#999999;}";

    const QString MPushButtonStyle05 = " \
            QPushButton{ border:none; color:#3333FF;} \
            QPushButton::hover{ color:#7777FF;}";

    const QString MPushButtonStyle06 = MPushButtonStyle03 + " \
            QPushButton{ font-size:17px; font-weight:bold; }";

    const QString MPushButtonStyle07 = MPushButtonStyle02 + " \
            QPushButton{ font-size:12px; color:#FFFFFF;}";

    const QString MPushButtonStyle08 = " \
            QPushButton{ color:#CCCCCC;} \
            QPushButton:hover{ color:#FFFFFF;}";
           
    const QString MPushButtonStyle09 = MPushButtonStyle08 + " \
            QPushButton{ text-align:right;}";

    const QString MPushButtonStyle10 = MPushButtonStyle01 + " \
            QPushButton::hover{ border:1px solid #000000;}";

    const QString MPushButtonStyle11 = " \
            QPushButton{ border-radius:5px; font-size:20px; \
            background:rgba(0, 0, 0, 50);} \
            QPushButton::hover{ background:rgba(0, 0, 0, 100);}";

    const QString MPushButtonStyle12 = " \
            QPushButton{ border-radius:2px; background:rgba(0, 0, 0, 50);}";

    const QString MPushButtonStyle13 = " \
            QPushButton::menu-indicator{ image:none;}";

    const QString MPushButtonStyle14 = " \
            QPushButton{ color:#E6C117;} \
            QPushButton:hover{ color:#E6E317;} \
            QPushButton{ text-align:right;}";


///RadioButton
    const QString MRadioButtonStyle01 = " \
            QRadioButton{ background-color:transparent;} \
            QRadioButton::indicator::unchecked{ image:url(:/control/btn_radioButton_unchecked);} \
            QRadioButton::checked { color:rgb(230, 115, 0);} \
            QRadioButton::indicator:checked { image:url(:/control/btn_radiobutton_checked);}";

    const QString MRadioButtonStyle02 = " \
            QRadioButton{ background-color:transparent;} \
            QRadioButton::indicator::unchecked{ image:url(:/control/btn_radioButton2_unchecked);} \
            QRadioButton::checked { color:rgb(230, 115, 0);} \
            QRadioButton::indicator:checked { image:url(:/control/btn_radiobutton2_checked);}";


///CheckBox
    const QString MCheckBoxStyle01 = " \
            QCheckBox{ background-color:transparent;} \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_checkBox_unchecked);} \
            QCheckBox::checked{ color:rgb(230, 115, 0);} \
            QCheckBox::indicator:checked { image:url(:/control/btn_checkBox_checked);}";

    const QString MCheckBoxStyle02 = " \
            QCheckBox{ background-color:transparent;} \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_radioButton2_unchecked);} \
            QCheckBox::checked { color:rgb(230, 115, 0);} \
            QCheckBox::indicator:checked { image:url(:/control/btn_radiobutton2_checked);}";

    const QString MCheckBoxStyle03 = MCheckBoxStyle01 + " \
            QCheckBox::checked{ color:rgb(0, 169, 236);}";

    const QString MCheckBoxStyle04 = MCheckBoxStyle01 + " \
            QCheckBox{ color:#888888;}";

    const QString MCheckBoxStyle05 = " \
            QCheckBox{ background-color:transparent; color:rgb(222, 222, 222);} \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_checkBox2_unchecked);} \
            QCheckBox::checked { color:rgb(255, 255, 255);} \
            QCheckBox::indicator:checked { image:url(:/control/btn_checkBox2_checked);}";


///ScrollBar
    const QString MScrollBarStyle01 = " \
            QScrollBar{ background:#FFFFFF; width:8px; padding-top:0px; padding-bottom:0px;} \
            QScrollBar::handle:vertical{ border-radius:4px; background:#CFCFCF; min-height: 30px;} \
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB;} \
            QScrollBar::handle:vertical:hover{ background:#BBBBBB;} \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none;} \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none;}";

    const QString MScrollBarStyle02 = " \
            QScrollBar{ background:#FFFFFF; height:8px; padding-left:0px; padding-right:0px;} \
            QScrollBar::handle:horizontal{ border-radius:4px; background:#CFCFCF; min-width: 30px;} \
            QScrollBar::handle:horizontal::disabled{ background:#DBDBDB;} \
            QScrollBar::handle:horizontal:hover{ background:#BBBBBB;} \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none;} \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none;}";

    const QString MScrollBarStyle03 = MScrollBarStyle01 + " \
            QScrollBar{ background:transparent;}";

    const QString MScrollBarStyle04 = MScrollBarStyle02 + " \
            QScrollBar{ background:transparent;}";

    const QString MScrollBarStyle05 = " \
            QScrollBar{ background:rgba(255, 255, 255, %1);}";


///Label
    const QString MLabelStyle01 = " \
            QLabel{ color:#888888; background-color:#FFE6E6; border:1px solid gray;}";

    const QString MLabelStyle02 = " \
            QLabel{ color:#777777; font-size:15px;} \
            QLabel::hover{ color:rgb(22, 154, 243);}";

    const QString MLabelStyle03 = " \
            QLabel{ color:rgb(22, 154, 243);}";


///LineEdit
    const QString MLineEditStyle01 = " \
            QLineEdit{ font-size:12px; color:#666666; font-weight:normal; border:1px solid gray;} \
            QLineEdit::hover{ border:1px solid rgb(22, 154, 243);} \
            QLineEdit::disabled{ color:#BBBBBB;}";

    const QString MLineEditStyle02 = " \
            QLineEdit{ border-image:url(:/tiny/lb_transparent);}";

    const QString MLineEditStyle03 = MLineEditStyle02 + " \
            QLineEdit{ font-size:13px; color:#333333;}";

    const QString MLineEditStyle04 = MLineEditStyle02 + " \
            QLineEdit{ color:#6495ED;}";

    const QString MLineEditStyle05 = " \
            QLineEdit{ color:#6495ED; border:1px solid #555555; }";

    const QString MLineEditStyle06 = MLineEditStyle01 + " \
            QLineEdit{ color:white; border:1px solid #545454;} \
            QLineEdit::disabled{ color:#323232; border:1px solid #323232;}";


///Menu
    const QString MMenuStyle01 = " \
            QMenu{ padding:5px;} \
            QMenu::icon{ margin-left:10px;} \
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

    const QString MMenuStyle04 = MMenuStyle03 + " \
            QMenu{ background:rgba(0, 0, 0, 150);}";

    const QString MMenuStyle05 = " \
            QMenu{ border:none; background:transparent;}";


///Slider
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
            QSlider::groove:horizontal{ background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \
            stop:0 #B5B4B4, stop:1 #FCFCFC); height:4px;} \
            QSlider::handle:horizontal{ border-image:url(:/control/lb_slider_handle_normal); \
            width:21px; margin-top:-8px; margin-bottom:-8px;} \
            QSlider::handle:horizontal:disabled{ border-image:url(:/control/lb_slider_disable);} \
            QSlider::handle:horizontal:hover{ border-image:url(:/control/lb_slider_handle_hover); border:none;}";

    const QString MSliderStyle06 = MSliderStyle05 + " \
            QSlider::groove:horizontal{ background:white;} \
            QSlider::sub-page:horizontal{ background:#AFC7F8;} \
            QSlider::add-page:horizontal{ background:white; border:1px solid #ACF;}";

    const QString MSliderStyle07 = " \
            QSlider::add-page:Horizontal{ background-color:rgb(87, 97, 106); height:4px;} \
            QSlider::sub-page:Horizontal{ background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, \
            y2:0, stop:0 rgba(231, 80, 229, 255), stop:1 rgba(7, 208, 255, 255)); height:4px;} \
            QSlider::groove:Horizontal{ background:transparent; height:4px;} \
            QSlider::handle:Horizontal{ border-image:url(':/image/lb_slider'); \
            width:8px; height:6px; margin:-2px 0px -2px 0px;}";

    const QString MSliderStyle08 = " \
            QSlider::groove:horizontal{ background-image:url(:/control/lb_volume_back);} \
            QSlider::sub-page:horizontal{ border-image:url(:/control/lb_volume_fore);} \
            QSlider::add-page:horizontal{ background-image:url(:/control/lb_volume_back);} \
            QSlider::handle:horizontal{ margin:0px -2px 0px -2px; \
            background-image:url(:/control/lb_volume_handle_normal); width:13px;} \
            QSlider::handle:horizontal:hover{ background-image:url(:/control/lb_volume_handle_hover);}";

    const QString MSliderStyle09 = " \
            QSlider::groove:horizontal{ background-image:url(:/control/lb_volume_back);} \
            QSlider::sub-page:horizontal{ border-image:url(:/control/lb_make_fore);} \
            QSlider::add-page:horizontal{ border-image:url(:/control/lb_make_back);} \
            QSlider::handle:horizontal{ margin:0px -1px 0px -1x; \
            border-image:url(:/control/lb_make_slider_handle_normal); width:18px;} \
            QSlider::handle:horizontal:hover{ border-image:url(:/control/lb_make_slider_handle_hover);} \
            QSlider::handle:horizontal:disabled{ border-image:url(:/control/lb_make_slider_disable);}";


///ComboBox
    const QString MComboBoxStyle01 = " \
            QComboBox{ border: 1px solid gray;} \
            QComboBox::hover{ border: 1px solid rgb(22, 154, 243);} \
            QComboBox::drop-down{ subcontrol-origin:padding; \
            subcontrol-position:top right; width:20px; border-left-width:1px; \
            border-left-color:darkgray; border-left-style:solid;} \
            QComboBox::down-arrow{ image:url(:/usermanager/btn_down_arrow);}";

    const QString MComboBoxStyle02 = " \
            QComboBox{ border:none;} \
            QComboBox::hover{ border:none;} \
            QComboBox::drop-down{ subcontrol-origin:padding; \
            subcontrol-position:top right; width:20px; border-left:none;} \
            QComboBox::down-arrow{ image:url(:/usermanager/btn_down_arrow);}";


///TableWidget
    const QString MTableWidgetStyle01 = " \
            QTableWidget{ selection-background-color:rgba(20, 20, 20, 20);}";

    const QString MTableWidgetStyle02 = " \
            QTableWidget{ selection-background-color:rgb(220, 220, 220);}";

    const QString MTableWidgetStyle03 = " \
            QTableWidget{ selection-background-color:rgb(50, 50, 50);}";

    const QString MTableWidgetStyle04 = " \
            QTableWidget{ background:rgba(0, 0, 0, 255);}";


///TableWidget
    const QString MTabWidgetStyle01 = " \
        QTabWidget{ border:none;} \
        QTabBar::tab{ height:30px; width:125px; background-color:#BFBFBF; color:white;} \
        QTabBar::tab:selected{ background-color:#158FE1; color:white}";


///TextEdit
    const QString MTextEditStyle01 = " \
            QTextEdit{ border:1px solid gray; color:#666666;} \
            QTextEdit::hover{ border:1px solid rgb(22, 154, 243);} \
            QTextEdit::disabled{ color:#BBBBBB;}";


///Widget
    const QString MWidgetStyle01 = " \
            QWidget{ background:rgba(255, 255, 255, 50); color:white; font-weight:bold;}";


///DateEdit
    const QString MDateEditStyle01 = " \
            QDateEdit{ font-size:12px; color:#666666; font-weight:normal; border: 1px solid gray;} \
            QDateEdit::hover{ border:1px solid rgb(22, 154, 243);} \
            QDateEdit::up-button{ image:url(:/usermanager/btn_up_arrow);} \
            QDateEdit::down-button{ image:url(:/usermanager/btn_down_arrow);} \
            QDateEdit::disabled{ color:#BBBBBB;}";


///SpinBox
    const QString MSpinBoxStyle01 = " \
            QSpinBox{ font-size:12px; color:#666666; font-weight:normal; border: 1px solid gray;} \
            QSpinBoxt::hover{ border:1px solid rgb(22, 154, 243);} \
            QSpinBox::up-button{ image:url(:/usermanager/btn_up_arrow);} \
            QSpinBox::down-button{ image:url(:/usermanager/btn_down_arrow);} \
            QSpinBox::disabled{ color:#BBBBBB;}";


///ProgressBar
    const QString MProgressBar01 = " \
            QProgressBar{ border:none; background:rgb(210, 225, 240); text-align:center;} \
            QProgressBar::chunk{ background:rgb(60, 140, 220);}";

    const QString MProgressBar02 = " \
            QProgressBar{ border:2px solid grey; border-radius: 5px;} \
            QProgressBar::chunk{ background-color: #05B8CC;}";


///ItemView
    const QString MItemView01 = " \
            QAbstractItemView::item { height:20px; }";

}

#endif // MUSICUIOBJECT_H
