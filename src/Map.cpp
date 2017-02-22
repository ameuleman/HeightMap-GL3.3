/**
*******************************************************************************
*
*  @file       Map.cpp
*
*  @brief      Class to load a height map file to displpay it later thanks to OpenGL
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

#include "Map.h"

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
Map::Map(string const& fileName) :
//------------------------------------------------------------------------------
	m_verticesCount(0)
//------------------------------------------------------------------------------
{

	// Open the file
	ifstream input(fileName, ios::in);

	assert(input);

	//read the number of rows and columns
	input >> m_m >> m_n;

	//allocate the vector
	vector<vector<float>> map(m_n, vector<float>(m_m));

	//read the map itself
	for (int i = 0; i < m_n; i++) {
		for (int j = 0; j < m_m; j++) {
			input >> map[i][j];
		}
	}

	//create m_verticesMap, m_colourMap, m_normalMap and m_verticesCount thanks to the read data
	create(map);

	input.close();
}

//------------------------------------------------------------------------------
Map::~Map()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
float Map::getLength() const
//------------------------------------------------------------------------------
{
    return SIDE_FACTOR * m_n / max(m_n,m_m);
}

//------------------------------------------------------------------------------
float Map::getWidth() const
//------------------------------------------------------------------------------
{
    return SIDE_FACTOR * m_m / max(m_n,m_m);
}

//------------------------------------------------------------------------------
vector<QVector3D > Map::getVerticeMap() const 
//------------------------------------------------------------------------------
{
	return m_verticesMap;
}

//------------------------------------------------------------------------------
vector<QVector3D > Map::getColourMap() const 
//------------------------------------------------------------------------------
{
	return m_colourMap;
}

//------------------------------------------------------------------------------
vector<QVector3D > Map::getNormalMap() const 
//------------------------------------------------------------------------------
{
	return m_normalMap;
}

//------------------------------------------------------------------------------
int Map::getVerticeCount() const 
//------------------------------------------------------------------------------
{
	return m_verticesCount;
}

//------------------------------------------------------------------------------
int Map::getN() const 
//------------------------------------------------------------------------------
{
	return m_n;
}

//------------------------------------------------------------------------------
int Map::getM() const 
//------------------------------------------------------------------------------
{
	return m_m;
}


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
			{
				return (true);
			}
			else
			{
				return (false);
			}
		}

        if (fabs(aLeftNode.y() - aRightNode.y()) > BIAS)
		{
			if (aLeftNode.y() < aRightNode.y())
			{
				return (true);
			}
			else
			{
				return (false);
			}
		}

        if (fabs(aLeftNode.z() - aRightNode.z()) > BIAS)
		{
			if (aLeftNode.z() < aRightNode.z())
			{
				return (true);
			}
			else
			{
				return (false);
			}
		}
		return (false);
	}
};


class TempRecord
{
public:
	inline const TempRecord& operator +=(const QVector3D& aNormal)
	{
		m_normal += aNormal;

		return (*this);
	}


	inline const TempRecord& operator =(unsigned int anID)
	{
		m_id = anID;

		return (*this);
	}

	QVector3D m_normal;
	unsigned int m_id;
};

//----------------------------------------------
void Map::shareNormalVectors()
//----------------------------------------------
{
	// Add the vertices to the hash table
	std::map<QVector3D, TempRecord, VectorComparer> hash_table;

	//Compute the vertices normal vectors (sum of the adjacent faces normal vectors)
	for (unsigned int i(0); i < m_verticesCount; ++i)
	{
		QVector3D current_vertex(m_verticesMap[i]);
		QVector3D normal(m_normalMap[i]);

		hash_table[current_vertex].m_normal += normal;
	}

	//set the vertices normal data
	for (unsigned int i(0); i < m_verticesCount; ++i)
	{
		QVector3D current_vertex(m_verticesMap[i]);
		QVector3D current_normal(hash_table[current_vertex].m_normal);

		m_normalMap[i].setX(current_normal.x());
		m_normalMap[i].setY(current_normal.y());
		m_normalMap[i].setZ(current_normal.z());

		m_normalMap[i].normalize();
	}
}

//------------------------------------------------------------------------------
void Map::create(vector<vector<float>> const& map)
//------------------------------------------------------------------------------
{
    m_verticesMap.reserve(m_n*m_m*6);
    m_colourMap.reserve(m_n*m_m*6);

    //multiply the x and y position of each vertex by this value
    float size(SIDE_FACTOR/(float(max(m_n, m_m))));

    for (int i = 0; i < m_n - 1; i++) {
		for (int j = 0; j < m_m - 1; j++) {

            float x = i * size;
            float dx = 1 * size;
            float y = j * size;
            float dy = 1 * size;

			//extract three vertices
            QVector3D v1(x, y, map[i][j] * HEIGHT_FACTOR);
            QVector3D v2(x + dx, y, map[i + 1][j] * HEIGHT_FACTOR);
            QVector3D v3(x + dx, y + dy, map[i + 1][j + 1] * HEIGHT_FACTOR);
            QVector3D v4(x, y + dy, map[i][j + 1] * HEIGHT_FACTOR);

            //Generate the color depending on the height
            QVector3D c1(map[i][j], 0, 1 - map[i][j]);
            QVector3D c2(map[i + 1][j], 0, 1 - map[i + 1][j]);
            QVector3D c3(map[i + 1][j + 1], 0, 1 - map[i + 1][j + 1]);
            QVector3D c4(map[i][j + 1], 0, 1 - map[i][j + 1]);

			//the first triangle
			m_verticesMap.push_back(v1);
			m_verticesMap.push_back(v2);
			m_verticesMap.push_back(v3);

            m_colourMap.push_back(c1);
            m_colourMap.push_back(c2);
            m_colourMap.push_back(c3);

			//the second triangle
			m_verticesMap.push_back(v1);
			m_verticesMap.push_back(v3);
			m_verticesMap.push_back(v4);

            m_colourMap.push_back(c1);
            m_colourMap.push_back(c3);
            m_colourMap.push_back(c4);

			//the order of the vertices is important to calculate the right normal vector
		}
	}

    m_verticesCount = int(m_verticesMap.size());

	m_normalMap.reserve(m_verticesCount);

	//all the vertices are grey
	m_colourMap.resize(m_verticesCount, 0.8f*QVector3D(1.f, 1.f, 1.f));

	//create the normal vector for each triangle
    for (int i = 0; i < m_verticesCount; i += 3) {
		QVector3D normal(QVector3D::crossProduct((m_verticesMap[i + 1] - m_verticesMap[i]), (m_verticesMap[i + 2] - m_verticesMap[i])));
		normal.normalize();
		for (int l = 0; l < 3; l++) {
			m_normalMap.push_back(normal);
		}
	}

    //change from one normal per face to one normal per vertex
    shareNormalVectors();
}
