#ifndef MESH_H
#define MESH_H

/**
*******************************************************************************
*
*  @file       Mesh.cpp
*
*  @brief      Class to handel a mesh to displpay it thanks to OpenGL
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
#include <QOpenGLFunctions>
#include <atomic>


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
class Mesh: protected QOpenGLFunctions
{
public:
    Mesh();

    virtual ~Mesh();

    //--------------------------------------------------------------------------
    ///Initialize VBO
    //--------------------------------------------------------------------------
    void initialize();

    //--------------------------------------------------------------------------
    /// VBO needs update when data change
    //--------------------------------------------------------------------------
    void updateVBO();

    //--------------------------------------------------------------------------
    /// Render the mesh in the current OpenGL context
    //--------------------------------------------------------------------------
    void render();

    //--------------------------------------------------------------------------
    /// change from one normal per face to one normal per vertex
    /// and set the index to improve performance.
    /// Do nothing if an index has already been set.
    //--------------------------------------------------------------------------
    void setIndex();

    //--------------------------------------------------------------------------
    /// Clean up VBO if needed
    //--------------------------------------------------------------------------
    void cleanUpVBO();

    //getters
    vector<QVector3D > getVerticesPosition() const;
    vector<QVector3D > getVerticesColour() const;
    vector<QVector3D > getVerticesNormal() const;

    unsigned int getVerticeCount() const;

protected:
    vector<QVector3D > m_verticesPosition, //Position of the vertices
        m_verticesColour,
        m_verticesNormal; //normal vector

    vector<unsigned int> m_verticesIndex; //normal vector

    unsigned int m_verticesCount; //number of vertices

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
