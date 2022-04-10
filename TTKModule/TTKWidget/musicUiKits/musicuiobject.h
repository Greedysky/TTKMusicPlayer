#ifndef MUSICUIOBJECT_H
#define MUSICUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include "ttkglobal.h"

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
///Color
    const QString MQSSColor01 = "#444444";

///Color
    const QString MQSSColorStyle01 = " \
            color:#FFFFFF;";

    const QString MQSSColorStyle02 = " \
            color:#E67300;";

    const QString MQSSColorStyle03 = " \
            color:#888888;";

    const QString MQSSColorStyle04 = " \
            color:#AAAAAA;";

    const QString MQSSColorStyle05 = " \
            color:#FF0000;";

    const QString MQSSColorStyle06 = " \
            color:#F4F4F4;";

    const QString MQSSColorStyle07 = " \
            color:#A0630B;";

    const QString MQSSColorStyle08 = " \
            color:#158FE1;";

    const QString MQSSColorStyle09 = " \
            color:#444444;";

    const QString MQSSColorStyle10 = " \
            color:#169AF3;";

    const QString MQSSColorStyle11 = " \
            color:#000000;";

///Background
    const QString MQSSBackgroundStyle01 = " \
            background-color:transparent; "
#ifdef Q_OS_UNIX
            + QString("border-style:falt;")
#endif
            ;

    const QString MQSSBackgroundStyle02 = " \
            background-color:black;";

    const QString MQSSBackgroundStyle03 = " \
            background-color:rgb(240, 240, 240);";

    const QString MQSSBackgroundStyle04 = " \
            background-color:rgba(0, 0, 0, 50);";

    const QString MQSSBackgroundStyle05 = " \
            background:rgb(250, 231, 191);";

    const QString MQSSBackgroundStyle06 = " \
            background:rgba(41, 41, 41);";

    const QString MQSSBackgroundStyle07 = " \
            background:rgba(0, 0, 0, 35);";

    const QString MQSSBackgroundStyle08 = " \
            background:rgba(0, 0, 0, 100);";

    const QString MQSSBackgroundStyle09 = " \
            background:rgba(255 ,255, 255, 50);";

    const QString MQSSBackgroundStyle10 = " \
            background:rgba(255, 255, 255, 200);";

    const QString MQSSBackgroundStyle11 = " \
            background:#BFBFBF;";

    const QString MQSSBackgroundStyle12 = " \
            background:#FFFFFF;";

    const QString MQSSBackgroundStyle13 = " \
            background:rgb(254, 249, 229);";

    const QString MQSSBackgroundStyle14 = " \
            background:rgba(0, 0, 0, 175);";

///border
    const QString MQSSBorderStyle01 = " \
            border:none;";

    const QString MQSSBorderStyle02 = " \
            border:1px solid #666666;";

    const QString MQSSBorderStyle03 = " \
            border:1px solid gray;";

    const QString MQSSBorderStyle04 = " \
            border:1px solid #0095CD;";

    const QString MQSSBorderStyle05 = " \
            border-top:1px dashed gray;";

    const QString MQSSBorderStyle06 = " \
            border-radius:3px;";

///Font
    const QString MQSSFontStyle01 = " \
            font-weight:bold;";

    const QString MQSSFontStyle02 = " \
            font-size:12px;";

    const QString MQSSFontStyle03 = " \
            font-size:15px;";

    const QString MQSSFontStyle04 = " \
            font-size:20px;";

    const QString MQSSFontStyle05 = " \
            font-size:25px;";

///Custom
    const QString MQSSCustomStyle01 = " \
            text-align:left;";

    const QString MQSSCustomStyle02 = " \
            text-align:center;";

///ToolButton
    const QString MQSSToolButtonStyle01 = " \
            QToolButton{  background-color:transparent; " +
#ifdef Q_OS_UNIX
            QString("border-style:falt;") +
#endif
            QString("}");

    const QString MQSSToolButtonStyle02 = " \
            QToolButton::hover{ border:1px solid #000000; }";

    const QString MQSSToolButtonStyle03= " \
            QToolButton{ border:none; }";

    const QString MQSSToolButtonStyle04 = MQSSToolButtonStyle01 + MQSSToolButtonStyle03 + " \
            QToolButton::hover{ background-color:rgba(255, 255, 255, 20); }";

    const QString MQSSToolButtonStyle05 = MQSSToolButtonStyle01 + " \
            QToolButton::menu-indicator{ image:None; }";

    const QString MQSSToolButtonStyle06 = " \
            QToolButton{ border:1px solid #777777; background:#FFFFFF; color:#555555; } \
            QToolButton::hover{ border:1px solid #444444; color:#222222; } \
            QToolButton::disabled{ color:#999999; border:1px solid #BBBBBB; }";

