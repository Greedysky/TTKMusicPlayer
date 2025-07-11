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

#ifndef QJSON_VALUEREF_H_
#define QJSON_VALUEREF_H_

#include <QtCore/QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtCore/QJsonValueRef>
#else

#include "QJsonValue.h"
class QJsonRoot;

class QJsonValueRef {
public:
	QJsonValueRef(QJsonArray *array, int idx);

	// slight variant from official APIs implementation
	QJsonValueRef(QJsonObject *object, const QString &key);

public:
	operator QJsonValue() const;

public:
	QJsonValueRef &operator=(const QJsonValue &val);
	QJsonValueRef &operator=(const QJsonValueRef &val);

public:
	QJsonValue::Type type() const;
	bool isNull() const;
	bool isBool() const;
	bool isDouble() const;
	bool isString() const;
	bool isArray() const;
	bool isObject() const;
	bool isUndefined() const;

public:
	bool toBool() const;
	double toDouble() const;
	QString toString() const;
	QJsonArray toArray() const;
	QJsonObject toObject() const;
	int toInt(int defaultValue = 0) const;

public:
	bool operator==(const QJsonValue &other) const;
	bool operator!=(const QJsonValue &other) const;

private:
	QJsonValue toValue() const;
	void swap(QJsonValueRef &other);

private:
	QJsonRoot *p_;
	int        index_;
	QString    key_;
};

#endif

#endif
