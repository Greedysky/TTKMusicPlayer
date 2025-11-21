include($$PWD/../../Input.pri)

DESTDIR = $$PLUGINS_PREFIX/Input

HEADERS += decoderopusfactory.h \
           decoder_opus.h \
           opusmetadatamodel.h

SOURCES += decoderopusfactory.cpp \
           decoder_opus.cpp \
           opusmetadatamodel.cpp

INCLUDEPATH += $$EXTRA_LIB_PATH/libopusfile/include \
               $$EXTRA_LIB_PATH/libogg/include \
               $$EXTRA_LIB_PATH/libtaglib/include

win32{
    LIBS += -L$$EXTRA_LIB_PATH/libopusfile/lib -lopusfile -lopus \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag -lm \
            -L$$EXTRA_LIB_PATH/libogg/lib -logg
    lessThan(QT_MAJOR_VERSION, 6)|if(equals(QT_MAJOR_VERSION, 6):lessThan(QT_MINOR_VERSION, 8)):LIBS += -lssp
}

unix:!mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_LIB_PATH/libopusfile/lib -lopusfile$$STATIC_LIB_SUFFIX -lopus$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libtaglib/lib -ltag$$STATIC_LIB_SUFFIX \
            -L$$EXTRA_LIB_PATH/libogg/lib -logg$$STATIC_LIB_SUFFIX
}

mac{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.dylib
    LIBS += -lopusfile -lopus -ltag -logg
}
