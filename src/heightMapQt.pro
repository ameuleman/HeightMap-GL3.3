QT += core gui

QT += opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heightMapQt
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    controlPanel/MainWindow.cpp \
    rendering/DepthMap.cpp \
    rendering/HeightMapMesh.cpp \
    rendering/RenderWindow.cpp \
    rendering/Mesh.cpp \
    rendering/LvlPlan.cpp \
    imageProcessing/ImageProcessor.cpp

HEADERS  += controlPanel/MainWindow.h \
    rendering/RenderWindow.h \
    rendering/DepthMap.h \
    rendering/HeightMapMesh.h \
    rendering/Mesh.h \
    rendering/LvlPlan.h \
    imageProcessing/ImageProcessor.h \
    tools/ParallelTool.h \
    tools/Types.h

DISTFILES += ../shader/displayShader.frag \
    ../shader/lvlShader.frag \
    ../shader/mapShader.frag \
    ../shader/displayShader.vert \
    ../shader/lvlShader.vert \
    ../shader/mapShader.vert \
    ../data/city.png \
    ../data/data.jpg \
    ../data/data.png \
    ../data/data.txt \
    ../results

FORMS    += controlPanel/mainwindow.ui
