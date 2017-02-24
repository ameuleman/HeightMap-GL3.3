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
#include <QtGui/QOpenGLShaderProgram>

#include "Mesh.h"

//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//------------------------------------------------------------------------------
Mesh::Mesh():
//------------------------------------------------------------------------------
m_verticesCount(0),
m_positionBuffer(0),
m_normalBuffer(0),
m_colourBuffer(0),
m_isInitialized(false)
//------------------------------------------------------------------------------
{

}

//------------------------------------------------------------------------------
Mesh::~Mesh()
//------------------------------------------------------------------------------
{
    // Cleanup VBO
    if(m_isInitialized)
    {
        glDeleteBuffers(1, &m_positionBuffer);
        glDeleteBuffers(1, &m_normalBuffer);
        glDeleteBuffers(1, &m_colourBuffer);
    }
}


//------------------------------------------------------------------------------
void Mesh::initialize()
//------------------------------------------------------------------------------
{
    initializeOpenGLFunctions();
    updateVBO();
    m_isInitialized = true;
}

//------------------------------------------------------------------------------
void Mesh::updateVBO()
//------------------------------------------------------------------------------
{
    if(m_isInitialized)
    {
        // Cleanup VBO if needed
        glDeleteBuffers(1, &m_positionBuffer);
        glDeleteBuffers(1, &m_normalBuffer);
        glDeleteBuffers(1, &m_colourBuffer);
    }

    if(m_verticesPosition.size())
    {
        glGenBuffers(1, &m_positionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_verticesPosition.size() * 3 * sizeof(float),
                     &m_verticesPosition[0], GL_STATIC_DRAW);
    }

    if(m_verticesNormal.size())
    {
        glGenBuffers(1, &m_normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_verticesNormal.size() * 3 * sizeof(float),
                     &m_verticesNormal[0], GL_STATIC_DRAW);
    }

    if(m_verticesColour.size())
    {
        glGenBuffers(1, &m_colourBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_verticesColour.size() * 3 * sizeof(float),
                     &m_verticesColour[0], GL_STATIC_DRAW);
    }
}

//------------------------------------------------------------------------------
void Mesh::render()
//------------------------------------------------------------------------------
{
    //initialize if necessary
    if(!m_isInitialized)
    {
        initialize();
    }

    if(m_verticesPosition.size())
    {
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    if(m_verticesNormal.size())
    {

    }

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if(m_verticesColour.size())
    {
        // 3rd attribute buffer : normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    //draws the triangle on the window
    glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

//------------------------------------------------------------------------------
vector<QVector3D > Mesh::getVerticesPosition() const
//------------------------------------------------------------------------------
{
    return m_verticesPosition;
}

//------------------------------------------------------------------------------
vector<QVector3D > Mesh::getVerticesColour() const
//------------------------------------------------------------------------------
{
    return m_verticesColour;
}

//------------------------------------------------------------------------------
vector<QVector3D > Mesh::getVerticesNormal() const
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
void Mesh::shareNormalVectors()
//----------------------------------------------
{
    // Add the vertices to the hash table
    std::map<QVector3D, TempRecord, VectorComparer> hash_table;

    //Compute the vertices normal vectors (sum of the adjacent faces normal vectors)
    for (unsigned int i(0); i < m_verticesCount; ++i)
    {
        QVector3D current_vertex(m_verticesPosition[i]);
        QVector3D normal(m_verticesNormal[i]);

        hash_table[current_vertex].m_normal += normal;
    }

    //set the vertices normal data
    for (unsigned int i(0); i < m_verticesCount; ++i)
    {
        QVector3D current_vertex(m_verticesPosition[i]);
        QVector3D current_normal(hash_table[current_vertex].m_normal);

        m_verticesNormal[i].setX(current_normal.x());
        m_verticesNormal[i].setY(current_normal.y());
        m_verticesNormal[i].setZ(current_normal.z());

        m_verticesNormal[i].normalize();
    }
}

