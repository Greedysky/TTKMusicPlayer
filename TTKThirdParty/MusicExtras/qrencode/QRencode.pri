# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/bitstream.h \
    $$PWD/config.h \
    $$PWD/mask.h \
    $$PWD/mmask.h \
    $$PWD/mqrspec.h \
    $$PWD/qrencode_inner.h \
    $$PWD/qrencode.h \
    $$PWD/qrinput.h \
    $$PWD/qrspec.h \
    $$PWD/rscode.h \
    $$PWD/split.h \
    $$PWD/qrcodewidget.h

SOURCES += \
    $$PWD/bitstream.c \
    $$PWD/mask.c \
    $$PWD/mmask.c \
    $$PWD/mqrspec.c \
    $$PWD/qrencode.c \
    $$PWD/qrinput.c \
    $$PWD/qrspec.c \
    $$PWD/rscode.c \
    $$PWD/split.c \
    $$PWD/qrcodewidget.cpp
