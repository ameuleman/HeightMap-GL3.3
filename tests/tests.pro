QT       += testlib

QT       -= gui

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += TestImageProcessor.h \
    TestHeightMapMesh.h \
    TestLvlPlanMesh.h

SOURCES += main.cpp\
    TestImageProcessor.cpp \
    TestHeightMapMesh.cpp \
    TestLvlPlanMesh.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"
