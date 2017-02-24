/**
*******************************************************************************
*
*  @file       OpenGLWindow.cpp
*
*  @brief      virtual class to have a basis for OpenGL rendering.
*
*  @version    1.0
*
*  @date       17/06/2016
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#include "OpenGLWindow.h"

#include <QtGui/QOpenGLContext>
#include <QtCore/QCoreApplication>

//------------------------------------------------------------------------
OpenGLWindow::OpenGLWindow(QWindow *parent): 
//------------------------------------------------------------------------
	QWindow(parent),
	m_context(0),
	m_isUpdated(true)
//------------------------------------------------------------------------
{
	//enable the use of OpenGL
	setSurfaceType(QWindow::OpenGLSurface);
}

//------------------------------------------------------------------------
OpenGLWindow::~OpenGLWindow()
//------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------
void OpenGLWindow::startAnotherRendering()
//------------------------------------------------------------------------
{
	if (m_isUpdated) {
		//prevent this to be called before the end of the rendering 
		m_isUpdated = false; 

		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

//------------------------------------------------------------------------
bool OpenGLWindow::event(QEvent * event)
//------------------------------------------------------------------------
{
	//render if the event is of type QEvent::UpdateRequest
	if (event->type() == QEvent::UpdateRequest) {
		//enable another rendering
		m_isUpdated = true;	
		
		updateRendering();
		return true;
	}
	else {
		return QWindow::event(event);
	}
}

//------------------------------------------------------------------------
void OpenGLWindow::exposeEvent(QExposeEvent *event)
//------------------------------------------------------------------------
{
	//render when the window can be seen
	if (isExposed())
		updateRendering();
}

//------------------------------------------------------------------------
void OpenGLWindow::updateRendering()
//------------------------------------------------------------------------
{
	//render only if the window is exposed
	if (!isExposed())
		return;

	if (!m_context) {
		//Set the context if updateRendering() is called for the first time
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();
		m_context->makeCurrent(this);

		//Initialize if the function is called for the first time
		initializeOpenGLFunctions();
		initializeOpenGL();
	}
	else {
		m_context->makeCurrent(this);
	}

	//display
	render();

	m_context->swapBuffers(this);

	//begin another rendering
	startAnotherRendering();
}
