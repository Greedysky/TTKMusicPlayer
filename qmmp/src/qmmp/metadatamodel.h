/***************************************************************************
 *   Copyright (C) 2009-2025 by Ilya Kotov                                 *
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

#ifndef METADATAMODEL_H
#define METADATAMODEL_H

#include <QCoreApplication>
#include <QPixmap>
#include <QVariant>
#include <QFlags>
#include "tagmodel.h"
#include "regularexpression.h"

/*! @brief Container of extra file/track property.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT MetaDataItem
{
public:
    /*!
     * Constructor
     * \param name Localized name of property.
     * \param value Property value.
     * \param suffix Localized suffix of property (i.e. kbit, kbps, etc).
     */
    MetaDataItem(const QString &name, const QVariant &value, const QString &suffix = QString());
    /*!
     * Returns localized name of property.
     */
    const QString &name() const;
    /*!
     * Changes localized name to \b name
     */
    void setName(const QString &name);
    /*!
     * Returns property value.
     */
    const QVariant &value() const;
    /*!
     * Returns value of property.
     */
    void setValue(const QString &value);
    /*!
     * Returns suffix of property.
     */
    const QString &suffix() const;
    /*!
     * Changes property suffix to \b suffixed
     */
    void setSuffix(const QString &suffix);

private:
    QString m_name, m_suffix;
    QVariant m_value;

};

/*! @brief The MetaDataModel is the base interface class of metadata access.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT MetaDataModel
{
public:
    /*!
     * Details dialog settings.
     */
    enum DialogHint
    {
        IsCoverEditable = 0x1,      /*!< Enable cover editor. */
        CompletePropertyList = 0x2, /*!< Show properties from \b extraProperties() only (ignore other sources) */
        IsCueEditable = 0x4         /*!< Enable CUE editor. */
    };
    Q_DECLARE_FLAGS(DialogHints, DialogHint)

    /*!
     * Constructor.
     * @param readOnly Open file in read-only mode (\b true - enabled, \b false - disable).
     * @param hints Details dialog settings.
     */
    explicit MetaDataModel(bool readOnly, DialogHints hints = DialogHints());
    /*!
     * Destructor.
     */
    virtual ~MetaDataModel() = default;

    /*!
     * Returns extra properties of the media source (in addition to the \b Qmmp::TrackProperty values).
     * Default implemetation returns empty array.
     */
    virtual QList<MetaDataItem> extraProperties() const;
    /*!
     * Returns a list of long descriptions.
     * Default implemetation returns empty array.
     */
    virtual QList<MetaDataItem> descriptions() const;
    /*!
     * Returns a list of available tags.
     * Subclass should reimplement this function. Default implementation returns empty array.
     */
    virtual QList<TagModel*> tags() const;
    /*!
     * Returns cover pixmap.
     * Subclass should reimplement this function. Default implementation returns empty pixmap.
     */
    virtual QPixmap cover() const;
    /*!
     * Sets cover.
     * @param pix Cover pixmap.
     * Subclass should reimplement this function. Default implementation does nothing.
     */
    virtual void setCover(const QPixmap &pix);
    /*!
     * Removes cover.
     * Subclass should reimplement this function. Default implementation does nothing.
     */
    virtual void removeCover();
    /*!
     * Returns path to cover pixmap.
     */
    virtual QString coverPath() const;
    /*!
     * Returns CUE file or tag content if necessary. Default implementation returns empty string.
     */
    virtual QString cue() const;
    /*!
     * Updates CUE file or tag content. Default implementation doesn nothing.
     */
    virtual void setCue(const QString &content);
    /*!
     * Removes CUE file or tag. Default implementation does nothing.
     */
    virtual void removeCue();
    /*!
     * Returns song lyrics. Default returns empty string.
     */
    virtual QString lyrics() const;
    /*!
     * Remove cue data.
     */
    bool isReadOnly() const;
    /*!
     * Returns details dialog hints.
     */
    const DialogHints &dialogHints() const;

protected:
    /*!
     * Changes details dialog hints to \b hints
     */
    void setDialogHints(const DialogHints &hints);
    /*!
     * Enables/Disables read only mode (\b true - enabled, \b false - disable).
     * @param readOnly read only mode (\b true - enabled, \b false - disable).
     */
    void setReadOnly(bool readOnly);

private:
    bool m_readOnly;
    DialogHints m_dialogHints;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(MetaDataModel::DialogHints)

#endif
