/**
*******************************************************************************
*
*  @file       HeightMapMesh.cpp
*
*  @brief      Class to load a height imageData file to displpay it later thanks to OpenGL
*
*  @version    1.0
*
*  @date       16/06/2016
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

#include "HeightMapMesh.h"

//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//******************************************************************************
//  constant variables
//******************************************************************************
//give the size of the greater side of the mesh
const float SIDE_FACTOR = 300.f;

//Multiply the height by this value
const float HEIGHT_FACTOR = 50.f;

//------------------------------------------------------------------------------
HeightMapMesh::HeightMapMesh(string const& fileName)
//------------------------------------------------------------------------------
{
	// Open the file
	ifstream input(fileName, ios::in);

	assert(input);

	//read the number of rows and columns
	input >> m_m >> m_n;

	//allocate the vector
    vector<vector<float>> imageData(m_n, vector<float>(m_m));

    //read the imageData itself
    for (unsigned int i(0); i < m_n; i++) {
        for (unsigned int j(0); j < m_m; j++) {
            input >> imageData[i][j];
		}
	}

    input.close();

    //create m_verticesPosition, m_verticesColour, m_verticesNormal
    //and m_verticesCount thanks to the data
    create(imageData);
}

//------------------------------------------------------------------------------
HeightMapMesh::HeightMapMesh(vector<vector<float>> const& imageData,
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
    return SIDE_FACTOR * m_n / max(m_n,m_m);
}

//------------------------------------------------------------------------------
float HeightMapMesh::getWidth() const
//------------------------------------------------------------------------------
{
    return SIDE_FACTOR * m_m / max(m_n,m_m);
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
void HeightMapMesh::create(vector<vector<float>> const& imageData)
//------------------------------------------------------------------------------
{
    m_verticesPosition.reserve(m_n*m_m*6);
    m_verticesColour.reserve(m_n*m_m*6);

    //multiply the x and y position of each vertex by this value
    float size(SIDE_FACTOR/(float(max(m_n, m_m))));

    for (unsigned int i(0); i < m_n - 1; i++) {
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

			//the first triangle
            m_verticesPosition.push_back(v1);
            m_verticesPosition.push_back(v2);
            m_verticesPosition.push_back(v3);

            m_verticesColour.push_back(c1);
            m_verticesColour.push_back(c2);
            m_verticesColour.push_back(c3);

			//the second triangle
            m_verticesPosition.push_back(v1);
            m_verticesPosition.push_back(v3);
            m_verticesPosition.push_back(v4);

            m_verticesColour.push_back(c1);
            m_verticesColour.push_back(c3);
            m_verticesColour.push_back(c4);

			//the order of the vertices is important to calculate the right normal vector
		}
	}

    m_verticesCount = (unsigned int)(m_verticesPosition.size());

    m_verticesNormal.reserve(m_verticesCount);

	//create the normal vector for each triangle
    for (unsigned int i(0); i < m_verticesCount; i += 3) {
        QVector3D normal(QVector3D::crossProduct(
            (m_verticesPosition[i + 1] - m_verticesPosition[i]),
            (m_verticesPosition[i + 2] - m_verticesPosition[i])));
		normal.normalize();
        for (unsigned int l(0); l < 3; l++) {
            m_verticesNormal.push_back(normal);
		}
	}
}
