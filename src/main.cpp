/**
*******************************************************************************
*
*  @file       main.cpp
*
*  @brief      Load an image, perform edge detection and display the result
*               and the original data as height maps.
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
// Include
//******************************************************************************
#include "RenderWindow.h"
#include "ImageProcessor.h"

#include <QApplication>
#include <QtGui/QScreen>
#include <iostream>

//------------------------------------------------------------------------------
int main(int argc, char **argv)
//------------------------------------------------------------------------------
{
    //Read the image and performe processing
    ImageProcessor imageProcessor("../data/data.jpg");

    //Launch the windows
    QApplication app(argc, argv);

    RenderWindow rawWindow(imageProcessor.getRawData(),
                           imageProcessor.getN(), imageProcessor.getM());

    rawWindow.setTitle("Original image");
    rawWindow.resize(800, 450);
    rawWindow.show();

    RenderWindow smoothedWindow(imageProcessor.getSmoothedData(),
                           imageProcessor.getN(), imageProcessor.getM());

    smoothedWindow.setTitle("Smoothed image");
    smoothedWindow.resize(800, 450);
    smoothedWindow.show();

    RenderWindow gradientWindow(imageProcessor.getGradientData(),
                           imageProcessor.getN(), imageProcessor.getM());

    gradientWindow.setTitle("Gradient norm image");
    gradientWindow.resize(800, 450);
    gradientWindow.show();

    RenderWindow cannyWindow(imageProcessor.getCannyData(),
                           imageProcessor.getN(), imageProcessor.getM());

    cannyWindow.setTitle("Canny image");
    cannyWindow.resize(800, 450);
    cannyWindow.show();

    return app.exec();
}
