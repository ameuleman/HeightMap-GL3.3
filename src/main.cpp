//******************************************************************************
// Include
//******************************************************************************
#include "RenderWindow.h"
#include "ImageProcessor.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>


//------------------------------------------------------------------------------
int main(int argc, char **argv)
//------------------------------------------------------------------------------
{
    ImageProcessor imageProcessor("../data.jpg");

    QGuiApplication app(argc, argv);

    RenderWindow rawWindow(imageProcessor.getRawData(),
                           imageProcessor.getN(), imageProcessor.getM());

    rawWindow.resize(1000, 700);
    rawWindow.show();

    RenderWindow processedWindow(imageProcessor.getProcessedData(),
                           imageProcessor.getN(), imageProcessor.getM());

    processedWindow.resize(1000, 700);
    processedWindow.show();

    return app.exec();
}
