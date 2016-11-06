/***************************************************************************
 *   Copyright (C) 2009 by Ilya Kotov                                      *
 *   forkotov02@hotmail.ru                                                 *
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

#include <QHash>
#include <QList>
#include <QString>
#include <QObject>
#include <QPixmap>
#include "tagmodel.h"

/*! @brief The MetaDataModel is the base interface class of metadata access.
 * @author Ilya Kotov <forkotov02@hotmail.ru>
 */
class Q_DECL_EXPORT MetaDataModel : public QObject
{
Q_OBJECT
public:
    /*!
     * Constructor.
     * @param parent Parent Object.
     */
    MetaDataModel(QObject *parent = 0);
    /*!
     * Destructor.
     */
    virtual ~MetaDataModel();
    /*!
     * Returns an associative array of the audio properties.
     * Subclass should reimplement this function. Default implementation returns empty array.
     */
    virtual QHash<QString, QString> audioProperties();
    /*!
     * Returns an associative array of the long descriptions.
     * Subclass should reimplement this function. Default implementation returns empty array.
     */
    virtual QHash<QString, QString> descriptions();
    /*!
     * Returns a list of available tags.
     * Subclass should reimplement this function. Default implementation returns empty array.
     */
    virtual QList<TagModel* > tags();
    /*!
     * Returns cover pixmap.
     * Subclass should reimplement this function. Default implementation returns empty pixmap.
     */
    virtual QPixmap cover();
    /*!
     * Returns path to cover pixmap.
     */
    virtual QString coverPath();
};

#endif // METADATAMODEL_H