///PushButton
    const QString MQSSPushButtonStyle01 = " \
            QPushButton{ background-color:transparent; " +
#ifdef Q_OS_UNIX
            QString("border-style:falt;") +
#endif
            QString("}");

    const QString MQSSPushButtonStyle02 = MQSSPushButtonStyle01 + " \
            QPushButton{ border-none; }";

    const QString MQSSPushButtonStyle03 = " \
            QPushButton{ border-radius:3px; background-color:#158FE1; color:white; } \
            QPushButton::disabled{ background-color:#BBBBBB; }";

    const QString MQSSPushButtonStyle04 = " \
            QPushButton{ border:1px solid #AAAAAA; background:#FFFFFF; color:#777777; } \
            QPushButton::hover{ border:1px solid #555555; color:#444444; } \
            QPushButton::disabled{ color:#BBBBBB; border:1px solid #DDDDDD; }";

    const QString MQSSPushButtonStyle05 = MQSSPushButtonStyle03 + " \
            QPushButton{ font-size:17px; font-weight:bold; }";

    const QString MQSSPushButtonStyle06 = " \
            QPushButton{ color:#CCCCCC; } \
            QPushButton:hover{ color:#FFFFFF; }";
           
    const QString MQSSPushButtonStyle07 = MQSSPushButtonStyle06 + " \
            QPushButton{ text-align:right; }";

    const QString MQSSPushButtonStyle08 = MQSSPushButtonStyle01 + " \
            QPushButton::hover{ border:1px solid #000000; }";

    const QString MQSSPushButtonStyle09 = " \
            QPushButton{ border-radius:5px; font-size:20px; \
            background:rgba(0, 0, 0, 50); } \
            QPushButton::hover{ background:rgba(0, 0, 0, 100); }";

    const QString MQSSPushButtonStyle10 = " \
            QPushButton{ border-radius:2px; background:rgba(0, 0, 0, 50); }";

    const QString MQSSPushButtonStyle11 = " \
            QPushButton::menu-indicator{ image:none; }";

    const QString MQSSPushButtonStyle12 = " \
            QPushButton{ color:#E6C117; } \
            QPushButton:hover{ color:#E6E317; } \
            QPushButton{ text-align:right; }";

    const QString MQSSPushButtonStyle13 = " \
            QPushButton{ background-color:#DDDDDD; color:#222222; }";

///RadioButton
    const QString MQSSRadioButtonStyle01 = " \
            QRadioButton{ background-color:transparent; } \
            QRadioButton::indicator::unchecked{ image:url(:/control/btn_radioButton_unchecked); } \
            QRadioButton::checked { color:#E67300; } \
            QRadioButton::indicator:checked { image:url(:/control/btn_radiobutton_checked); } \
            QRadioButton::disabled{ color:#BBBBBB; }";

///CheckBox
    const QString MQSSCheckBoxStyle01 = " \
            QCheckBox{ background-color:transparent; } \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_checkBox_unchecked); } \
            QCheckBox::checked{ color:#E67300; } \
            QCheckBox::indicator:checked { image:url(:/control/btn_checkBox_checked); } \
            QCheckBox::disabled{ color:#BBBBBB; }";

    const QString MQSSCheckBoxStyle02 = " \
            QCheckBox{ background-color:transparent; } \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_radioButton2_unchecked); } \
            QCheckBox::checked { color:#E67300; } \
            QCheckBox::indicator:checked { image:url(:/control/btn_radiobutton2_checked); } \
            QCheckBox::disabled{ color:#BBBBBB; }";

    const QString MQSSCheckBoxStyle03 = MQSSCheckBoxStyle01 + " \
            QCheckBox::checked{ color:rgb(0, 169, 236); }";

    const QString MQSSCheckBoxStyle04 = MQSSCheckBoxStyle01 + " \
            QCheckBox{ color:#888888; }";

    const QString MQSSCheckBoxStyle05 = " \
            QCheckBox{ background-color:transparent; color:rgb(222, 222, 222); } \
            QCheckBox::indicator::unchecked{ image:url(:/control/btn_checkBox2_unchecked); } \
            QCheckBox::checked { color:rgb(255, 255, 255); } \
            QCheckBox::indicator:checked { image:url(:/control/btn_checkBox2_checked); } \
            QCheckBox::disabled{ color:#BBBBBB; }";

