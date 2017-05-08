DESTDIR = build/
TARGET = heightMap
TEMPLATE = app

QT += core gui opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES = $$PWD/src.qrc

SOURCES += $$PWD/main.cpp \
    $$PWD/controlPanel/MainWindow.cpp \
    $$PWD/rendering/DepthMap.cpp \
    $$PWD/rendering/HeightMapMesh.cpp \
    $$PWD/rendering/RenderWindow.cpp \
    $$PWD/rendering/Mesh.cpp \
    $$PWD/rendering/LvlPlan.cpp \
    $$PWD/imageProcessing/ImageProcessor.cpp

HEADERS  += $$PWD/controlPanel/MainWindow.h \
    $$PWD/rendering/RenderWindow.h \
    $$PWD/rendering/DepthMap.h \
    $$PWD/rendering/HeightMapMesh.h \
    $$PWD/rendering/Mesh.h \
    $$PWD/rendering/LvlPlan.h \
    $$PWD/imageProcessing/ImageProcessor.h \
    $$PWD/tools/ParallelTool.h \
    $$PWD/tools/Types.h

FORMS += $$PWD/controlPanel/mainwindow.ui

