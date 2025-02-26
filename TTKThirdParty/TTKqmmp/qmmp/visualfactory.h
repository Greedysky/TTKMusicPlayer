/***************************************************************************
 *   Copyright (C) 2008-2025 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
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

#ifndef VISUALFACTORY_H
#define VISUALFACTORY_H

#include <QObject>
#include "qmmp_export.h"

class Visual;

/*! @brief Structure to store visual plugin properies.
 *  @author Ilya Kotov <forkotov02@ya.ru>
 */
struct QMMP_EXPORT VisualProperties
{
    QString name;              /*!< Effect plugin full name */
    QString shortName;         /*!< Effect plugin short name for internal usage */
    bool hasSettings = false;  /*!< Should be \b true if plugin has settings dialog, and \b false otherwise */
};

/*! @brief %Visual plugin interface (visual factory).
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT VisualFactory
{
public:
    /*!
     * Destructor.
     */
    virtual ~VisualFactory() = default;

     /*!
     * Returns visual plugin properties.
     */
    virtual VisualProperties properties() const = 0;
    /*!
     * Creates visualization provided by plugin.
     * @param parent Parent object.
     */
    virtual Visual *create(QWidget *parent) = 0;

};

Q_DECLARE_INTERFACE(VisualFactory, "VisualFactory/1.0")

#endif
