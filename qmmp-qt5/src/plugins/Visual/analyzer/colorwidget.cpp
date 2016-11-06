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

#include <QColorDialog>
#include <QPalette>
#include "colorwidget.h"

ColorWidget::ColorWidget(QWidget *parent) : QFrame(parent)
{
    setFrameShape(QFrame::Box);
    setAutoFillBackground(true);
}

ColorWidget::~ColorWidget()
{}

void ColorWidget::mousePressEvent(QMouseEvent *)
{
    QColor color = QColorDialog::getColor(palette().color(backgroundRole()), parentWidget(),
                                          tr("Select Color"));
    if (color.isValid())
    {
        setColor(color.name());
    }
}

void ColorWidget::setColor(QString c)
{
    m_colorName = c;
    setStyleSheet(QString("QFrame { background: %1 }").arg(m_colorName));
}

const QString ColorWidget::colorName() const
{
    return m_colorName;
}
