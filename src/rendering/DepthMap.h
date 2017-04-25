#ifndef DEPTHMAP_H
#define DEPTHMAP_H

/**
*******************************************************************************
*
*  @file       DepthMap.h
*
*  @brief      Class to render to a buffer in order to create, for exemple, a shadow map.
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QMatrix4x4>
#include <QOpenGLTexture>
#include <vector>
#include <memory>

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

	/**
	 * @brief initialize Initialize the buffers
	 */
	void initialize();

	/**
	 * @brief render render to the frame buffer,
	 * call initialize() if it has never been called before
	 * @param mesh the mesh to be rendered
	 * @param matrix the projection matrix
	 * @param program the OpenGL shader program to create the depth map
	 */
	void render(Mesh &mesh, QMatrix4x4 const& matrix,
				std::unique_ptr<QOpenGLShaderProgram> const& program);

	/**
	 * @brief getMapTexture
	 * @return the depth map as a texture
	 */
	GLuint getMapTexture() const;

//******************************************************************************
private:
	//No copy constructor
	DepthMap(DepthMap const&);

	//to know if initialize() has been called
	bool m_isInitialized;

	//ID of the MVP matrix for inputs in the shader program
	GLuint m_matrixID;

	//To create a buffer for the depth map
	GLuint	m_mapFrameBuffer,
		m_mapTexture;

};

#endif //DEPTHMAP_H
