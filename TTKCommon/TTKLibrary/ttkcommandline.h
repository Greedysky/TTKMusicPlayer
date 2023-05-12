#ifndef TTKCOMMANDLINE_H
#define TTKCOMMANDLINE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include <QCoreApplication>
#include "ttkglobaldefine.h"

/*! @brief The class of the command line option.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKCommandLineOption
{
public:
    /*!
     * Object contsructor.
     */
    TTKCommandLineOption(const QString &first)
        : m_first(first)
    {

    }

    TTKCommandLineOption(const QString &first, const QString &description)
        : m_first(first),
          m_description(description)
    {

    }

    TTKCommandLineOption(const QString &first, const QString &second, const QString &description)
        : m_first(first),
          m_second(second),
          m_description(description)
    {

    }

    /*!
     * Get first option.
     */
    inline QString first() const { return m_first; }
    /*!
     * Get second option.
     */
    inline QString second() const { return m_second; }
    /*!
     * Get option description.
     */
    inline QString description() const { return m_description; }

private:
    QString m_first;
    QString m_second;
    QString m_description;

};


/*! @brief The class of the command line parser.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKCommandLineParser
{
    TTK_DECLARE_MODULE(TTKCommandLineParser)
public:
    /*!
     * Object contsructor.
     */
    TTKCommandLineParser() = default;

    /*!
     * Add command line option.
     */
    void addOption(const TTKCommandLineOption &option);

    /*!
     * Process command line by input.
     */
    void process(const QStringList &arguments);
    /*!
     * Process command line by input.
     */
    void process(const QCoreApplication &app);

    /*!
     * Check current option is set.
     */
    bool isSet(const TTKCommandLineOption &option) const;
    /*!
     * Get command line option value.
     */
    QString value(const TTKCommandLineOption &option) const;

    /*!
     * Current input command is empty.
     */
    inline bool isEmpty() const { return m_commands.isEmpty(); }

    /*!
     * Print help text.
     */
    void printHelp() const;

private:
    QHash<QString, QString> m_commands;
    QList<TTKCommandLineOption> m_options;

};

#endif // TTKCOMMANDLINE_H
