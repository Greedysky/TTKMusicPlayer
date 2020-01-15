include(../../plugins.pri)

HEADERS += outputalsa.h \
           outputalsafactory.h  \

SOURCES += outputalsa.cpp \
           outputalsafactory.cpp  \

TARGET = $$PLUGINS_PREFIX/Output/alsa
QMAKE_CLEAN = $$PLUGINS_PREFIX/Output/libalsa.so

INCLUDEPATH += $$EXTRA_PREFIX/libalsa/include

LIBS += -L$$EXTRA_PREFIX/libalsa/lib -lasound

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
