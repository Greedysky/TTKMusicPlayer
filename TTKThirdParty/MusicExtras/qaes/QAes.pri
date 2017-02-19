# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/aes_cbc.cpp \
    $$PWD/aes_core.cpp \
    $$PWD/cbc128.cpp \
    $$PWD/qaeswrap.cpp
    
HEADERS += \
    $$PWD/aes.h \
    $$PWD/aes_locl.h \
    $$PWD/cbc128.h \
    $$PWD/qaeswrap.h