///GroupBox
    const QString MQSSGroupBoxStyle01 = " \
            QGroupBox::indicator::unchecked{ image:url(:/control/btn_checkBox_unchecked); } \
            QGroupBox::indicator:checked { image:url(:/control/btn_checkBox_checked); }";

///ScrollBar
    const QString MQSSScrollBarStyle01 = " \
            QScrollBar{ background:#FFFFFF; width:8px; padding-top:0px; padding-bottom:0px; } \
            QScrollBar::handle:vertical{ border-radius:4px; background:#CFCFCF; min-height: 30px; } \
            QScrollBar::handle:vertical::disabled{ background:#DBDBDB; } \
            QScrollBar::handle:vertical:hover{ background:#BBBBBB; } \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none; } \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none; }";

    const QString MQSSScrollBarStyle02 = " \
            QScrollBar{ background:#FFFFFF; height:8px; padding-left:0px; padding-right:0px; } \
            QScrollBar::handle:horizontal{ border-radius:4px; background:#CFCFCF; min-width: 30px; } \
            QScrollBar::handle:horizontal::disabled{ background:#DBDBDB; } \
            QScrollBar::handle:horizontal:hover{ background:#BBBBBB; } \
            QScrollBar::add-line, QScrollBar::sub-line{ background:none; border:none; } \
            QScrollBar::add-page, QScrollBar::sub-page{ background:none; }";

    const QString MQSSScrollBarStyle03 = MQSSScrollBarStyle01 + " \
            QScrollBar{ background:transparent; }";

    const QString MQSSScrollBarStyle04 = MQSSScrollBarStyle02 + " \
            QScrollBar{ background:transparent; }";

///Label
    const QString MQSSLabelStyle01 = " \
            QLabel{ color:#888888; background-color:#FFE6E6; border:1px solid gray; }";

///LineEdit
    const QString MQSSLineEditStyle01 = " \
            QLineEdit{ font-size:12px; color:#666666; font-weight:normal; border:1px solid gray; } \
            QLineEdit::hover{ border:1px solid rgb(22, 154, 243); } \
            QLineEdit::disabled{ color:#BBBBBB; border:1px solid #BBBBBB; }";

    const QString MQSSLineEditStyle02 = " \
            QLineEdit{ border-image:url(:/tiny/lb_transparent); }";

    const QString MQSSLineEditStyle03 = MQSSLineEditStyle02 + " \
            QLineEdit{ font-size:13px; color:#333333; }";

    const QString MQSSLineEditStyle04 = " \
            QLineEdit{ color:#6495ED; border:1px solid #555555; }";

    const QString MQSSLineEditStyle05 = MQSSLineEditStyle01 + " \
            QLineEdit{ color:white; border:1px solid #545454; } \
            QLineEdit::disabled{ color:#323232; border:1px solid #323232; }";

///Menu
    const QString MQSSMenuStyle01 = " \
            QMenu{ padding:5px; } \
            QMenu::icon{ margin-left:10px; } \
            QMenu::right-arrow{ background: url(:/tiny/lb_arrow_up_normal); } \
            QMenu::item{ color:#666666; padding:6px 30px 6px 30px; border:1px solid transparent; } \
            QMenu::item:disabled{ color:#AAAAAA; } \
            QMenu::item:selected{ color:white; background: rgb(22, 154, 243); } \
            QMenu::separator{ height:1px; background:#DDDDDD; margin-top:5px; margin-bottom:5px; }" +
#if TTK_QT_VERSION_CHECK(5,12,0)
            QString("QMenu::item{ padding:6px 30px 6px 10px; } "
                    "QMenu::item::icon{ padding:6px 30px 6px 10px; }") +
