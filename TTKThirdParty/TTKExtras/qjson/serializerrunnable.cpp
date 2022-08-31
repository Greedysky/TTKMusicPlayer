#include "serializerrunnable.h"

/* This file is part of qjson
 *
 * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
 *               2009 Frank Osterfeld <osterfeld@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software Foundation.
 * 
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "parserrunnable.h"
#include "serializer.h"

#include <QtCore/QDebug>
#include <QtCore/QVariant>

using namespace QJson;

/**
* @brief Convenience class for converting JSON data to QVariant objects using a dedicated thread private
*/
class SerializerRunnable::SerializerRunnablePrivate : public TTKPrivate<SerializerRunnable>
{
public:
  QVariant json;
};

SerializerRunnable::SerializerRunnable(QObject* parent)
    : QObject(parent),
      QRunnable()
{
  TTK_INIT_PRIVATE(SerializerRunnable);
  qRegisterMetaType<QVariant>("QVariant");
}

void SerializerRunnable::setJsonObject(const QVariant &json)
{
  TTK_D(SerializerRunnable);
  d->json = json;
}

void SerializerRunnable::run()
{
  TTK_D(SerializerRunnable);
  Serializer serializer;
  bool ok;
  const QByteArray serialized = serializer.serialize( d->json, &ok);
  Q_EMIT parsingFinished( serialized, ok, serializer.errorMessage() );
}
