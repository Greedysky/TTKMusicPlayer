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

#include "QJsonValueRef.h"

#if QT_VERSION < 0x050000

#include "QJsonArray.h"
#include "QJsonObject.h"

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
QJsonValueRef::QJsonValueRef(QJsonArray *array, int idx) : p_(array), index_(idx) {
}

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
QJsonValueRef::QJsonValueRef(QJsonObject *object, const QString &key) : p_(object), index_(0), key_(key) {
}

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
QJsonValueRef::operator QJsonValue() const {
	return toValue();
}

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
QJsonValueRef &QJsonValueRef::operator=(const QJsonValue &val) {

	if(QJsonObject *const o = p_->toObject()) {
		o->values_[key_] = val;
	} else if(QJsonArray *const a = p_->toArray()) {
		a->values_[index_] = val;
	}
	return *this;
}

//------------------------------------------------------------------------------
// Name:
// Desc:
//------------------------------------------------------------------------------
QJsonValueRef &QJsonValueRef::operator=(const QJsonValueRef &ref) {

	if(QJsonObject *const o = p_->toObject()) {
		o->values_[key_] = ref;
	} else if(QJsonArray *const a = p_->toArray()) {
		a->values_[index_] = ref;
	}
	return *this;
}

//------------------------------------------------------------------------------
// Name: type
// Desc:
//------------------------------------------------------------------------------
QJsonValue::Type QJsonValueRef::type() const {
	return toValue().type();
}

//------------------------------------------------------------------------------
// Name: isNull
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isNull() const {
	return toValue().isNull();
}

//------------------------------------------------------------------------------
// Name: isBool
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isBool() const {
	return toValue().isBool();
}

//------------------------------------------------------------------------------
// Name: isDouble
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isDouble() const {
	return toValue().isDouble();
}

//------------------------------------------------------------------------------
// Name: isString
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isString() const {
	return toValue().isString();
}

//------------------------------------------------------------------------------
// Name: isArray
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isArray() const {
	return toValue().isArray();
}

//------------------------------------------------------------------------------
// Name: isObject
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isObject() const {
	return toValue().isObject();
}

//------------------------------------------------------------------------------
// Name: isUndefined
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::isUndefined() const {
	return toValue().isUndefined();
}

//------------------------------------------------------------------------------
// Name: toBool
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::toBool() const {
	return toValue().toBool();
}

//------------------------------------------------------------------------------
// Name: toDouble
// Desc:
//------------------------------------------------------------------------------
double QJsonValueRef::toDouble() const {
	return toValue().toDouble();
}

//------------------------------------------------------------------------------
// Name: toInt
// Desc:
//------------------------------------------------------------------------------
int QJsonValueRef::toInt(int defaultValue) const {
	return toValue().toInt(defaultValue);
}

//------------------------------------------------------------------------------
// Name: toString
// Desc:
//------------------------------------------------------------------------------
QString QJsonValueRef::toString() const {
	return toValue().toString();
}

//------------------------------------------------------------------------------
// Name: toArray
// Desc:
//------------------------------------------------------------------------------
QJsonArray QJsonValueRef::toArray() const {
	return toValue().toArray();
}

//------------------------------------------------------------------------------
// Name: toObject
// Desc:
//------------------------------------------------------------------------------
QJsonObject QJsonValueRef::toObject() const {
	return toValue().toObject();
}

//------------------------------------------------------------------------------
// Name: operator==
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::operator==(const QJsonValue &other) const {
	return toValue() == other;
}

//------------------------------------------------------------------------------
// Name: operator!=
// Desc:
//------------------------------------------------------------------------------
bool QJsonValueRef::operator!=(const QJsonValue &other) const {
	return toValue() != other;
}

//------------------------------------------------------------------------------
// Name: toValue
// Desc:
//------------------------------------------------------------------------------
QJsonValue QJsonValueRef::toValue() const {
	if(QJsonObject *const o = p_->toObject()) {
		return o->values_[key_];
	} else if(QJsonArray *const a = p_->toArray()) {
		return a->values_[index_];
	}

	return QJsonValue();
}

//------------------------------------------------------------------------------
// Name: swap
// Desc:
//------------------------------------------------------------------------------
void QJsonValueRef::swap(QJsonValueRef &other) {
	qSwap(p_, other.p_);
	qSwap(key_, other.key_);
	qSwap(index_, other.index_);
}

#endif
