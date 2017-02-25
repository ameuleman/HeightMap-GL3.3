#ifndef DepthMap_H
#define DepthMap_H

/**
*******************************************************************************
*
*  @file       DepthMap.h
*
*  @brief      Class to render to a buffer in order to create, for exemple, a shadow map.
*
*  @version    1.0
*
*  @date       22/06/2016
*
*  @author     Andr�as Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QMatrix4x4>
#include <QOpenGLTexture>
#include <vector>

#include "Mesh.h"

//==============================================================================
/**
*  @class  DepthMap
*  @brief  DepthMap is a class to render to a buffer in order to create, for exemple, a shadow map.
*/
//==============================================================================
class DepthMap: protected QOpenGLFunctions {

public:

	DepthMap();

	~DepthMap();

	//--------------------------------------------------------------------------
	///Initialize the buffers
	//--------------------------------------------------------------------------
	void initialize();

	//--------------------------------------------------------------------------
	///render to the frame buffer, call initialize() if it has never been called before
	/**
	*  @param verticesData: the position of the vertices
	*  @param dataCount: the pnumber of vertices
	*  @param matrix: the projection matrix
	*/
	//--------------------------------------------------------------------------
    void render(Mesh &mesh, QMatrix4x4 const& matrix, QOpenGLShaderProgram * program);

	//--------------------------------------------------------------------------
	///Get the texture
	//--------------------------------------------------------------------------
	GLuint getMapTexture() const;

	//******************************************************************************
private:
	//No copy constructor
	DepthMap(DepthMap const&);

    static const int MAP_SIZE = 4096; //size of the side of the buffer

	bool m_isInitialized;//to know if initialize() has been called

	//IDs for inputs in the map program
    GLuint m_matrixID; //ID of the MVP matrix

	//To create a buffer for the scene map
	GLuint	m_mapFrameBuffer,
		m_mapTexture;

};

#endif //DepthMap_H
