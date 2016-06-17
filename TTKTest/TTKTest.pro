#-------------------------------------------------
#
# Project created by QtCreator 2014-08-01T14:49:11
#
#-------------------------------------------------

QT       += core testlib gui
equals(QT_MAJOR_VERSION, 5){
QT       += widgets
}

TARGET = TTKTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS  += \
    ttkautotest.h

SOURCES  += \
    musicapplicationmain.cpp
