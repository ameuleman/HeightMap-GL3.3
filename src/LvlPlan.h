#ifndef LVLPLAN_H
#define LVLPLAN_H

/**
*******************************************************************************
*
*  @file       LvlPlan.h
*
*  @brief      Class to handle a lvl plan to highlight the edges
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

//==============================================================================
/**
*  @class  LvlPlan
*  @brief  LvlPlan is a class to handle a lvl plan mesh  to highlight the edges
*/
//==============================================================================
class LvlPlan: public Mesh
{
public:
    LvlPlan(float height, float length, float width);

    //--------------------------------------------------------------------------
    /// Change the height of the lvl plan
    /**
      * @param delta: add this to the height of the plan
      */
    //--------------------------------------------------------------------------
    void changeHeight(float delta);

//******************************************************************************
private:

    float m_height, //The height of the lvl plan. Can be seen as a threshold for the edge detection.
        m_length,
        m_width;
};

#endif // LVLPLAN_H
