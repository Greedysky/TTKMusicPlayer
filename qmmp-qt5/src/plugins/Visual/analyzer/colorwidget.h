/***************************************************************************
 *   Copyright (C) 2005-2014 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QFrame>
#include <QPaintEvent>

/**
@author Ilya Kotov
*/
class ColorWidget : public QFrame
{
    Q_OBJECT
public:
    ColorWidget(QWidget *parent = 0);

    ~ColorWidget();

    const QString colorName() const;

public slots:
    void setColor (QString);

private:
    void mousePressEvent(QMouseEvent *);
    QString m_colorName;


};

#endif
