#ifndef MAP_H
#define MAP_H

/**
*******************************************************************************
*
*  @file       Map.h
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
//  Include
//******************************************************************************
#include <string>
#include <vector>
#include <QtGui/QOpenGLShaderProgram>
#include <QVector3D>


//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//==============================================================================
/**
*  @class  Map
*  @brief  Map is a class to load a height map file.
*/
//==============================================================================
class Map{
public:

	//--------------------------------------------------------------------------
	///Overloaded constructor with the name of the file
	/**
	*  @param fileName: the name of the height map file
	*/
	//--------------------------------------------------------------------------
	Map(string const& fileName);

	virtual ~Map();

    //Calculate and get the size of the sides of the heightmap's mesh
    float getLength() const;
    float getWidth() const;

	//getters
	vector<QVector3D > getVerticeMap() const;
	vector<QVector3D > getColourMap() const;
	vector<QVector3D > getNormalMap() const;

	int getVerticeCount() const;
	int getN() const;
	int getM() const;

private:
	//No default constructor
	Map();

    //--------------------------------------------------------------------------
    ///translate the vector read into three vector<QVector3D>
    ///that can be exploited by the rendering window (position, colour and normal vector)
    //--------------------------------------------------------------------------
    void create(vector<vector<float>> const& map);

    //--------------------------------------------------------------------------
    /// shareNormalVectors
    //--------------------------------------------------------------------------
    void shareNormalVectors();

	vector<QVector3D > m_verticesMap, //Position of the vertices
		m_colourMap, 
		m_normalMap; //normal vector
	
	int m_n, //number of rows 
		m_m, //number of columns
		m_verticesCount; //number of vertices
};

#endif //MAP_H
