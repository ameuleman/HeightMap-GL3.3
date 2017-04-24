#ifndef USEFULLTYPES_H
#define USEFULLTYPES_H

/**
*******************************************************************************
*
*  @file       Types.h
*
*  @brief      Class to handle some usefull type definitions
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QVector3D>
#include <vector>


//==============================================================================
/**
*  @class  Types
*  @brief  Types is Class to load an image and process it.
*               Uses Canny's algorithm to perform edge detection.
*/
//==============================================================================
class Types
{
public:
	/**
	 * @brief vector_data
	 */
	typedef std::vector<QVector3D> vertices_data;

	/**
	 * @brief float_line
	 */
	typedef std::vector<float> float_line;

	/**
	 * @brief image_matrix
	 */
	typedef std::vector<float_line> float_matrix;
};

#endif // USEFULLTYPES_H
