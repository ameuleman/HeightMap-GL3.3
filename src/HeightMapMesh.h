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

	//--------------------------------------------------------------------------
	/// Overloaded constructor with the name of the file. The file has to contain
	/// the width, the height and then the data in the [0,1] range
	/**
	*  @param fileName: the name of the height map file
	*/
	//--------------------------------------------------------------------------
	HeightMapMesh(std::string const& fileName);

	//--------------------------------------------------------------------------
	/// Overloaded constructor with the image size and data
	/**
	*  @param imageData: the data of the image as floats in the [0,1] range
	*  @param n: height of the image
	*  @param m: width of the image
	*/
	//--------------------------------------------------------------------------
	HeightMapMesh(std::vector<std::vector<float>> const& imageData, unsigned int n, unsigned int m);

	virtual ~HeightMapMesh();

	//Calculate and get the size of the sides of the heightmap's mesh
	float getLength() const;
	float getWidth() const;

	//Getters
	unsigned int getN() const;
	unsigned int getM() const;

//******************************************************************************
private:
	//No default constructor
	HeightMapMesh();

	//--------------------------------------------------------------------------
	///Create the mesh
	/**
	*  @param imageData: the data of the image as floats in the [0,1] range
	*/
	//--------------------------------------------------------------------------
	void create(std::vector<std::vector<float>> const& imageData);

	//--------------------------------------------------------------------------
	///translate the vector read into three vector<QVector3D>
	///that can be exploited by the rendering window (position, colour and normal vectors)
	/// Proceed between two values to enable parallel processing
	/**
	* @param imageData: the data of the image as floats in the [0,1] range
	* @param leftIndex: proceed from this index
	* @param rightIndex: to this index
	*/
	//--------------------------------------------------------------------------
	void generateVertices(float size, std::vector<std::vector<float>> const& imageData,
						  unsigned int leftIndex, unsigned int rightIndex);

	unsigned int m_n, //number of rows
		m_m; //number of columns
};

#endif //HEIGHTMAPMESH_H
