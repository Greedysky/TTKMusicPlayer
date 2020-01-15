include(../../plugins.pri)

HEADERS += decoderaacfactory.h \
           decoder_aac.h \
           aacfile.h
          
SOURCES += decoder_aac.cpp \
           decoderaacfactory.cpp \
           aacfile.cpp
	   
TARGET = $$PLUGINS_PREFIX/Input/aac

INCLUDEPATH += $$EXTRA_PREFIX/libfaad2/include \
               $$EXTRA_PREFIX/libtaglib/include

unix:{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_aac
        QMAKE_CLEAN = $$PLUGINS_PREFIX/../libplugin_input_aac.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libaac.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad$$STATIC_LIBRARY_SUFFIX \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag$$STATIC_LIBRARY_SUFFIX
}

win32:{
    LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll
}
