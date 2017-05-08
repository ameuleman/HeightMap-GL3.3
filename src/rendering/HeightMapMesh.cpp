/**
*******************************************************************************
*
*  @file       HeightMapMesh.cpp
*
*  @brief      Class to create a height map mesh thanks to a file or data
* to displpay it thanks to OpenGL.
* Produce a mesh contained in a cuboid of size 150 * 150 * 50
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//	Include
//******************************************************************************
#include <iostream>
#include <fstream>
#include <cassert>

#include "tools/ParallelTool.h"
#include "HeightMapMesh.h"

//******************************************************************************
//  constant variables
//******************************************************************************
//give the size of the greater side of the mesh
const float SIDE_FACTOR = 300.f;

//Multiply the height by this value
const float HEIGHT_FACTOR = 50.f;

//------------------------------------------------------------------------------
HeightMapMesh::HeightMapMesh(std::string const& fileName)
//------------------------------------------------------------------------------
{
	// Open the file
	std::ifstream input(fileName, std::ios::in);

	Types::float_matrix imageData;

	if(input)
	{
		//read the number of rows and columns
		input >> m_m >> m_n;

		//allocate the vector
		imageData.resize(m_n, Types::float_line(m_m));

		//read the imageData itself
		for (unsigned int i(0); i < m_n; i++) {
			for (unsigned int j(0); j < m_m; j++) {
				input >> imageData[i][j];
			}
		}

		input.close();
	}
	else
		throw std::runtime_error("Cannot open " + fileName);


	//create m_verticesPosition, m_verticesColour, m_verticesNormal
	//and m_verticesCount thanks to the data
	create(imageData);
}

//------------------------------------------------------------------------------
HeightMapMesh::HeightMapMesh(Types::float_matrix const& imageData,
							 unsigned int n, unsigned int m):
//------------------------------------------------------------------------------
	m_n(n),
	m_m(m)
//------------------------------------------------------------------------------
{
	//create m_verticesPosition, m_verticesColour, m_verticesNormal
	//and m_verticesCount thanks to the data
	create(imageData);
}

//------------------------------------------------------------------------------
HeightMapMesh::~HeightMapMesh()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
float HeightMapMesh::getLength() const
//------------------------------------------------------------------------------
{
	return SIDE_FACTOR * m_n / std::max(m_n,m_m);
}

//------------------------------------------------------------------------------
float HeightMapMesh::getWidth() const
//------------------------------------------------------------------------------
{
	return SIDE_FACTOR * m_m / std::max(m_n,m_m);
}


//------------------------------------------------------------------------------
unsigned int HeightMapMesh::getN() const
//------------------------------------------------------------------------------
{
	return m_n;
}

//------------------------------------------------------------------------------
unsigned int HeightMapMesh::getM() const
//------------------------------------------------------------------------------
{
	return m_m;
}



//------------------------------------------------------------------------------
void HeightMapMesh::create(Types::float_matrix const& imageData)
//------------------------------------------------------------------------------
{
	m_verticesCount = (m_n - 1) * (m_m - 1) * 6;

	m_verticesNormal.resize(m_verticesCount);
	m_verticesPosition.resize(m_verticesCount);
	m_verticesColour.resize(m_verticesCount);

	if(m_n != 0 && m_m != 0 && imageData.size() == m_n && imageData[0].size() == m_m)
	{
		float size(SIDE_FACTOR/(float(std::max(m_n, m_m))));

		ParallelTool::performInParallel(
			[this, size, &imageData](unsigned int leftIndex, unsigned int rightIndex)
			{
				generateVertices(size, imageData, leftIndex, rightIndex);
			},
			0, m_n - 1);
	}
	else
	{
		throw std::runtime_error("Wrong data, cannot create the model");
	}
}

//------------------------------------------------------------------------------
void HeightMapMesh::generateVertices(float size, const Types::float_matrix &imageData,
									 unsigned int leftIndex, unsigned int rightIndex)
//------------------------------------------------------------------------------
{
	for (unsigned int i(leftIndex); i < rightIndex; i++) {
		for (unsigned int j(0); j < m_m - 1; j++) {

			float x = i * size;
			float dx = 1 * size;
			float y = j * size;
			float dy = 1 * size;

			//extract three vertices
			QVector3D v1(x, y, imageData[i][j] * HEIGHT_FACTOR);
			QVector3D v2(x + dx, y, imageData[i + 1][j] * HEIGHT_FACTOR);
			QVector3D v3(x + dx, y + dy, imageData[i + 1][j + 1] * HEIGHT_FACTOR);
			QVector3D v4(x, y + dy, imageData[i][j + 1] * HEIGHT_FACTOR);

			//Generate the color depending on the height
			QVector3D c1(imageData[i][j], 0, 1 - imageData[i][j]);
			QVector3D c2(imageData[i + 1][j], 0, 1 - imageData[i + 1][j]);
			QVector3D c3(imageData[i + 1][j + 1], 0, 1 - imageData[i + 1][j + 1]);
			QVector3D c4(imageData[i][j + 1], 0, 1 - imageData[i][j + 1]);

			int index(6 * (i * (m_m - 1) + j));
			//the first triangle
			m_verticesPosition[index] = (v1);
			m_verticesPosition[index + 1] = (v2);
			m_verticesPosition[index + 2] = (v3);

			m_verticesColour[index] = (c1);
			m_verticesColour[index + 1] = (c2);
			m_verticesColour[index + 2] = (c3);

			//the second triangle
			m_verticesPosition[index + 3] = (v1);
			m_verticesPosition[index + 4] = (v3);
			m_verticesPosition[index + 5] = (v4);

			m_verticesColour[index + 3] = (c1);
			m_verticesColour[index + 4] = (c3);
			m_verticesColour[index + 5] = (c4);

			//the order of the vertices is important to calculate the right normal vector

			//create the normal vector for each triangle
			QVector3D normal(QVector3D::crossProduct(
				(v2 - v1),
				(v3 - v1)));
			normal.normalize();
			for (unsigned int l(0); l < 3; l++)
				m_verticesNormal[index + l] = (normal);

			normal = QVector3D::crossProduct(
				(v3 - v1),
				(v4 - v1));
			normal.normalize();
			for (unsigned int l(3); l < 6; l++)
				m_verticesNormal[index + l] = (normal);
		}
	}
}
