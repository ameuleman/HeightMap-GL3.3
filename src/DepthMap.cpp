/**
*******************************************************************************
*
*  @file       DepthMap.cpp
*
*  @brief      Class to render to a buffer in order to create, for exemple, a shadow map.
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <fstream>
#include <QOpenGLFunctions>

#include "DepthMap.h"

//******************************************************************************
//  constant variables
//******************************************************************************
//size of the side of the buffer
const unsigned int MAP_SIZE = 4096;

//------------------------------------------------------------------------
DepthMap::DepthMap() :
//------------------------------------------------------------------------
	m_isInitialized(false),
	m_matrixID(0),
	m_mapFrameBuffer(0),
	m_mapTexture(0)
//------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------
DepthMap::~DepthMap()
//------------------------------------------------------------------------
{
	glDeleteFramebuffers(1, &m_mapFrameBuffer);
	glDeleteTextures(1, &m_mapTexture);
}

//------------------------------------------------------------------------
void DepthMap::initialize()
//------------------------------------------------------------------------
{
	initializeOpenGLFunctions();

	//Intialize the buffer for the depth map texture and enable
	//sampling in the fragment shader as a sampler2DShadow
	glGenTextures(1, &m_mapTexture);
	glBindTexture(GL_TEXTURE_2D, m_mapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
				 MAP_SIZE, MAP_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	//initalize the frame buffer for the shadow map
	glGenFramebuffers(1, &m_mapFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_mapFrameBuffer);

	//link the frame buffer and the texture buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
						   m_mapTexture, 0);

	//check the creation of the buffer
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Depth map frame buffer creation incomplete");

	// Cleanup for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	m_isInitialized = true;
}

//------------------------------------------------------------------------
void DepthMap::render(Mesh &mesh, QMatrix4x4 const& matrix,
					  std::unique_ptr<QOpenGLShaderProgram> const& program)
//------------------------------------------------------------------------
{
	//Initialize if necessary
	if (!m_isInitialized) {
		initialize();
	}

	//link the atribute to their IDs
	m_matrixID = program->uniformLocation("matrix");

	//render to the map buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_mapFrameBuffer);
	glViewport(0, 0, MAP_SIZE, MAP_SIZE);

	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//use the depth map shader program
	program->bind();

	//send the matrix to the program
	program->setUniformValue(m_matrixID, matrix);

	mesh.render();

	program->release();
}

//------------------------------------------------------------------------
GLuint DepthMap::getMapTexture() const
//------------------------------------------------------------------------
{
	return m_mapTexture;
}
