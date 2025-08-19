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

#include "QJsonDocument.h"

#if QT_VERSION < 0x050000

#include "QJsonObject.h"
#include "QJsonArray.h"
#include "QJsonParser.h"

#include <QtCore/QStringList>
#include <QtCore/QByteArray>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>
#include <QtCore/QtCore>

//------------------------------------------------------------------------------
// Name: QJsonDocument
//------------------------------------------------------------------------------
QJsonDocument::QJsonDocument() : root_(0) {
}

//------------------------------------------------------------------------------
// Name: QJsonDocument
//------------------------------------------------------------------------------
QJsonDocument::QJsonDocument(const QJsonObject &object) : root_(0) {
	setObject(object);
}

//------------------------------------------------------------------------------
// Name: QJsonDocument
//------------------------------------------------------------------------------
QJsonDocument::QJsonDocument(const QJsonArray &array) : root_(0) {
	setArray(array);
}

//------------------------------------------------------------------------------
// Name: QJsonDocument
//------------------------------------------------------------------------------
QJsonDocument::QJsonDocument(const QJsonDocument &other) : root_(0) {
	if(other.root_) {
		root_ = other.root_->clone();
	}
}

//------------------------------------------------------------------------------
// Name: ~QJsonDocument
//------------------------------------------------------------------------------
QJsonDocument::~QJsonDocument() {
	delete root_;
}

//------------------------------------------------------------------------------
// Name: operator=
//------------------------------------------------------------------------------
QJsonDocument &QJsonDocument::operator=(const QJsonDocument &other) {
	QJsonDocument(other).swap(*this);
	return *this;
}

//------------------------------------------------------------------------------
// Name: operator!=
//------------------------------------------------------------------------------
bool QJsonDocument::operator!=(const QJsonDocument &other) const {
	return !(*this == other);
}

