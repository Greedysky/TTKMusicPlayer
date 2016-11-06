include(../../plugins.pri)

HEADERS += decoderaacfactory.h \
           decoder_aac.h \
           aacfile.h \
           aacmetadatamodel.h
          
SOURCES += decoder_aac.cpp \
           decoderaacfactory.cpp \
           aacfile.cpp \
           aacmetadatamodel.cpp

unix:android {
    TARGET = $$PLUGINS_PREFIX/../plugin_input_aac
}else{
    TARGET = $$PLUGINS_PREFIX/Input/aac
}

INCLUDEPATH += ../../../ \
               $$EXTRA_PREFIX/libfaad2/include \
               $$EXTRA_PREFIX/libtaglib/include
               
CONFIG += warn_on \
          plugin \
          link_pkgconfig
    
TEMPLATE = lib

win32:{
    QMAKE_LIBDIR += ../../../../bin/$$TTKMusicPlayer
    gcc{
      LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad \
              -L$$EXTRA_PREFIX/libtaglib/lib -ltag.dll -lqmmp1
    }
}

unix:{
    isEmpty(LIB_DIR):LIB_DIR = /lib/$$TTKMusicPlayer
    QMAKE_LIBDIR += ../../../../lib/$$TTKMusicPlayer
    unix:android {
        QMAKE_CLEAN =$$PLUGINS_PREFIX/../libplugin_input_aac.so
        target.path = $$LIB_DIR
    }else{
        QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libaac.so
        target.path = $$LIB_DIR/qmmp/Input
    }
    INSTALLS += target
    LIBS += -L$$EXTRA_PREFIX/libfaad2/lib -lfaad \
            -L$$EXTRA_PREFIX/libtaglib/lib -ltag -lqmmp
}