#endif
            QString();

    const QString MQSSMenuStyle02 = MQSSMenuStyle01 + " \
            QMenu{ background:rgba(255, 255, 255, 235); }";

    const QString MQSSMenuStyle03 = MQSSMenuStyle01 + " \
            QMenu{ border:none; background:rgba(0, 0, 0, 210); border-radius:4px; } \
            QMenu::item{ color:#BBBBBB; } \
            QMenu::item:disabled{ color:#555555; } \
            QMenu::item:selected{ background:rgba(0, 0, 0, 200); } \
            QMenu::separator{ background:#BBBBBB; }";

    const QString MQSSMenuStyle04 = MQSSMenuStyle03 + " \
            QMenu{ background:rgba(0, 0, 0, 150); }";

    const QString MQSSMenuStyle05 = " \
            QMenu{ border:none; background:transparent; }";

///Slider
    const QString MQSSSliderStyle01 = " \
            QSlider::groove:horizontal{ \
            background:#FFFEA1; height:3px; border-radius:1px; } \
            QSlider::sub-page:horizontal{ background:#FFFEA1; } \
            QSlider::add-page:horizontal{ background:#4E4F4F; } \
            QSlider::handle:horizontal{ background:#FFFEA1; \
            width:9px; margin-top:-3px; margin-bottom:-3px; border-radius:4px; }";

    const QString MQSSSliderStyle02 = " \
            QSlider::groove:vertical{ \
            background: #FFFEA1; width:3px; border-radius:1px; } \
            QSlider::sub-page:vertical{ background:#4E4F4F; } \
            QSlider::add-page:vertical{ background:#FFFEA1; } \
            QSlider::handle:vertical{ background:#FFFEA1; \
            height:9px; margin-left:-3px; margin-right:-3px; border-radius:4px; }";

    const QString MQSSSliderStyle03 = " \
            QSlider::groove:vertical{ background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \
            stop:0 #B5B4B4, stop:1 #FCFCFC); width:3px; } \
            QSlider::handle:vertical{ border-image:url(:/control/lb_slider_handle_normal); \
            height:21px; margin-left:-8px; margin-right:-8px; } \
            QSlider::handle:vertical:disabled{ border-image:url(:/control/lb_slider_disable); } \
            QSlider::handle:vertical:hover{ border-image:url(:/control/lb_slider_handle_hover); border:none; }";

    const QString MQSSSliderStyle04 = MQSSSliderStyle03 + " \
            QSlider::groove:vertical{ background:white; } \
            QSlider::sub-page:vertical{ background:#AFC7F8; } \
            QSlider::add-page:vertical{ background:white; border:1px solid #ACF; }";

    const QString MQSSSliderStyle05 = " \
            QSlider::groove:horizontal{ background: qlineargradient(x1:0, y1:0, x2:0, y2:1, \
            stop:0 #B5B4B4, stop:1 #FCFCFC); height:4px; } \
            QSlider::handle:horizontal{ border-image:url(:/control/lb_slider_handle_normal); \
            width:21px; margin-top:-8px; margin-bottom:-8px; } \
            QSlider::handle:horizontal:disabled{ border-image:url(:/control/lb_slider_disable); } \
            QSlider::handle:horizontal:hover{ border-image:url(:/control/lb_slider_handle_hover); border:none; }";

    const QString MQSSSliderStyle06 = MQSSSliderStyle05 + " \
            QSlider::groove:horizontal{ background:white; } \
            QSlider::sub-page:horizontal{ background:#AFC7F8; } \
            QSlider::add-page:horizontal{ background:white; border:1px solid #ACF; }";

    const QString MQSSSliderStyle07 = " \
            QSlider::add-page:Horizontal{ background-color:rgba(120, 120, 120, 100); height:4px; } \
            QSlider::sub-page:Horizontal{ background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, \
            y2:0, stop:0 rgba(231, 80, 229, 255), stop:1 rgba(7, 208, 255, 255)); height:4px; } \
            QSlider::groove:Horizontal{ background:transparent; height:4px; } \
            QSlider::handle:Horizontal{ border-image:url(':/image/lb_slider'); \
            width:8px; height:6px; margin:-2px 0px -2px 0px; }";

    const QString MQSSSliderStyle08 = " \
            QSlider::groove:horizontal{ background-image:url(:/control/lb_volume_back); } \
            QSlider::sub-page:horizontal{ border-image:url(:/control/lb_volume_fore); } \
            QSlider::add-page:horizontal{ background-image:url(:/control/lb_volume_back); } \
            QSlider::handle:horizontal{ margin:0px -2px 0px -2px; \
            background-image:url(:/control/lb_volume_handle_normal); width:13px; } \
            QSlider::handle:horizontal:hover{ background-image:url(:/control/lb_volume_handle_hover); }";

    const QString MQSSSliderStyle09 = " \
            QSlider::groove:horizontal{ background-image:url(:/control/lb_volume_back); } \
            QSlider::sub-page:horizontal{ border-image:url(:/control/lb_make_fore); } \
            QSlider::add-page:horizontal{ border-image:url(:/control/lb_make_back); } \
            QSlider::handle:horizontal{ margin:0px -1px 0px -1x; \
            border-image:url(:/control/lb_make_slider_handle_normal); width:18px; } \
            QSlider::handle:horizontal:hover{ border-image:url(:/control/lb_make_slider_handle_hover); } \
            QSlider::handle:horizontal:disabled{ border-image:url(:/control/lb_make_slider_disable); }";

    const QString MQSSSliderStyle10 = " \
            QSlider::groove:horizontal{ \
            background:transparent; height:3px; border-radius:1px; } \
            QSlider::sub-page:horizontal{ background:rgba(255, 255, 255, 190); } \
            QSlider::add-page:horizontal{ background:rgba(255, 255, 255, 50); } \
            QSlider::handle:horizontal{ background:rgba(255, 255, 255, 255); \
            width:9px; margin-top:-3px; margin-bottom:-3px; border-radius:4px; }";

