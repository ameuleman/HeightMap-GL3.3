/**
*******************************************************************************
*
*  @file       Mesh.cpp
*
*  @brief      Class to handel a mesh to displpay it thanks to OpenGL
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QtGui/QOpenGLShaderProgram>
#include <math.h>
#include <iostream>

#include "ParallelTool.h"
#include "Mesh.h"

//------------------------------------------------------------------------------
Mesh::Mesh():
//------------------------------------------------------------------------------
m_verticesCount(0),
m_positionBuffer(0),
m_normalBuffer(0),
m_colourBuffer(0),
m_indexBuffer(0),
m_isInitialized(false),
m_hasNormalData(false),
m_hasColourData(false),
m_usesIndex(false)
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
Mesh::~Mesh()
//------------------------------------------------------------------------------
{
	cleanUpVBO();
}

//------------------------------------------------------------------------------
void Mesh::initialize()
//------------------------------------------------------------------------------
{
	if(!m_usesIndex)
	{
		m_hasNormalData = (m_verticesNormal.size() > 0);
		m_hasColourData = (m_verticesColour.size() > 0);
	}

	initializeOpenGLFunctions();

	updateVBO();
	m_isInitialized = true;
}

//------------------------------------------------------------------------------
void Mesh::updateVBO()
//------------------------------------------------------------------------------
{
	cleanUpVBO();

	//Create VBO thanks to the data
	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_verticesPosition.size() * 3 * sizeof(float),
				 &m_verticesPosition[0], GL_STATIC_DRAW);

	if(m_hasNormalData)
	{
		glGenBuffers(1, &m_normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_verticesNormal.size() * 3 * sizeof(float),
					 &m_verticesNormal[0], GL_STATIC_DRAW);
	}

	if(m_hasColourData)
	{
		glGenBuffers(1, &m_colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_verticesColour.size() * 3 * sizeof(float),
					 &m_verticesColour[0], GL_STATIC_DRAW);
	}

	if(m_usesIndex)
	{
		glGenBuffers(1, &m_indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_verticesCount * sizeof(float),
					 &m_verticesIndex[0], GL_STATIC_DRAW);
	}
}

//------------------------------------------------------------------------------
void Mesh::render()
//------------------------------------------------------------------------------
{
	try
	{

		//initialize if necessary
		if(!m_isInitialized)
		{
			initialize();
		}

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		if(m_hasNormalData)
		{
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if(m_hasColourData)
		{
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}

		if(m_usesIndex)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

			//draws the triangle on the window
			glDrawElements(GL_TRIANGLES, m_verticesCount, GL_UNSIGNED_INT, (void*)0);
		}
		else
		{
			//draws the triangle on the window
			glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
		}

		//Disable
		glDisableVertexAttribArray(0);

		if(m_hasNormalData)
			glDisableVertexAttribArray(1);

		if(m_hasColourData)
			glDisableVertexAttribArray(2);
	}
	catch(std::exception const& e)
	{
        std::cerr << "ERROR : " << e.what() << std::endl;
	}
}

///@cond
/**
 * @brief The VectorComparer struct Enable to compare two vectors
 * to create a hash table containing vertices
 */
struct VectorComparer
{
	// Return true if aLeftNode is less than aRightNode
	bool operator()(const QVector3D & aLeftNode, const QVector3D & aRightNode) const
	{
		//Bias to compensate float imprecision
		const float BIAS = 0.001f;

		if (fabs(aLeftNode.x() - aRightNode.x()) > BIAS)
		{
			if (aLeftNode.x() < aRightNode.x())
				return (true);
			else
				return (false);
		}


		if (fabs(aLeftNode.y() - aRightNode.y()) > BIAS)
		{
			if (aLeftNode.y() < aRightNode.y())
				return (true);
			else
				return (false);
		}


		if (fabs(aLeftNode.z() - aRightNode.z()) > BIAS)
		{
			if (aLeftNode.z() < aRightNode.z())
				return (true);
			else
				return (false);
		}

		return (false);
	}
};

/**
 * @brief The VertexData class store some data of a vertex:
 * its normal vector, colour and id for the index
 */
