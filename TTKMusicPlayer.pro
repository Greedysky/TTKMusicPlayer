# =================================================
# * This file is part of the TTK Music Player project
# * Copyright (C) 2015 - 2017 Greedysky Studio
#
# * This program is free software; you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation; either version 3 of the License, or
# * (at your option) any later version.
#
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
#
# * You should have received a copy of the GNU General Public License along
# * with this program; If not, see <http://www.gnu.org/licenses/>.
# =================================================

include(TTKVersion.pri)

TEMPLATE = subdirs
SUBDIRS = TTKQrc TTKThirdParty TTKModule TTKService TTKRun TTKTest

TRANSLATIONS += TTKLanguage/cn.ts \
                TTKLanguage/cn_c.ts \
                TTKLanguage/en.ts

##find translation
unix:exists($$[QT_INSTALL_BINS]/lrelease){
    LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease
}

unix:exists($$[QT_INSTALL_BINS]/lrelease-qt5){
    LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease-qt5
}

win32:exists($$[QT_INSTALL_BINS]/lrelease.exe){
    LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease.exe
}

isEmpty(LRELEASE_EXECUTABLE){
    error(Could not find lrelease executable)
}
else{
    message(Found lrelease executable: $$LRELEASE_EXECUTABLE)
}

##update translation
unix:{
    output = $$OUT_PWD/lib/$$TTKMusicPlayer/MLanguage
    !exists($$output):system(mkdir $$output)

    system(find TTKLanguage -name *.ts | xargs $$LRELEASE_EXECUTABLE)
    system(find TTKLanguage -name *.qm | xargs rename -v -f 's/.qm/.ln/' *)
    system(for F in TTKLanguage/*.ln ; do mv $F $$output ;done)
}
win32:{
    output = $$OUT_PWD/bin/$$TTKMusicPlayer/MLanguage
    output = $$replace(output, /, \\)
    !exists($$output):system(md $$output)

    system(for /r TTKLanguage %i in (*.ts) do $$LRELEASE_EXECUTABLE %i)
    system(for /r TTKLanguage %i in (*.qm) do ren %i *.ln)
    system(for /r TTKLanguage %i in (*.ln) do move /y %i $$output)
}

##qmmp lib check
win32:{
    QMMP_DEPANDS = $$OUT_PWD/bin/$$TTKMusicPlayer
    equals(QT_MAJOR_VERSION, 4){
        QMMP_DEPANDS = $$QMMP_DEPANDS/qmmp0.dll
    }else{
        QMMP_DEPANDS = $$QMMP_DEPANDS/qmmp1.dll
    }
    QMMP_DEPANDS = $$replace(QMMP_DEPANDS, /, \\)
}
unix:!mac{
    QMMP_DEPANDS = $$OUT_PWD/lib/$$TTKMusicPlayer/libqmmp.so
}
!exists($$QMMP_DEPANDS): error("Could not find qmmp library, please download and put it to output dir")
