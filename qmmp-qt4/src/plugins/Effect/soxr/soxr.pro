include(../../plugins.pri)

HEADERS += soxresampler.h \
           effectsoxrfactory.h \
           settingsdialog.h

SOURCES += soxresampler.cpp \
           effectsoxrfactory.cpp \
           settingsdialog.cpp

FORMS += settingsdialog.ui

TARGET = $$PLUGINS_PREFIX/Effect/soxr

INCLUDEPATH += $$EXTRA_PREFIX/libsoxr/include \

win32:{
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr
}

unix:{
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Effect/libsoxr.so
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    LIBS += -L/usr/lib -I/usr/include
    LIBS += -L$$EXTRA_PREFIX/libsoxr/lib -lsoxr$$STATIC_LIBRARY_SUFFIX
}