class VertexData
{
public:
	QVector3D m_normal;
	QVector3D m_colour;
	unsigned int m_id;
};
///@endcond

//----------------------------------------------
void Mesh::setIndex()
//----------------------------------------------
{
	if(!m_usesIndex)
	{
		if(!m_isInitialized)
		{
			m_hasNormalData = (m_verticesNormal.size() > 0);
			m_hasColourData = (m_verticesColour.size() > 0);
		}

		// Add the vertices to the hash table
		std::map<QVector3D, VertexData, VectorComparer> hash_table;

		VertexData *hashTableElement;

		//Compute the vertices normal vectors
		//(sum of the adjacent faces normal vectors)
		//and set the hash table
		for (unsigned int i(0); i < m_verticesCount; i++)
		{
			QVector3D currentVertex(m_verticesPosition[i]);
			QVector3D normal;
			QVector3D colour;

			if(m_hasNormalData)
				normal = m_verticesNormal[i];

			if(m_hasColourData)
				colour = m_verticesColour[i];

			hashTableElement = (&hash_table[currentVertex]);

			hashTableElement->m_normal += normal;
			hashTableElement->m_colour = colour;
		}

		unsigned int dataSize = (unsigned int)(hash_table.size());

		//set the vertices normal and colour data
		if(m_hasNormalData)
		{
			m_verticesNormal.clear();
			m_verticesNormal.resize(dataSize);
		}

		if(m_hasColourData)
		{
			m_verticesColour.clear();
			m_verticesColour.resize(dataSize);
		}

		unsigned int id(0);

		for (std::map<QVector3D, VertexData, VectorComparer>::iterator ite(
				 hash_table.begin()); ite != hash_table.end(); ++ite, ++id)
		{
			ite->second.m_id = id;

			if(m_hasNormalData)
				m_verticesNormal[id] = ite->second.m_normal.normalized();

			if(m_hasColourData)
				m_verticesColour[id] = ite->second.m_colour;
		}

		//Set the index
		m_verticesIndex.clear();
		m_verticesIndex.resize(m_verticesCount);

		ParallelTool::performInParallel(
			[this, &hash_table, &hashTableElement](unsigned int leftIndex, unsigned int rightIndex)
			{
			for (unsigned int i(leftIndex); i < rightIndex; i++)
			{
				m_verticesIndex[i] = hash_table[m_verticesPosition[i]].m_id;
			}
			},
			0, m_verticesCount);


		//set the vertices position data
		m_verticesPosition.clear();
		m_verticesPosition.resize(dataSize);

		id = 0;
		for (std::map<QVector3D, VertexData, VectorComparer>::iterator
			 ite(hash_table.begin()); ite != hash_table.end(); ++ite, ++id)
		{
			m_verticesPosition[id] = ite->first;
		}

		if(m_isInitialized)
			updateVBO();

		m_usesIndex = true;
	}
}

void Mesh::cleanUpVBO()
{
	if(m_isInitialized)
	{
		// Cleanup VBO if needed
		glDeleteBuffers(1, &m_positionBuffer);

		if(m_hasNormalData)
			glDeleteBuffers(1, &m_normalBuffer);

		if(m_hasColourData)
			glDeleteBuffers(1, &m_colourBuffer);

		if(m_verticesIndex.size())
			glDeleteBuffers(1, &m_indexBuffer);
	}
}

//------------------------------------------------------------------------------
std::vector<QVector3D > Mesh::getVerticesPosition() const
//------------------------------------------------------------------------------
{
	return m_verticesPosition;
}

//------------------------------------------------------------------------------
std::vector<QVector3D > Mesh::getVerticesColour() const
//------------------------------------------------------------------------------
{
	return m_verticesColour;
}

//------------------------------------------------------------------------------
std::vector<QVector3D > Mesh::getVerticesNormal() const
//------------------------------------------------------------------------------
{
	return m_verticesNormal;
}

//------------------------------------------------------------------------------
unsigned int Mesh::getVerticeCount() const
//------------------------------------------------------------------------------
{
	return m_verticesCount;
}
