/* =================================================
 * This file is part of the TTK Music Player qmmp plugin project
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

#ifndef SPEKFACTORY_H
#define SPEKFACTORY_H

#include <QWidget>
#include "qmmp_export.h"

/*! @brief Helper class to spek widget.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT Spek : public QWidget
{
public:
    /*!
     * Constructor.
     */
    explicit Spek(QWidget *parent = 0) : QWidget(parent) {}
    /*!
     * Destructor.
     */
    virtual ~Spek() {}
     /*!
     * Open spek file path.
     * Subclass should reimplement this function.
     */
    virtual void open(const QString &path) = 0;
    /*!
     * Start spek plugin.
     * Subclass should reimplement this function.
     */
    virtual void start() = 0;
    /*!
     * Stop spek plugin.
     * Subclass should reimplement this function.
     */
    virtual void stop() = 0;

};

/*! @brief Helper class to store spek plugin properies.
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT SpekProperties
{
public:
    /*!
     * Constructor
     */
    SpekProperties()
    {
        hasSettings = false;
    }
    QString name;      /*!< Spek plugin full name */
    QString shortName; /*!< Spek plugin short name for internal usage */
    bool hasSettings;  /*!< Should be \b true if plugin has settings dialog, otherwise returns \b false */
};
/*! @brief Spek plugin interface (Spek factory).
 * @author Greedysky <greedysky@163.com>
 */
class QMMP_EXPORT SpekFactory
{
public:
    /*!
     * Destructor.
     */
    virtual ~SpekFactory() {}
     /*!
     * Returns Spek plugin properties.
     */
    virtual const SpekProperties properties() const = 0;
    /*!
     * Creates Spek provided by plugin.
     * @param parent Parent object.
     */
    virtual Spek *create(QWidget *parent) = 0;

};

Q_DECLARE_INTERFACE(SpekFactory, "SpekFactory/1.0")

#endif