//------------------------------------------------------------------------------
// Name: operator==
//------------------------------------------------------------------------------
bool QJsonDocument::operator==(const QJsonDocument &other) const {

	if(isArray() && other.isArray()) {
		return array() == other.array();
	}

	if(isObject() && other.isObject()) {
		return object() == other.object();
	}

	if(isEmpty() && other.isEmpty()) {
		return true;
	}

	if(isNull() && other.isNull()) {
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------
// Name: isArray
//------------------------------------------------------------------------------
bool QJsonDocument::isArray() const {
	return root_ && root_->toArray();
}

//------------------------------------------------------------------------------
// Name: isEmpty
//------------------------------------------------------------------------------
bool QJsonDocument::isEmpty() const {

	// TODO(eteran): figure out the rules here that Qt5 uses
	//               it *looks* like they define empty as being NULL
	//               which is obviously different than this

	return !root_;
}

//------------------------------------------------------------------------------
// Name: isNull
//------------------------------------------------------------------------------
bool QJsonDocument::isNull() const {
	return !root_;
}

//------------------------------------------------------------------------------
// Name: isObject
//------------------------------------------------------------------------------
bool QJsonDocument::isObject() const {
	return root_ && root_->toObject();
}

//------------------------------------------------------------------------------
// Name: setArray
//------------------------------------------------------------------------------
void QJsonDocument::setArray(const QJsonArray &array) {
	setRoot(array);
}

//------------------------------------------------------------------------------
// Name: setObject
//------------------------------------------------------------------------------
void QJsonDocument::setObject(const QJsonObject &object) {
	setRoot(object);
}

//------------------------------------------------------------------------------
// Name: setRoot
//------------------------------------------------------------------------------
void QJsonDocument::setRoot(const QJsonRoot &root) {
	delete root_;
	root_ = root.clone();
}

//------------------------------------------------------------------------------
// Name: toBinaryData
//------------------------------------------------------------------------------
QByteArray QJsonDocument::toBinaryData() const {
	QByteArray r;
	// TODO(eteran): implement this
	return r;
}

//------------------------------------------------------------------------------
// Name: escapeString
//------------------------------------------------------------------------------
QString QJsonDocument::escapeString(const QString &s) const {

	QString r;

	Q_FOREACH(QChar ch, s) {
		switch(ch.toLatin1()) {
		case '\"': r.append("\\\""); break;
		case '\\': r.append("\\\\"); break;
	#if 0
		case '/':  r.append("\\/"); break;
	#endif
		case '\b': r.append("\\b"); break;
		case '\f': r.append("\\f"); break;
		case '\n': r.append("\\n"); break;
		case '\r': r.append("\\r"); break;
		case '\t': r.append("\\t"); break;
		default:
			r += ch;
			break;
		}
	}

	return r;
}

//------------------------------------------------------------------------------
// Name: toJson
//------------------------------------------------------------------------------
QString QJsonDocument::toJson(const QJsonValue &v, JsonFormat format, int indent) const {

	QString b;
	QTextStream ss(&b, QIODevice::WriteOnly | QIODevice::Text);
	bool compact = (format == QJsonDocument::Compact);

	switch(v.type()) {
	case QJsonValue::Null:
		ss << "null";
		break;
	case QJsonValue::Bool:
		ss << (v.toBool() ? "true" : "false");
		break;
	case QJsonValue::Double:
	{
        double d = v.toDouble();
		if (qIsFinite(d)) {
			// +2 to format to ensure the expected precision
			ss <<  QByteArray::number(d, 'g', 15 + 2); // ::digits10 is 15
		} else {
			ss <<  "null"; // +INF || -INF || NaN (see RFC4627#section2.4)
		}
	}
		break;
	case QJsonValue::String:
		ss << '"' << escapeString(v.toString()) << '"';
		break;
	case QJsonValue::Array:
	{
		const QJsonArray a = v.toArray();
		ss << (compact ? "[" : "[\n");
		if(!a.empty()) {
			QJsonArray::const_iterator it = a.begin();
			QJsonArray::const_iterator e  = a.end();

			if (!compact) ss << QByteArray(4*indent, ' ');
			ss << toJson(*it++, format, indent+1);

			for(;it != e; ++it) {
				ss << (compact ? "," : ",\n");
				if (!compact) ss << QByteArray(4*indent, ' ');
				ss << toJson(*it, format, indent+1);
			}
		}
		indent--;
		ss << (compact ? "]" : QString("\n%1]").arg(QString(4*indent, ' ')));
	}
		break;
	case QJsonValue::Object:
	{
		const QJsonObject o = v.toObject();
		ss << (compact ? "{" : "{\n");
		if(!o.empty()) {
			QJsonObject::const_iterator it = o.begin();
			QJsonObject::const_iterator e  = o.end();

			if (!compact) ss << QByteArray(4*indent, ' ');
			ss << '"' << escapeString(it.key()) << (compact ? "\":" : "\": ") << toJson(it.value(), format, indent+1);
			++it;
			for(;it != e; ++it) {
				ss << (compact ? "," : ",\n");
				if (!compact) ss << QByteArray(4*indent, ' ');
				ss << '"' << escapeString(it.key()) << (compact ? "\":" : "\": ") << toJson(it.value(), format, indent+1);
			}
		}
		indent--;
		ss << (compact ? "}" : QString("\n%1}").arg(QString(4*indent, ' ')));
	}
		break;
	case QJsonValue::Undefined:
		Q_ASSERT(0);
		break;
	}

	return b;
}

//------------------------------------------------------------------------------
// Name: toJson
//------------------------------------------------------------------------------
QByteArray QJsonDocument::toJson(JsonFormat format) const {

	Q_UNUSED(format);

	if(isArray()) {
		QString s = toJson(array(), format);
		return s.toUtf8();
	}

	if(isObject()) {
		QString s = toJson(object(), format);
		return s.toUtf8();
	}

	return QByteArray();
}

//------------------------------------------------------------------------------
// Name: toVariant
//------------------------------------------------------------------------------
QVariant QJsonDocument::toVariant() const {

	if(!isEmpty()) {
		if(QJsonObject *const object = root_->toObject()) {
			return object->toVariantMap();
		}

		if(QJsonArray *const array = root_->toArray()) {
			return array->toVariantList();
		}
	}

	return QVariant();
}

//------------------------------------------------------------------------------
// Name: array
//------------------------------------------------------------------------------
QJsonArray QJsonDocument::array() const {

	if(!isEmpty()) {
		if(QJsonArray *const array = root_->toArray()) {
			return *array;
		}
	}

	return QJsonArray();
}

//------------------------------------------------------------------------------
// Name: object
//------------------------------------------------------------------------------
QJsonObject QJsonDocument::object() const {

	if(!isEmpty()) {
		if(QJsonObject *const object = root_->toObject()) {
			return *object;
		}
	}

	return QJsonObject();
}

//------------------------------------------------------------------------------
// Name: rawData
//------------------------------------------------------------------------------
const char *QJsonDocument::rawData(int *size) const {
	Q_UNUSED(size);
	// TODO(eteran): implement this
	return 0;
}

//------------------------------------------------------------------------------
// Name: fromBinaryData
//------------------------------------------------------------------------------
QJsonDocument QJsonDocument::fromBinaryData(const QByteArray &data, DataValidation validation) {
	Q_UNUSED(data);
	Q_UNUSED(validation);

	QJsonDocument doc;
	// TODO(eteran): implement this
	return doc;
}

//------------------------------------------------------------------------------
// Name: fromJson
//------------------------------------------------------------------------------
QJsonDocument QJsonDocument::fromJson(const QByteArray &json, QJsonParseError *error) {
	QJsonDocument doc;

	const char *const begin = json.constData();
	const char *const end   = begin + json.size();

	QJsonParser parser(begin, end);

	doc.root_ = parser.parse();

	if(error) {
		*error = parser.state();
	}

	return doc;
}

//------------------------------------------------------------------------------
// Name: fromRawData
//------------------------------------------------------------------------------
QJsonDocument QJsonDocument::fromRawData(const char *data, int size, DataValidation validation) {

	// data has to be aligned to a 4 byte boundary.
	Q_ASSERT(!(reinterpret_cast<quintptr>(data) % 3));

	return fromBinaryData(QByteArray::fromRawData(data, size), validation);
}

//------------------------------------------------------------------------------
// Name: fromVariant
//------------------------------------------------------------------------------
QJsonDocument QJsonDocument::fromVariant(const QVariant &variant) {

	QJsonDocument doc;

	if (variant.type() == QVariant::Map) {
		doc.setObject(QJsonObject::fromVariantMap(variant.toMap()));
	} else if (variant.type() == QVariant::Hash) {
		doc.setObject(QJsonObject::fromVariantHash(variant.toHash()));
	} else if (variant.type() == QVariant::List) {
		doc.setArray(QJsonArray::fromVariantList(variant.toList()));
	} else if (variant.type() == QVariant::StringList) {
		doc.setArray(QJsonArray::fromStringList(variant.toStringList()));
	}

	return doc;
}

//------------------------------------------------------------------------------
// Name: swap
//------------------------------------------------------------------------------
void QJsonDocument::swap(QJsonDocument &other) {
	qSwap(root_, other.root_);
}

#endif