///ComboBox
    const QString MQSSComboBoxStyle01 = " \
            QComboBox{ border: 1px solid gray; color:#666666; } \
            QComboBox::hover{ border: 1px solid rgb(22, 154, 243); } \
            QComboBox::drop-down{ subcontrol-origin:padding; \
            subcontrol-position:top right; width:20px; border-left:none; } \
            QComboBox::down-arrow{ image:url(:/image/btn_down_arrow); } \
            QComboBox::disabled{ border: 1px solid #BBBBBB; }";

    const QString MQSSComboBoxStyle02 = MQSSComboBoxStyle01 + " \
            QComboBox{ border:none; } \
            QComboBox::hover{ border:none; }";

///TableWidget
    const QString MQSSTableWidgetStyle01 = " \
            QTableWidget{ selection-background-color:rgba(20, 20, 20, 20); }";

    const QString MQSSTableWidgetStyle02 = " \
            QTableWidget{ selection-background-color:rgb(220, 220, 220); }";

    const QString MQSSTableWidgetStyle03 = " \
            QTableWidget{ selection-background-color:rgb(50, 50, 50); }";

    const QString MQSSTableWidgetStyle04 = " \
            QTableWidget{ background:rgba(0, 0, 0, 255); }";

///TabWidget
    const QString MQSSTabWidgetStyle01 = " \
            QTabWidget{ border:none; } \
            QTabWidget::pane{ border:none; } \
            QTabBar::tab{ height:30px; width:100px; background-color:#BFBFBF; color:white; margin-left:0; margin-right:0; } \
            QTabBar::tab:selected{ background-color:#158FE1; color:white}";

///TextEdit
    const QString MQSSTextEditStyle01 = " \
            QTextEdit{ border:1px solid gray; color:#666666; } \
            QTextEdit::hover{ border:1px solid rgb(22, 154, 243); } \
            QTextEdit::disabled{ color:#BBBBBB; border: 1px solid #BBBBBB; }";

///HeadView
    const QString MQSSHeaderView01 = " \
            QHeaderView{ background-color:white; color:black; }";

///Widget
    const QString MQSSWidgetStyle01 = " \
            QWidget{ background:rgba(255, 255, 255, 50); color:white; font-weight:bold; }";

///SpinBox
    const QString MQSSSpinBoxStyle01 = " \
            QSpinBox{ font-size:12px; color:#666666; font-weight:normal; border: 1px solid gray; } \
            QSpinBoxt::hover{ border:1px solid rgb(22, 154, 243); } \
            QSpinBox::up-button{ image:url(:/image/btn_up_arrow); } \
            QSpinBox::down-button{ image:url(:/image/btn_down_arrow); } \
            QSpinBox::disabled{ color:#BBBBBB; border: 1px solid #BBBBBB; }";

///ProgressBar
    const QString MQSSProgressBar01 = " \
            QProgressBar{ border:none; background:rgb(210, 225, 240); text-align:center; } \
            QProgressBar::chunk{ background:rgb(60, 140, 220); }";

///ItemView
    const QString MQSSItemView01 = " \
            QAbstractItemView::item { height:20px; }";

}

#endif // MUSICUIOBJECT_H
