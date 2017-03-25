/**
*******************************************************************************
*
*  @file       LvlPlan.cpp
*
*  @brief      Class to handel a lvl plan
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include "LvlPlan.h"


//------------------------------------------------------------------------------
LvlPlan::LvlPlan(float height, float length, float width):
//------------------------------------------------------------------------------
	m_height(height),
	m_length(length),
	m_width(width)
//------------------------------------------------------------------------------
{
	m_verticesPosition = {
		QVector3D(0.f, 0.f, m_height),
		QVector3D(0.f, m_width, m_height),
		QVector3D(m_length, 0.f, m_height),
		QVector3D(0.f, m_width, m_height),
		QVector3D(m_length, m_width, m_height),
		QVector3D(m_length, 0.f, m_height)
	};
	m_verticesCount = 6;
}

//------------------------------------------------------------------------------
void LvlPlan::changeHeight(float delta)
//------------------------------------------------------------------------------
{
	m_height += delta;
	for(unsigned int i(0); i < m_verticesPosition.size(); i++)
	{
		m_verticesPosition[i].setZ(m_height);
	}

	updateVBO();
}
