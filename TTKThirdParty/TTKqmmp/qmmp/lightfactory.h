/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#ifndef LIGHTFACTORY_H
#define LIGHTFACTORY_H

#include <QWidget>
#include "qmmp_export.h"

/*! @brief Helper class to light widget.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT Light : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     */
    explicit Light(QWidget *parent = nullptr)
        : QWidget(parent)
    {

    }
    /*!
     * Destructor.
     */
    virtual ~Light() = default;

     /*!
     * Open Light file path.
     * Subclass should reimplement this function.
     */
    virtual void open(const QString &path) = 0;
    /*!
     * Start Light plugin.
     * Subclass should reimplement this function.
     */
    virtual void start() = 0;
    /*!
     * Stop Light plugin.
     * Subclass should reimplement this function.
     */
    virtual void stop() = 0;

};

/*! @brief Structure to store Light plugin properies.
 * @author Greedysky <greedysky@163.com>
 */
struct QMMP_EXPORT LightProperties
{
    QString name;              /*!< Light plugin full name */
    QString shortName;         /*!< Light plugin short name for internal usage */
    bool hasSettings = false;  /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
};

/*! @brief Light plugin interface (Light factory).
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT LightFactory
{
public:
    /*!
     * Destructor.
     */
    virtual ~LightFactory() = default;

     /*!
     * Returns Light plugin properties.
     */
    virtual const LightProperties properties() const = 0;
    /*!
     * Creates Light provided by plugin.
     * @param parent Parent object.
     */
    virtual Light *create(QWidget *parent) = 0;

};

Q_DECLARE_INTERFACE(LightFactory, "LightFactory/1.0")

#endif
