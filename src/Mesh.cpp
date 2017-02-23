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
#include "Mesh.h"

//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//------------------------------------------------------------------------------
Mesh::Mesh():
//------------------------------------------------------------------------------
m_verticesCount(0)
//------------------------------------------------------------------------------
{

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

