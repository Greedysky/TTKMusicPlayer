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

#include "QJsonObject.h"

#if QT_VERSION < 0x050000

//------------------------------------------------------------------------------
// Name: QJsonObject
//------------------------------------------------------------------------------
QJsonObject::QJsonObject() {
}

//------------------------------------------------------------------------------
// Name: QJsonObject
//------------------------------------------------------------------------------
QJsonObject::QJsonObject(const QJsonObject &other) : values_(other.values_) {
}

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: QJsonObject
//------------------------------------------------------------------------------
QJsonObject::QJsonObject(std::initializer_list<QPair<QString, QJsonValue> > args) {
	for(const QPair<QString, QJsonValue> &arg : args) {
		values_.insert(arg.first, arg.second);

	}
}
#endif

//------------------------------------------------------------------------------
// Name: ~QJsonObject
//------------------------------------------------------------------------------
QJsonObject::~QJsonObject() {
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject &QJsonObject::operator=(const QJsonObject &other) {
	QJsonObject(other).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::iterator QJsonObject::begin() {
	return values_.begin();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::const_iterator QJsonObject::begin() const {
	return values_.begin();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::iterator QJsonObject::end() {
	return values_.end();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::const_iterator QJsonObject::end() const {
	return values_.end();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::const_iterator QJsonObject::constBegin() const {
	return begin();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::const_iterator QJsonObject::constEnd() const {
	return end();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
int QJsonObject::count() const {
	return size();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
int QJsonObject::length() const {
	return size();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
int QJsonObject::size() const {
	return values_.size();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
bool QJsonObject::empty() const {
	return values_.empty();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
bool QJsonObject::isEmpty() const {
	return empty();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::const_iterator QJsonObject::constFind(const QString &key) const {
	return values_.find(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
bool QJsonObject::contains(const QString &key) const {
	return values_.contains(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::iterator QJsonObject::find(const QString &key) {
	return values_.find(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::const_iterator QJsonObject::find(const QString &key) const {
	return values_.find(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::iterator QJsonObject::erase(iterator it) {
	return values_.erase(it);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject::iterator QJsonObject::insert(const QString &key, const QJsonValue &value) {
	return values_.insert(key, value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QStringList QJsonObject::keys() const {
	return values_.keys();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonObject::remove(const QString &key) {
	values_.remove(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonValue QJsonObject::take(const QString &key) {
	return values_.take(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonValue QJsonObject::value(const QString &key) const {
	return values_.value(key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
bool QJsonObject::operator!=(const QJsonObject &other) const {
	return values_ != other.values_;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
bool QJsonObject::operator==(const QJsonObject &other) const {
	return values_ != other.values_;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonValue QJsonObject::operator[](const QString &key) const {
	return values_[key];
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonValueRef QJsonObject::operator[](const QString &key) {
	return QJsonValueRef(this, key);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QVariantMap QJsonObject::toVariantMap() const {
	QVariantMap a;
	for(const_iterator it = begin(); it != end(); ++it) {
		a.insert(it.key(), it.value().toVariant());
	}
	return a;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QVariantHash QJsonObject::toVariantHash() const {
	QVariantHash a;
	for(const_iterator it = begin(); it != end(); ++it) {
		a.insert(it.key(), it.value().toVariant());
	}
	return a;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject QJsonObject::fromVariantMap(const QVariantMap &map) {
	QJsonObject o;
	for(QVariantMap::const_iterator it = map.begin(); it != map.end(); ++it) {
		o.insert(it.key(), QJsonValue::fromVariant(it.value()));
	}
	return o;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject QJsonObject::fromVariantHash(const QVariantHash &hash) {
	QJsonObject o;
	for(QVariantHash::const_iterator it = hash.begin(); it != hash.end(); ++it) {
		o.insert(it.key(), QJsonValue::fromVariant(it.value()));
	}
	return o;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonRoot *QJsonObject::clone() const {
	return new QJsonObject(*this);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
const QJsonObject *QJsonObject::toObject() const {
	return this;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject *QJsonObject::toObject() {
	return this;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonArray *QJsonObject::toArray() {
	return 0;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
const QJsonArray *QJsonObject::toArray() const {
	return 0;
}

//------------------------------------------------------------------------------
// Name: swap
//------------------------------------------------------------------------------
void QJsonObject::swap(QJsonObject &other) {
	qSwap(values_, other.values_);
}

#endif
