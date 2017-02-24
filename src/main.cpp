//******************************************************************************
// Include
//******************************************************************************
#include "RenderWindow.h"
#include "HeightMapMesh.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>


//------------------------------------------------------------------------------
int main(int argc, char **argv)
//------------------------------------------------------------------------------
{
    QGuiApplication app(argc, argv);

    RenderWindow window("../data.txt");

	//set the format to get a better rendering
    QSurfaceFormat format;
    format.setSamples(16);
    window.setFormat(format);

	window.resize(1000, 700);
    window.show();

    return app.exec();
}
