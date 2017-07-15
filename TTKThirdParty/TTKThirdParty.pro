# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (c) 2015 - 2017 Greedysky Studio
# * All rights reserved!
# * Redistribution and use of the source code or any derivative
# * works are strictly forbiden.
# =================================================

TEMPLATE = subdirs
SUBDIRS = zlib MusicExtras Devicewatcher
win32{
    SUBDIRS += MiniDumper
}
