Qt5 introduced a very nice [API](http://doc.qt.io/qt-5/qjsondocument.html) for dealing with JSON. Unfortunately, it has not been back-ported to Qt4. This library is an attempt to implement the same API in Qt4.

Additionally, it is designed to be forward compatible, so if the library is built with Qt5, then you will get Qt's version of the API.

Simply include the files in your project and `#include QJsonDocument.h` wherever you would `#include <QJsonDocument>` and it should be a drop-in replacement.

NOTE: The library is not 100% complete yet, but is at a usable stage in development. I do plan to implement the library completely at some point. The following functions have not yet been implemented:

* QJsonDocument::fromRawData
* QJsonDocument::fromBinaryData
* QJsonDocument::rawData
* QJsonDocument::toBinaryData

Finally, the following flags are not yet respected:

* QJsonDocument::BypassValidation

Here's an example of using the library:

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

	#include "QJsonValue.h"
	#include "QJsonDocument.h"
	#include "QJsonArray.h"
	#include "QJsonObject.h"
	#include "QJsonParseError.h"
	#include <QFile>
	#include <QCoreApplication>
	#include <QTextCodec>
	#include <QtDebug>

	int main(int argc, char *argv[]) {

		QCoreApplication app(argc, argv);

		QFile f("test.json");
		if(f.open(QIODevice::ReadOnly)) {
			QJsonParseError e;
			QJsonDocument d = QJsonDocument::fromJson(f.readAll(), &e);
			if(!d.isNull() && e.error == QJsonParseError::NoError) {
				qDebug() << QString::fromUtf8(d.toJson(QJsonDocument::Compact));
			} else {
				qDebug() << e.errorString();
			}
		}
	}

If you are not using Qt, but still want a high quality c++ JSON implementation, then you can also check out my other JSON project [cpp-json](https://github.com/eteran/cpp-json).
