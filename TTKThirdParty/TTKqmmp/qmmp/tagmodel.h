/***************************************************************************
 *   Copyright (C) 2009-2024 by Ilya Kotov                                 *
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

#ifndef TAGMODEL_H
#define TAGMODEL_H

#include <QList>
#include "qmmp.h"

/*! @brief The StateHandler class provides is the base interface class of tag editor.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT TagModel
{
public:
    /*!
     * This enum describes tag editor capabilities
     */
    enum ModelCap
    {
        CreateRemove = 0x1, /*!< Can create/remove tag */
        Save = 0x2,         /*!< Can save changes */
        DefaultCaps = CreateRemove | Save,
    };
    Q_DECLARE_FLAGS(ModelCaps, ModelCap)
    /*!
     * Constructor.
     * @param f Capabilities.
     */
    explicit TagModel(ModelCaps f = DefaultCaps);
    /*!
     * Destructor.
     */
    virtual ~TagModel();
    /*!
     * Returns tag name.
     * Subclass should reimplement this fucntion.
     */
    virtual QString name() const = 0;
    /*!
     * Returns available keys. Default implementations returns all possible keys.
     */
    virtual QList<Qmmp::MetaData> keys() const;
    /*!
     * Returns the metdata string associated with the given \b key.
     * Subclass should reimplement this fucntion.
     */
    virtual QString value(Qmmp::MetaData key) const = 0;
    /*!
     * Changes metadata string associated with the given \b key to \b value.
     * Subclass should reimplement this fucntion.
     */
    virtual void setValue(Qmmp::MetaData key, const QString &value) = 0;
    /*!
     * Changes metadata string associated with the given \b key to \b value.
     * Subclass should reimplement this fucntion.
     */
    virtual void setValue(Qmmp::MetaData key, int value);
    /*!
     * Returns \b true if this tag exists; otherwise returns \b false.
     */
    virtual bool exists() const;
    /*!
     * Creates tag.
     */
    virtual void create();
    /*!
     * Removes tag.
     */
    virtual void remove();
    /*!
     * Saves tag.
     */
    virtual void save();
    /*!
     * Returns capability flags.
     */
    ModelCaps caps() const;

private:
    ModelCaps m_f = DefaultCaps;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(TagModel::ModelCaps)

#endif
