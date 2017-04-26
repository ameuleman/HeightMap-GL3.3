QT += core gui opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = build/
TARGET = heightMap
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

#Copy required folder to the build destination
resources_folder = $${PWD}/resources
resources_destination = $${DESTDIR}

win32 {
    resources_destination = $${DESTDIR}/resources

    resources_folder ~= s,/,\\,g
    resources_destination ~= s,/,\\,g
}

copyresources.commands = $(COPY_DIR) $${resources_folder}  $${resources_destination}
first.depends = $(first) copyresources
export(first.depends)
export(copyresources.commands)
QMAKE_EXTRA_TARGETS += first copyresources

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

DISTFILES += resources/shader/displayShader.frag \
    resources/shader/lvlShader.frag \
    resources/shader/mapShader.frag \
    resources/shader/displayShader.vert \
    resources/shader/lvlShader.vert \
    resources/shader/mapShader.vert \
    resources/data/*

FORMS    += controlPanel/mainwindow.ui
