/*
Copyright (C) 2014 - 2016 Evan Teran
                          evan.teran@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// NOTE: this is not part of the "public" Qt API, so using this class directly
//       is not recomended

#ifndef QJSON_PARSER_H_
#define QJSON_PARSER_H_

#include <QtCore/QtGlobal>

#if QT_VERSION < 0x050000

#include "QJsonParseError.h"
#include <QPair>
class QJsonRoot;
class QJsonObject;
class QJsonArray;
class QJsonValue;

class QJsonParser {
	friend class QJsonDocument;

public:
	QJsonParser(const char *begin, const char *end);

public:
	QJsonRoot *parse();

public:
	QJsonParseError state() const;

private:
	static const char ArrayBegin     = '[';
	static const char ArrayEnd       = ']';
	static const char NameSeparator  = ':';
	static const char ValueSeparator = ',';
	static const char ObjectBegin    = '{';
	static const char ObjectEnd      = '}';
	static const char Quote          = '"';

private:
	char peek();
	QJsonObject *getObject();
	QJsonArray *getArray();
	QJsonValue getValue();
	QString getString();
	QJsonValue getTrue();
	QJsonValue getFalse();
	QJsonValue getNull();
	QJsonValue getNumber();
	QPair<QString, QJsonValue> getPair();

private:
	void throwError(QJsonParseError::ParseError e);

private:
	QJsonParseError   state_;
	const char *const begin_;
	const char *const end_;
	const char *      p_;
};

#endif

#endif
