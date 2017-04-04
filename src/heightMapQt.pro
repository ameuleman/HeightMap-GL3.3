QT += core gui

QT += opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heightMapQt
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    MainWindow.cpp \
    DepthMap.cpp \
    HeightMapMesh.cpp \
    RenderWindow.cpp \
    Mesh.cpp \
    LvlPlan.cpp \
    ImageProcessor.cpp

HEADERS  += MainWindow.h \
    RenderWindow.h \
    DepthMap.h \
    HeightMapMesh.h \
    Mesh.h \
    LvlPlan.h \
    ImageProcessor.h \
    ParallelTool.h

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

FORMS    += mainwindow.ui
