include(../../plugins.pri)

HEADERS += decoderaacfactory.h \
           decoder_aac.h \
           aacfile.h
          
SOURCES += decoder_aac.cpp \
           decoderaacfactory.cpp \
           aacfile.cpp
	   
TARGET = $$PLUGINS_PREFIX/Input/aac
QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libaac.so

INCLUDEPATH += $$EXTRA_PREFIX/libfaad2/include \
               $$EXTRA_PREFIX/libtaglib/include

win32:{
    gcc{
      LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad \
              -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll
    }
}

unix:{
    unix:android {
        TARGET = $$PLUGINS_PREFIX/../plugin_input_aac
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_aac.so
        target.path = $$LIB_DIR
    }else{
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag
}
