#ifndef TTKQTWRAPPER_H
#define TTKQTWRAPPER_H

/***************************************************************************
 * This file is part of the TTK Library Module project
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

/*! @brief The class of the qt deprecated function wrapper.
 * @author Greedysky <greedysky@163.com>
 */
namespace Qt
{
/// Font string width
#if TTK_QT_VERSION_CHECK(5,11,0)
#  define QtFontWidth(p, t) p.horizontalAdvance(t)
#else
#  define QtFontWidth(p, t) p.width(t)
#endif

/// Font init
#if TTK_QT_VERSION_CHECK(6,2,0)
#  define QtFontInit_1(f) QFont(QStringList() << f)
#  define QtFontInit_2(f, p) QFont(QStringList() << f, p)
#  define QtFontInit_3(f, p, w) QFont(QStringList() << f, p, w)
#else
#  define QtFontInit_1(f) QFont(f)
#  define QtFontInit_2(f, p) QFont(f, p)
#  define QtFontInit_3(f, p, w) QFont(f, p, w)
#endif

#ifndef Q_CC_MSVC
#  define QtFontInit(...) TTK_PP_OVERLOAD(QtFontInit_, __VA_ARGS__)(__VA_ARGS__)
#else
#  define QtFontInit(...) TTK_PP_CAT(TTK_PP_OVERLOAD(QtFontInit_, __VA_ARGS__)(__VA_ARGS__), TTK_PP_EMPTY())
#endif

/// Image byte count
#if TTK_QT_VERSION_CHECK(5,10,0)
#  define QtImageBytes(p) p.sizeInBytes()
#else
#  define QtImageBytes(p) p.byteCount()
#endif

/// Item foreground color
#if TTK_QT_VERSION_CHECK(5,13,0)
#  define QtItemSetForegroundColor(p, c) p->setForeground(c)
#else
#  define QtItemSetForegroundColor(p, c) p->setTextColor(c)
#endif

#if TTK_QT_VERSION_CHECK(5,13,0)
#  define QtItemForegroundColor(p) p->foreground().color()
#else
#  define QtItemForegroundColor(p) p->textColor()
#endif

/// Item background color
#if TTK_QT_VERSION_CHECK(5,13,0)
#  define QtItemSetBackgroundColor(p, c) p->setBackground(c)
#else
#  define QtItemSetBackgroundColor(p, c) p->setBackgroundColor(c)
#endif

#if TTK_QT_VERSION_CHECK(5,13,0)
#  define QtItemBackgroundColor(p) p->background().color()
#else
#  define QtItemBackgroundColor(p) p->backgroundColor()
#endif

/// Item text alignment
#if TTK_QT_VERSION_CHECK(6,4,0)
#  define QtItemSetTextAlignment(p, a) p->setTextAlignment(Qt::Alignment(a));
#else
#  define QtItemSetTextAlignment(p, a) p->setTextAlignment(a);
#endif

/// Painter
#if TTK_QT_VERSION_CHECK(5,13,0)
#  define QtDrawRoundedRect(p, r, x, y) (p)->drawRoundedRect(r, x, y)
#else
#  define QtDrawRoundedRect(p, r, x, y) (p)->drawRoundRect(r, x, y)
#endif

/// Container swap
#if TTK_QT_VERSION_CHECK(5,13,0)
#  define QtContainerSwap(p, i, j) (p)->swapItemsAt(i, j)
#else
#  define QtContainerSwap(p, i, j) (p)->swap(i, j)
#endif

/// Network error none connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtNetworkErrorNoneConnect(p, q, f) QObject::connect(p, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), q, SLOT(f()))
#else
#  define QtNetworkErrorNoneConnect(p, q, f) QObject::connect(p, SIGNAL(error(QNetworkReply::NetworkError)), q, SLOT(f()))
#endif

/// Network error connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtNetworkErrorConnect(p, q, f) QObject::connect(p, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), q, SLOT(f(QNetworkReply::NetworkError)))
#else
#  define QtNetworkErrorConnect(p, q, f) QObject::connect(p, SIGNAL(error(QNetworkReply::NetworkError)), q, SLOT(f(QNetworkReply::NetworkError)))
#endif

/// Socket error none connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtSocketErrorNoneConnect(p, q, f) QObject::connect(p, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), q, SLOT(f()))
#else
#  define QtSocketErrorNoneConnect(p, q, f) QObject::connect(p, SIGNAL(error(QAbstractSocket::SocketError)), q, SLOT(f()))
#endif

/// Socket error connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtSocketErrorConnect(p, q, f) QObject::connect(p, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), q, SLOT(f(QAbstractSocket::SocketError)))
#else
#  define QtSocketErrorConnect(p, q, f) QObject::connect(p, SIGNAL(error(QAbstractSocket::SocketError)), q, SLOT(f(QAbstractSocket::SocketError)))
#endif

/// Process error none connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtProcessNoneConnect(p, q, f) QObject::connect(p, SIGNAL(errorOccurred(QProcess::ProcessError)), q, SLOT(f()))
#else
#  define QtProcessNoneConnect(p, q, f) QObject::connect(p, SIGNAL(error(QProcess::ProcessError)), q, SLOT(f()))
#endif

/// Process error connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtProcessConnect(p, q, f) QObject::connect(p, SIGNAL(errorOccurred(QProcess::ProcessError)), q, SLOT(f(QProcess::ProcessError)))
#else
#  define QtProcessConnect(p, q, f) QObject::connect(p, SIGNAL(error(QProcess::ProcessError)), q, SLOT(f(QProcess::ProcessError)))
#endif

/// Button group none connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtButtonGroupNoneConnect(p, q, f) QObject::connect(p, SIGNAL(idClicked(int)), SLOT(f()))
#else
#  define QtButtonGroupNoneConnect(p, q, f) QObject::connect(p, SIGNAL(buttonClicked(int)), q, SLOT(f()))
#endif

/// Button group connect
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtButtonGroupConnect(p, q, f) QObject::connect(p, SIGNAL(idClicked(int)), SLOT(f(int)))
#else
#  define QtButtonGroupConnect(p, q, f) QObject::connect(p, SIGNAL(buttonClicked(int)), q, SLOT(f(int)))
#endif

/// Stream endl
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtStreamEndLine(p) p << Qt::endl
#else
#  define QtStreamEndLine(p) p << endl
#endif

/// Lable pixmap
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtLablePixmap(p) p->pixmap(Qt::ReturnByValue)
#else
#  define QtLablePixmap(p) p->pixmap() ? *p->pixmap() : QPixmap();
#endif

/// String skip empty parts
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtSkipEmptyParts Qt::SkipEmptyParts
#else
#  define QtSkipEmptyParts QString::SkipEmptyParts
#endif

/// Event wheel
#if TTK_QT_VERSION_CHECK(5,15,0)
#  define QtWheelEventDelta(p) p->angleDelta().y()
#else
#  define QtWheelEventDelta(p) p->delta()
#endif

/// Event mouse
#if TTK_QT_VERSION_CHECK(6,0,0)
#  define QtMouseEventX(p) p->position().x()
#  define QtMouseEventY(p) p->position().y()
#  define QtMouseEventPos(p) p->position()
#  define QtMouseEventGlobalX(p) p->globalPosition().x()
#  define QtMouseEventGlobalY(p) p->globalPosition().y()
#  define QtMouseEventGlobalPos(p) p->globalPosition().toPoint()
#else
#  define QtMouseEventX(p) p->x()
#  define QtMouseEventY(p) p->y()
#  define QtMouseEventPos(p) p->pos()
#  define QtMouseEventGlobalX(p) p->globalX()
#  define QtMouseEventGlobalY(p) p->globalY()
#  define QtMouseEventGlobalPos(p) p->globalPos()
#endif

/// Event enter
#if TTK_QT_VERSION_CHECK(6,0,0)
#  define QtEnterEvent QEnterEvent
#else
#  define QtEnterEvent QEvent
#endif

/// Variant type
#if TTK_QT_VERSION_CHECK(6,0,0)
#  define QtVariantType(p) p.typeId()
#else
#  define QtVariantType(p) p.type()
#endif
}

#endif // TTKQTWRAPPER_H
