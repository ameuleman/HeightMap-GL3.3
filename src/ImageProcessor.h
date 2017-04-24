#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

/**
*******************************************************************************
*
*  @file       ImageProcessor.h
*
*  @brief      Class to load an image and process it.
*               Uses Canny's algorithm to perform edge detection.
*
*  @author     Andréas Meuleman
*******************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <QVector2D>
#include <QImage>

#include "Types.h"

//==============================================================================
/**
*  @class  ImageProcessor
*  @brief  ImageProcessor is Class to load an image and process it.
*               Uses Canny's algorithm to perform edge detection.
*/
//==============================================================================
class ImageProcessor
{
public:
	/**
	 * @brief ImageProcessor Overloaded constructor with the name of the image file
	 * Load the file and perform the procesing
	 * @param fileName the name of the height map file
	 * @throws
	 */
	ImageProcessor(std::string const& fileName);

	/**
	 * @brief ImageProcessor default constructor, create an empty image processor
	 */
	ImageProcessor();

	/**
	 * @brief loadData Load the file and store it as vector<vector<float>>
	 * @param fileName the name of the height map file
	 * @throws
	 */
	void loadData(std::string const& fileName);

	/**
	 * @brief processImage Apply Canny algorithm and store the intermediate steps
	 * @throws
	 */
	void processImage();

	/**
	 * @brief getRawData get data corresponding to an image
	 * @return data before processing
	 */
	Types::float_matrix getRawData() const;

	/**
	 * @brief getSmoothedData get data corresponding to an image
	 * @return data after linear filtering
	 */
	Types::float_matrix getSmoothedData() const;

	/**
	 * @brief getGradientData get data corresponding to an image
	 * @return gradient norm for each pixel
	 */
	Types::float_matrix getGradientData() const;

	/**
	 * @brief getCannyData get data corresponding to an image
	 * @return data after Canny  algorithm
	 */
	Types::float_matrix getCannyData() const;

	/**
	 * @brief getM get the size of the image
	 * @return number of columns
	 */
	unsigned int getM() const;

	/**
	 * @brief getN get the size of the image
	 * @return number of rows
	 */
	unsigned int getN() const;

//******************************************************************************
private:
	/**
	 * @brief obtainLowerIndices retrieve one to each index of the input if possible
	 * @param i the first index
	 * @param j the second index
	 * @return the pair of indices minus 1 and clamp the result above 0
	 */
	std::pair<int, int> obtainLowerIndices(int i, int j);

	/**
	 * @brief obtainUpperIndices add one to each index of the input if possible
	 * @param i the first index
	 * @param j the second index
	 * @return the pair of indices plus 1 and clamp the result below the size of the data
	 */
	std::pair<int, int> obtainUpperIndices(int i, int j);

	/**
	 * @brief applyLinearFilter Apply a linear filter on the raw data
	 * and produce the smoother preprocessed data m_smoothedData
	 * Proceed between two values to enable parallel processing
	 * @param linearFilter the linear filter to apply
	 * @param leftIndex proceed from this index
	 * @param rightIndex to this index
	 */
	void applyLinearFilter(Types::float_matrix const& linearFilter,
						   unsigned int leftIndex, unsigned int rightIndex);

	/**
	 * @brief applyGradientNorm Calculate the gradient and its norm for each pixel
	 * and store it in m_gradientData. Also store gradients angles in m_gradientsAngles
	 * Proceed between two values to enable parallel processing
	 * @param leftIndex
	 * @param rightIndex
	 */
	void applyGradientNorm(unsigned int leftIndex, unsigned int rightIndex);

	/**
	 * @brief applyCannyAlgorithm Apply Canny algorithm and store it in m_cannyData
	 * Proceed between two values to enable parallel processing
	 * @param leftIndex proceed from this index
	 * @param rightIndex to this index
	 */
	void applyCannyAlgorithm(unsigned int leftIndex, unsigned int rightIndex);

	Types::float_matrix m_rawData, //Data before processing
		m_smoothedData, //Data after the first step of the processing: the linear filtering
		m_gradientData, //Data after gradient processing
		m_cannyData; //Data after edge detection using Canny algorithm

	//Save all the gradients angles to apply Canny Algorithm
	Types::float_matrix m_gradientsAngles;

	unsigned int m_m, //number of columns
		m_n; //number of rows
};

#endif // IMAGEPROCESSOR_H
