#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

/**
*******************************************************************************
*
*  @file       OpenGLWindow.h
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
//  Include
//******************************************************************************
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

//==============================================================================
/**
*  @class  OpenGLWindow
*  @brief  OpenGLWindow is  virtual class to have a basis for OpenGL rendering.
*/
//==============================================================================
class OpenGLWindow : public QWindow, protected QOpenGLFunctions{
public:

	//--------------------------------------------------------------------------
	/// Default Constructor
	/**
	*  @param parent: the QWindow in which the OpenGLWindow is included (default value is 0)
	*/
	//--------------------------------------------------------------------------
	OpenGLWindow(QWindow *parent = 0);
	virtual ~OpenGLWindow();

	//--------------------------------------------------------------------------
	///display on the window
	//--------------------------------------------------------------------------
	virtual void render() = 0;
	
	//--------------------------------------------------------------------------
	///display on the window
	//--------------------------------------------------------------------------
	virtual void initializeOpenGL() = 0;

	//--------------------------------------------------------------------------
	///Generate an UpdateRequest event to refresh the display
	//--------------------------------------------------------------------------
	void startAnotherRendering();

	//--------------------------------------------------------------------------
	///Call render() and initialize the context and OpenGL if they have not been initialized before
	//--------------------------------------------------------------------------
	void updateRendering();

	//******************************************************************************
private:
	//Should never be copied
	OpenGLWindow(OpenGLWindow const&);
	
	//--------------------------------------------------------------------------
	///call updateRendering() when UpdateRequest event occurs
	//--------------------------------------------------------------------------
	bool event(QEvent *event);

	//--------------------------------------------------------------------------
	///Call updateRendering() when expose event occurs
	//--------------------------------------------------------------------------
	void exposeEvent(QExposeEvent *event);

	QOpenGLContext *m_context; // Context to enable OpenGL to render on the window
	
	bool m_isUpdated; //to know if an update is required to control the refresh of the display
};

#endif //OPENGLWINDOW_H
