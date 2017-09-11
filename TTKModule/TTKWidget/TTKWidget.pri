# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

contains(CONFIG, TTK_BUILD_LIB){
    include($$PWD/musicUiKits/MusicUiKits.pri)
}
include($$PWD/musicLrcKits/MusicLrcKits.pri)
include($$PWD/musicRemoteKits/MusicRemoteKits.pri)
include($$PWD/musicSearchKits/MusicSearchKits.pri)
include($$PWD/musicToolsKits/MusicToolsKits.pri)
include($$PWD/musicToolsSetsKits/MusicToolsSetsKits.pri)
include($$PWD/musicUserKits/MusicUserKits.pri)
include($$PWD/musicVideoKits/MusicVideoKits.pri)
include($$PWD/musicWidgetKits/MusicWidgetKits.pri)
include($$PWD/musicWidgetCoreKits/MusicWidgetCoreKits.pri)
