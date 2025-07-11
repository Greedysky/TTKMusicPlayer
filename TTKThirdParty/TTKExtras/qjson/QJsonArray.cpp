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

#include "QJsonArray.h"
#include "QJsonValueRef.h"
#include "QJsonValue.h"
#include <QtCore/QStringList>

#if QT_VERSION < 0x050000

//------------------------------------------------------------------------------
// Name: QJsonArray
// Desc: default constructor
//------------------------------------------------------------------------------
QJsonArray::QJsonArray() {

}

//------------------------------------------------------------------------------
// Name: QJsonArray
// Desc: copy constructor
//------------------------------------------------------------------------------
QJsonArray::QJsonArray(const QJsonArray &other) : values_(other.values_) {

}

#if __cplusplus >= 201103L
//------------------------------------------------------------------------------
// Name: QJsonArray
// Desc: Creates an array initialized from args initialization list.
//------------------------------------------------------------------------------
QJsonArray::QJsonArray(std::initializer_list<QJsonValue> args) {
	for(const QJsonValue &arg : args) {
		values_.append(arg);
	}
}
#endif

//------------------------------------------------------------------------------
// Name: ~QJsonArray
// Desc: destructor
//------------------------------------------------------------------------------
QJsonArray::~QJsonArray() {

}

//------------------------------------------------------------------------------
// Name: operator=
// Desc: assignment operator
//------------------------------------------------------------------------------
QJsonArray &QJsonArray::operator=(const QJsonArray &other) {
	QJsonArray(other).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator+=
// Desc:
//------------------------------------------------------------------------------
QJsonArray &QJsonArray::operator+=(const QJsonValue &value) {
	values_.append(value);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator<<
// Desc:
//------------------------------------------------------------------------------
QJsonArray &QJsonArray::operator<<(const QJsonValue &value) {
	values_.append(value);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator+
// Desc:
//------------------------------------------------------------------------------
QJsonArray QJsonArray::operator+(const QJsonValue &value) const {
	QJsonArray arr(*this);
	arr.append(value);
	return arr;
}

//------------------------------------------------------------------------------
// Name: operator!=
// Desc: returns true if the compared array IS NOT equal to this
//------------------------------------------------------------------------------
bool QJsonArray::operator!=(const QJsonArray &other) const {
	return values_ != other.values_;
}

//------------------------------------------------------------------------------
// Name: operator==
// Desc: returns true if the compared array IS equal to this
//------------------------------------------------------------------------------
bool QJsonArray::operator==(const QJsonArray &other) const {
	return values_ == other.values_;
}

//------------------------------------------------------------------------------
// Name: begin
// Desc: returns an iterator to the first contained element
//------------------------------------------------------------------------------
QJsonArray::const_iterator QJsonArray::begin() const {
	return values_.begin();
}

//------------------------------------------------------------------------------
// Name: end
// Desc: returns an iterator to one past the last contained element
//------------------------------------------------------------------------------
QJsonArray::const_iterator QJsonArray::end() const {
	return values_.end();
}

//------------------------------------------------------------------------------
// Name: begin
// Desc: returns an iterator to the first contained element
//------------------------------------------------------------------------------
QJsonArray::iterator QJsonArray::begin() {
	return values_.begin();
}

//------------------------------------------------------------------------------
// Name: end
// Desc: returns an iterator to one past the last contained element
//------------------------------------------------------------------------------
QJsonArray::iterator QJsonArray::end() {
	return values_.end();
}

//------------------------------------------------------------------------------
// Name: constBegin
// Desc: returns an iterator to the first contained element
//------------------------------------------------------------------------------
QJsonArray::const_iterator QJsonArray::constBegin() const {
	return begin();
}

//------------------------------------------------------------------------------
// Name: constEnd
// Desc: returns an iterator to one past the last contained element
//------------------------------------------------------------------------------
QJsonArray::const_iterator QJsonArray::constEnd() const {
	return end();
}

//------------------------------------------------------------------------------
// Name: first
// Desc: returns the first element by value
//------------------------------------------------------------------------------
QJsonValue QJsonArray::first() const {
	Q_ASSERT(!empty());
	return values_.first();
}

//------------------------------------------------------------------------------
// Name: last
// Desc: returns the last element by value
//------------------------------------------------------------------------------
QJsonValue QJsonArray::last() const {
	Q_ASSERT(!empty());
	return values_.last();
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
QJsonValueRef QJsonArray::operator[](int i) {
	return QJsonValueRef(this, i);
}

//------------------------------------------------------------------------------
// Name: operator[]
//------------------------------------------------------------------------------
QJsonValue QJsonArray::operator[](int i) const {
	return values_[i];
}

//------------------------------------------------------------------------------
// Name: at
//------------------------------------------------------------------------------
QJsonValue QJsonArray::at(int i) const {
	return values_.at(i);
}

//------------------------------------------------------------------------------
// Name: size
//------------------------------------------------------------------------------
int QJsonArray::size() const {
	return values_.size();
}

//------------------------------------------------------------------------------
// Name: count
//------------------------------------------------------------------------------
int QJsonArray::count() const {
	return size();
}

//------------------------------------------------------------------------------
// Name: empty
//------------------------------------------------------------------------------
bool QJsonArray::empty() const {
	return values_.empty();
}

//------------------------------------------------------------------------------
// Name: isEmpty
//------------------------------------------------------------------------------
bool QJsonArray::isEmpty() const {
	return empty();
}

//------------------------------------------------------------------------------
// Name: pop_back
//------------------------------------------------------------------------------
void QJsonArray::pop_back() {
	values_.pop_back();
}

//------------------------------------------------------------------------------
// Name: pop_front
//------------------------------------------------------------------------------
void QJsonArray::pop_front() {
	values_.pop_front();
}

//------------------------------------------------------------------------------
// Name: push_back
//------------------------------------------------------------------------------
void QJsonArray::push_back(const QJsonValue &value) {
	values_.push_back(value);
}

//------------------------------------------------------------------------------
// Name: push_front
//------------------------------------------------------------------------------
void QJsonArray::push_front(const QJsonValue &value) {
	values_.push_front(value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::append(const QJsonValue &value) {
	values_.append(value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
bool QJsonArray::contains(const QJsonValue &value) const {
	return values_.contains(value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonArray::iterator QJsonArray::erase(iterator it) {
	return values_.erase(it);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::insert(int i, const QJsonValue &value) {
	values_.insert(i, value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonArray::iterator QJsonArray::insert(iterator before, const QJsonValue &value) {
	return values_.insert(before, value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::prepend(const QJsonValue &value) {
	values_.prepend(value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::removeAt(int i) {
	values_.removeAt(i);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::removeFirst() {
	values_.removeFirst();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::removeLast() {
	values_.removeLast();
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::replace(int i, const QJsonValue &value) {
	values_.replace(i, value);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonValue QJsonArray::takeAt(int i) {
	return values_.takeAt(i);
}

//------------------------------------------------------------------------------
// Name: toVariantList
//------------------------------------------------------------------------------
QVariantList QJsonArray::toVariantList() const {
	QVariantList a;
	Q_FOREACH(const QJsonValue &v, *this) {
		a.push_back(v.toVariant());
	}
	return a;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonArray QJsonArray::fromStringList(const QStringList &list) {
	QJsonArray a;
	Q_FOREACH(const QString &s, list) {
		a.push_back(QJsonValue(s));
	}
	return a;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonArray QJsonArray::fromVariantList(const QVariantList &list) {
	QJsonArray a;
	Q_FOREACH(const QVariant &v, list) {
		a.push_back(QJsonValue::fromVariant(v));
	}
	return a;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonRoot *QJsonArray::clone() const {
	return new QJsonArray(*this);
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
const QJsonObject *QJsonArray::toObject() const {
	return 0;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonObject *QJsonArray::toObject() {
	return 0;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
QJsonArray *QJsonArray::toArray() {
	return this;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
const QJsonArray *QJsonArray::toArray() const {
	return this;
}

//------------------------------------------------------------------------------
// Name:
//------------------------------------------------------------------------------
void QJsonArray::swap(QJsonArray &other) {
	qSwap(values_, other.values_);
}

#endif
