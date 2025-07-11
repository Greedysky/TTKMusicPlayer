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

#include "QJsonParseError.h"

#if QT_VERSION < 0x050000

//------------------------------------------------------------------------------
// Name: errorString
// Desc: The QJsonParseError class is used to report errors during JSON parsing.
//------------------------------------------------------------------------------
QString QJsonParseError::errorString() const {
	switch(error) {
	case NoError:
		return "No error occurred";
	case UnterminatedObject:
		return "unterminated object";
	case MissingNameSeparator:
		return "missing name separator";
	case UnterminatedArray:
		return "unterminated array";
	case MissingValueSeparator:
		return "missing value separator";
	case IllegalValue:
		return "illegal value";
	case TerminationByNumber:
		return "invalid termination by number";
	case IllegalNumber:
		return "illegal number";
	case IllegalEscapeSequence:
		return "illegal escape sequence";
	case IllegalUTF8String:
		return "invalid UTF8 string";
	case UnterminatedString:
		return "unterminated string";
	case MissingObject:
		return "object is missing after a comma";
	case DeepNesting:
		return "too deeply nested document";
	case DocumentTooLarge:
		return "too large document";
	case GarbageAtEnd:
		return "garbage at the end of the document";
	}

	return QString();
}

#endif
