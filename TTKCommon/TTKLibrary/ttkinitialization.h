#ifndef TTKINITIALIZATION_H
#define TTKINITIALIZATION_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkobject.h"
#include "ttkmoduleexport.h"

/*! @brief The namespace of the ttk initialization.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum Attribute
    {
        UseLog = 0x01,      /*!< Use log option */
        ScaleFactor = 0x02, /*!< Use scale factor option */
        UseXCB = 0x04,      /*!< Use xcb (x11 only) option */
        All = 0x07,         /*!< Use all option */
    };

    /*!
     * Initialization before qApp.
     */
    TTK_MODULE_EXPORT void initialize(TTK::Attribute attr);
}

#define TTKRegisterOrganization(ORG) \
    QCoreApplication::setOrganizationName(ORG ## _NAME); \
    QCoreApplication::setOrganizationDomain(ORG ## _COME_NAME); \
    QCoreApplication::setApplicationName(ORG ## _NAME);

class TTKDumper;
typedef void (*TTKCleanFunctor)(void);

/*! @brief The class of the ttk initialization.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKInitialization
{
public:
    /*!
     * Object constructor.
     */
    TTKInitialization();
    TTKInitialization(const TTKCleanFunctor &functor);
    /*!
     * Object destructor.
     */
    ~TTKInitialization();

    /*!
     * Generate application font.
     */
    void generateFont();
    /*!
     * Set codec for locale.
     */
    void codecForLocale();

    /*!
     * Execute module.
     */
    void execute();
    /*!
     * Execute module.
     */
    void execute(const QStringList &arguments);

private:
    TTKDumper *m_dumper;

};

#endif // TTKINITIALIZATION_H
