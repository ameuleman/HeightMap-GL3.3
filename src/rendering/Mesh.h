#ifndef MESH_H
#define MESH_H

/**
*******************************************************************************
*
*  @file       Mesh.h
*
*  @brief      Class to handel a mesh to display it thanks to OpenGL
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QVector3D>
#include <vector>
#include <QOpenGLFunctions>
#include <atomic>

#include "tools/Types.h"

//==============================================================================
/**
*  @class  Mesh
*  @brief  Mesh is a class to handel a mesh to display it thanks to OpenGL
*/
//==============================================================================
class Mesh: protected QOpenGLFunctions
{
public:
	Mesh();

	/**
	 * @brief ~Mesh destructor, call cleanUpVBO()
	 */
	virtual ~Mesh();

	/**
	 * @brief initialize Initialize VBO
	 */
	void initialize();

	/**
	 * @brief updateVBO VBO needs update when data change
	 */
	void updateVBO();

	/**
	 * @brief render Render the mesh in the current OpenGL context.
	 * An OpenGL shader program need to be bound before calling this function.
	 */
	void render();

	/**
	 * @brief setIndex change from one normal per face to one normal per vertex
	 * and set the index to improve performance.
	 * Do nothing if an index has already been set.
	 */
	void setIndex();

	/**
	 * @brief cleanUpVBO Clean up VBO if needed, needed before deletion
	 */
	void cleanUpVBO();

	/**
	 * @brief getVerticesPosition get data describing the mesh
	 * @return position of the vertices
	 */
	Types::vertices_data getVerticesPosition() const;

	/**
	 * @brief getVerticesColour get data describing the mesh
	 * @return colour of the vertices
	 */
	Types::vertices_data getVerticesColour() const;

	/**
	 * @brief getVerticesNormal get data describing the mesh
	 * @return Normal vectors of the vertices
	 */
	Types::vertices_data getVerticesNormal() const;

	/**
	 * @brief getVerticeCount
	 * @return number of vertices of the mesh
	 */
	unsigned int getVerticeCount() const;

protected:
	//no copy constructor
	Mesh(const Mesh&);

	Types::vertices_data m_verticesPosition, //Position of the vertices
		m_verticesColour,
		m_verticesNormal; //normal vectors

	//normal vector
	Types::uint_line m_verticesIndex;

	//number of vertices
	unsigned int m_verticesCount;

	//IDs of array buffers
	GLuint m_positionBuffer,
		m_normalBuffer,
		m_colourBuffer,
		m_indexBuffer;

	bool m_isInitialized,//to know if initialize() has been called
		m_hasNormalData,
		m_hasColourData,
		m_usesIndex;
};

#endif // MESH_H
