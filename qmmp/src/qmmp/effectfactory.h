/***************************************************************************
 *   Copyright (C) 2007-2024 by Ilya Kotov                                 *
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

#ifndef EFFECTFACTORY_H
#define EFFECTFACTORY_H

#include <QObject>
#include "qmmp_export.h"

class QDialog;
class Effect;

/*! @brief Structure to store effect plugin properties.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
struct QMMP_EXPORT EffectProperties
{
    /*!
     * This enum describes effect preferd position.
     */
    enum Priority
    {
        EFFECT_PRIORITY_LOW = 0,    /*!< Low priority */
        EFFECT_PRIORITY_DEFAULT,    /*!< Default priority */
        EFFECT_PRIORITY_HIGH        /*!< High priority */
    };

    QString name;               /*!< Effect plugin full name */
    QString shortName;          /*!< Effect plugin short name for internal usage */
    bool hasSettings = false;   /*!< Should be \b true if plugin has settings dialog,and \b false otherwise */
    int priority = EFFECT_PRIORITY_DEFAULT;  /*!< Effect priority. Effecs with higher priority will be applied first */
};

/*! @brief Effect plugin interface (effect factory).
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT EffectFactory
{
public:
    /*!
    * Object destructor.
    */
    virtual ~EffectFactory() = default;

    /*!
     * Returns effect plugin properties.
     */
    virtual EffectProperties properties() const = 0;
    /*!
     * Creates effect provided by plugin.
     */
    virtual Effect *create() = 0;
    /*!
     * Creates settings dialog.
     * @param parent Parent widget.
     * @return Settings dialog pointer.
     */
    virtual QDialog *createSettings(QWidget *parent) = 0;

};

Q_DECLARE_INTERFACE(EffectFactory, "EffectFactory/1.0")

#endif
