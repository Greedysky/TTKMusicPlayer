/***************************************************************************
 *   Copyright (C) 2007-2019 by Ilya Kotov                                 *
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

#ifndef OUTPUTFACTORY_H
#define OUTPUTFACTORY_H

#include "qmmp_export.h"

class QObject;
class QString;
class QIODevice;
class QWidget;
class Volume;
class Decoder;
class Output;

/*! @brief Helper class to store output plugin properies.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT OutputProperties
{
public:
    /*!
     * Constructor
     */
    OutputProperties()
    {
        hasSettings = false;
    }
    QString name;        /*!< Effect plugin full name */
    QString shortName;   /*!< Effect plugin short name for internal usage */
    bool hasSettings;    /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
};
/*! @brief %Output plugin interface (output factory).
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT OutputFactory
{
public:
    /*!
     * Destructor.
     */
    virtual ~OutputFactory() {}
    /*!
     * Returns output plugin properties.
     */
    virtual OutputProperties properties() const = 0;
    /*!
     * Creates output provided by plugin.
     */
    virtual Output *create() = 0;
    /*!
     * Creates volume control object provided by plugin.
     * Returns \b 0 if volume control is not supported by plugin.
     */
    virtual Volume *createVolume() = 0;

};

Q_DECLARE_INTERFACE(OutputFactory, "OutputFactory/1.0")

#endif
