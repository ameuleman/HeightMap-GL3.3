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
*  @brief  Types to handle some usefull type definitions
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

	/**
	 * @brief int_line
	 */
	typedef std::vector<unsigned int> uint_line;
};

#endif // USEFULLTYPES_H
