include($$PWD/../../qmmp.pri)

EXTRA_PREFIX = $$PWD/../../../extra/gcc

QMAKE_LIBDIR += $$OUT_PWD/../../../../bin/$$TTK_VERSION
PLUGINS_PREFIX = $$OUT_PWD/../../../../bin/$$TTK_VERSION/plugins

LIBS += -lTTKqmmp

INCLUDEPATH += $$PWD/../

CONFIG += warn_off plugin lib thread link_pkgconfig
TEMPLATE = lib
