#ifndef MESH_H
#define MESH_H

/**
*******************************************************************************
*
*  @file       Mesh.cpp
*
*  @brief      Class to handel a mesh to displpay it thanks to OpenGL
*
*  @version    1.0
*
*  @date       23/02/2017
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QVector3D>
#include <vector>

//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//==============================================================================
/**
*  @class  Mesh
*  @brief  Mesh is a class to handel a mesh to displpay it thanks to OpenGL
*/
//==============================================================================
class Mesh
{
public:
    Mesh();

    //getters
    vector<QVector3D > getVerticesPosition() const;
    vector<QVector3D > getVerticesColour() const;
    vector<QVector3D > getVerticesNormal() const;

    unsigned int getVerticeCount() const;

protected:
    //--------------------------------------------------------------------------
    /// change from one normal per face to one normal per vertex
    //--------------------------------------------------------------------------
    void shareNormalVectors();

    vector<QVector3D > m_verticesPosition, //Position of the vertices
        m_verticesColour,
        m_verticesNormal; //normal vector

    unsigned int m_verticesCount; //number of vertices


};

#endif // MESH_H
