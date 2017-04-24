
#ifndef HEIGHTMAPMESH_H
#define HEIGHTMAPMESH_H

/**
*******************************************************************************
*
*  @file       HeightMapMesh.h
*
*  @brief      Class to create a height map mesh thanks to a file or data
*               to displpay it thanks to OpenGL
*
*  @author     Andréas Meuleman
*******************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <string>
#include <vector>
#include <QtGui/QOpenGLShaderProgram>
#include <QVector3D>

#include "Types.h"
#include "Mesh.h"

//==============================================================================
/**
*  @class  HeightMapMesh
*  @brief  HeightMapMesh is a class to create a height map mesh thanks to a file or data
*               to displpay it thanks to OpenGL
*/
//==============================================================================
class HeightMapMesh: public Mesh{
public:

	/**
	 * @brief HeightMapMesh Overloaded constructor with the name of the file.
	 * The file has to contain the width, the height and then the data in the [0,1] range
	 * @param fileName the name of the height map file
	 */
	HeightMapMesh(std::string const& fileName);

	/**
	 * @brief HeightMapMesh Overloaded constructor with the image size and data
	 * @param imageData the data of the image as floats in the [0,1] range
	 * @param n height of the image
	 * @param m width of the image
	 */
	HeightMapMesh(const Types::float_matrix &imageData, unsigned int n, unsigned int m);

	virtual ~HeightMapMesh();

	/**
	 * @brief getLength Calculate the length of the heightmap's mesh
	 * @return the length of the heightmap's mesh
	 */
	float getLength() const;

	/**
	 * @brief getWidth Calculate the width of the heightmap's mesh
	 * @return the width of the heightmap's mesh
	 */
	float getWidth() const;

	//Getters
	unsigned int getN() const;
	unsigned int getM() const;

//******************************************************************************
private:
	//No default constructor
	HeightMapMesh();

	//No copy constructor
	HeightMapMesh(HeightMapMesh const&);

	/**
	 * @brief create Create the mesh
	 * @param imageData the data of the image as floats in the [0,1] range
	 */
	void create(Types::float_matrix const& imageData);

	/**
	 * @brief generateVertices translate the vector read into three vector<QVector3D>
	 * that can be exploited by the rendering window (position, colour and normal vectors)
	 * Proceed between two values to enable parallel processing
	 * @param size multiply the position of all vertices by this value
	 * @param imageData the data of the image as floats in the [0,1] range
	 * @param leftIndex proceed from this index
	 * @param rightIndex to this index
	 */
	void generateVertices(float size, Types::float_matrix const& imageData,
						  unsigned int leftIndex, unsigned int rightIndex);

	unsigned int m_n, //number of rows
		m_m; //number of columns
};

#endif //HEIGHTMAPMESH_H
