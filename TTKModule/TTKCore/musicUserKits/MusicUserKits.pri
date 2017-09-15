# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

INCLUDEPATH += $$PWD

!contains(CONFIG, MUSIC_NO_MSVC_LINK_NEED){
HEADERS  += \
    $$PWD/musicusermodel.h \
    $$PWD/musicuserconfigmanager.h
    
}

contains(CONFIG, MUSIC_BUILD_LIB){
SOURCES += \
    $$PWD/musicusermodel.cpp \
    $$PWD/musicuserconfigmanager.cpp

}
