#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

/**
*******************************************************************************
*
*  @file       RenderWindow.h
*
*  @brief      Class to display the height map
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
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QVector3D>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <string>
#include <vector>
#include <QKeyEvent>

#include "OpenGLWindow.h"
#include "DepthMap.h"

//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//==============================================================================
/**
*  @class  RenderWindow
*  @brief  RenderWindow is a class to render to the screen
*/
//==============================================================================
class RenderWindow : public OpenGLWindow {

public:
	
	//--------------------------------------------------------------------------
	///Overloaded constructor with the position, the colours and the normal vector of each vertex, 
	///the number of vertices, the width and the lenght of the model to display
	/**
	*  @param vertexData: the position of the vertices
	*  @param colourData: the color of the vertices
	*  @param normalData: the normal vectors of the vertices
	*  @param count: the number of vertices
	*  @param width: the width of the object to render
	*  @param length: the length
	*/
	//--------------------------------------------------------------------------
	RenderWindow(vector<QVector3D> const& vertexData,
		vector<QVector3D> const& colourData, vector<QVector3D> const& normalData, 
        int count, float length, float width);
	
	virtual ~RenderWindow();

	//--------------------------------------------------------------------------
	/// Initialize the programs and the buffers
	//--------------------------------------------------------------------------
	virtual void initializeOpenGL();

	//--------------------------------------------------------------------------
	/// display on the window
	//--------------------------------------------------------------------------
	virtual void render();
	
	//--------------------------------------------------------------------------
	/// set the visibility of the lvl plan to false if it was true and vice versa
	//--------------------------------------------------------------------------	
	void changeLvlPlanVisibility();

	//******************************************************************************
private:
	//No copy constructor
	RenderWindow(RenderWindow const&);

	//No default constructor
	RenderWindow();

	//--------------------------------------------------------------------------
	///event to control the display 
	///rotation of the camera thanks to A and D, 
	///Enable/disable lvl plan display: spacebarre
	///change the height of the lvl plan: W and S
	///Rotation of the light source: arrows
	//--------------------------------------------------------------------------
	void keyPressEvent(QKeyEvent *event);

	//--------------------------------------------------------------------------
	///Rotate the light source and rebuild the shadow
	//--------------------------------------------------------------------------
	void rotateLightSource(float angle, float x, float y, float z);

	//--------------------------------------------------------------------------
	///read the shader file to crate a program
	/**
	*  @param shaderFileName: the name of the shader file to load
	*/
	//--------------------------------------------------------------------------
	static string readShaderFile(string const& shaderFileName);

	//IDs for inputs in the ground display program
	GLuint m_verticesDisplayPositionID, //ID of the position of the vertex
		m_verticesColourID, //ID of the colour of the vertex
		m_verticesNormalID, //ID of the normal vector
		m_lightDirID, //ID of the light direction vector (from the vertex to the light)
		m_mvpMatrixID, //ID of the Model view position matrix
		m_cameraPosID, //ID of the position of the camera
		m_shadowMapDisplayMatrixID, //ID of the projection matrix of the shadow map 
		m_shadowMapTextureID; //ID of the texture of the shadow map

	//IDs for inputs in the lvl plan display program
	GLuint m_verticesLvlPlanPositionID,//ID of the position of the vertex
		m_mvpLvlPlanMatrixID;//ID of the Model view position matrix


	QOpenGLShaderProgram *m_displayProgram, // The render program displaying on the screen
		*m_lvlProgram, //Program to display the lvl plan
		*m_mapProgram;//the program to create the shadow map and the ambiant occlusion map
	
	DepthMap m_shadowMap; //object containing buffers and program to create the shadow map

	int m_dataCount; //number of vertices

	vector<QVector3D> m_verticesData, //The position of the vertices
		m_colourData, //the colours 
		m_normalData; //the normal vectors of the triangles associated to the vertices

	QVector3D m_lightDir;//the direction of the light (from the vertex to the light)

	QMatrix4x4 m_shadowMapMatrix,	//the  matrix from the light's point of view to create the shadow map  
		m_pMatrix, //the projection matrix for the camera to display on the window
		m_vMatrix, //the view matrix
		m_mMatrix; //the model matrix

	float m_lvlPlanHeight, //Height of the lvl Plan
        m_length, //length of the model
        m_width, //width of the model
		m_shadowMatrixSide; //size of the cube that the shadow map take into account

	bool m_LvlPlanVisibility; //to chose if the lvl plan has to be displayed
};

#endif //RENDERWINDOW_H
