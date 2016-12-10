# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

HEADERS  += \
    $$PWD/parser.h \
    $$PWD/parserrunnable.h \
    $$PWD/qobjecthelper.h \
    $$PWD/serializer.h \
    $$PWD/serializerrunnable.h

SOURCES += \
    $$PWD/parser.cpp \
    $$PWD/qobjecthelper.cpp \
    $$PWD/json_scanner.cpp \
    $$PWD/json_parser.cc \
    $$PWD/parserrunnable.cpp \
    $$PWD/serializer.cpp \
    $$PWD/serializerrunnable.cpp
