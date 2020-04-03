
INCLUDEPATH += $$PWD/

HEADERS += \
           $$PWD/convert_utf.h \
           $$PWD/stdio_file.h \
           $$PWD/stdio_meta.h

SOURCES += \
           $$PWD/convert_utf.c \
           $$PWD/stdio_file.c \
           $$PWD/stdio_meta.c

# Disbale gcc warnings
QMAKE_CFLAGS += -w
QMAKE_CXXFLAGS += -w
