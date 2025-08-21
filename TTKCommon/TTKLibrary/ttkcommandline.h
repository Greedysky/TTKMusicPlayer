#ifndef TTKCOMMANDLINE_H
#define TTKCOMMANDLINE_H

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

#include <QCoreApplication>
#include "ttkmoduleexport.h"

/*! @brief The class of the command line option.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKCommandLineOption
{
public:
    /*!
     * Object constructor.
     */
    TTKCommandLineOption(const QString &first) noexcept
        : m_first(first)
    {

    }

    TTKCommandLineOption(const QString &first, const QString &description) noexcept
        : m_first(first),
          m_description(description)
    {

    }

    TTKCommandLineOption(const QString &first, const QString &second, const QString &description) noexcept
        : m_first(first),
          m_second(second),
          m_description(description)
    {

    }

    /*!
     * Get first option.
     */
    inline QString first() const noexcept { return m_first; }
    /*!
     * Get second option.
     */
    inline QString second() const noexcept { return m_second; }
    /*!
     * Get option description.
     */
    inline QString description() const noexcept { return m_description; }

    inline bool operator== (const TTKCommandLineOption &other) const noexcept
    {
        return m_first == other.m_first && m_second == other.m_second;
    }
    inline bool operator!= (const TTKCommandLineOption &other) const noexcept
    {
        return m_first != other.m_first || m_second != other.m_second;
    }
    
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
public:
    /*!
     * Object constructor.
     */
    TTKCommandLineParser() = default;

    /*!
     * Set application description.
     */
    void setDescription(const QString &description) noexcept;

    /*!
     * Add command line option.
     */
    bool addOption(const TTKCommandLineOption &option);
    /*!
     * Add command line options.
     */
    bool addOptions(const QList<TTKCommandLineOption> &options);

    /*!
     * Process command line by input.
     */
    void process();
    /*!
     * Process command line by input.
     */
    void process(const QStringList &arguments);

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
    inline bool isEmpty() const noexcept { return m_commands.isEmpty(); }

    /*!
     * Show help text.
     */
    void showHelp() const;

private:
    QString m_description;
    QHash<QString, QString> m_commands;
    QList<TTKCommandLineOption> m_options;

};

#endif // TTKCOMMANDLINE_H
