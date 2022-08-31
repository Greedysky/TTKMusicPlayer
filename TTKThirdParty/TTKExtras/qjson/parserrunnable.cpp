/* This file is part of qjson
  *
  * Copyright (C) 2009 Flavio Castelli <flavio@castelli.name>
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
#include "parser.h"

#include <QtCore/QDebug>
#include <QtCore/QVariant>

using namespace QJson;

/**
* @brief Convenience class for converting JSON data to QVariant objects using a dedicated thread private
*/
class ParserRunnable::ParserRunnablePrivate : public TTKPrivate<ParserRunnable>
{
  public:
    QByteArray m_data;
};

ParserRunnable::ParserRunnable(QObject* parent)
    : QObject(parent),
      QRunnable()
{
  TTK_INIT_PRIVATE(ParserRunnable);
  qRegisterMetaType<QVariant>("QVariant");
}

void ParserRunnable::setData(const QByteArray &data)
{
  TTK_D(ParserRunnable);
  d->m_data = data;
}

void ParserRunnable::run()
{
  TTK_D(ParserRunnable);

  bool ok;
  Parser parser;
  QVariant result = parser.parse (d->m_data, &ok);
  if (ok) {
    TTK_LOGGER_INFO("successfully converted json item to QVariant object");
    Q_EMIT parsingFinished(result, true, QString());
  } else {
    const QString errorText = tr("An error occurred while parsing json: %1").arg(parser.errorString());
    TTK_LOGGER_ERROR(errorText);
    Q_EMIT parsingFinished(QVariant(), false, errorText);
  }
}
