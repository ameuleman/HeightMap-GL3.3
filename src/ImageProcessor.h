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


//******************************************************************************
//  Type definition
//******************************************************************************
typedef std::vector<float> float_line;
typedef std::vector<float_line> image_matrix;

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
	//--------------------------------------------------------------------------
	/// Overloaded constructor with the name of the image file
	/// Load the file and perform the procesing
	/**
	* @param fileName: the name of the height map file
	* @throws
	*/
	//--------------------------------------------------------------------------
	ImageProcessor(std::string const& fileName);

	ImageProcessor();

	//--------------------------------------------------------------------------
	/// Load the file and store it as vector<vector<float>>
	/**
	* @param fileName: the name of the height map file
	* @throws
	*/
	//--------------------------------------------------------------------------
	void loadData(std::string const& fileName);

	//--------------------------------------------------------------------------
	/// Calculate the norm of the gradient of each pixel to extract the outline
	/**
	* @throws
	*/
	//--------------------------------------------------------------------------
	void processImage();

	// getters
	image_matrix getRawData() const;
	image_matrix getSmoothedData() const;
	image_matrix getGradientData() const;
	image_matrix getCannyData() const;

	unsigned int getM() const;
	unsigned int getN() const;

//******************************************************************************
private:
	//--------------------------------------------------------------------------
	/// retrieve one to each index of the input if possible
	/**
	* @param i: the first index
	* @param j: the second index
	* @return the pair of indices minus 1 and clamp the result above 0
	*/
	//--------------------------------------------------------------------------
	std::pair<int, int> obtainLowerIndices(int i, int j);

	//--------------------------------------------------------------------------
	/// add one to each index of the input if possible
	/**
	* @param i: the first index
	* @param j: the second index
	* @return the pair of indices plus 1 and clamp the result below the size of the data
	*/
	//--------------------------------------------------------------------------
	std::pair<int, int> obtainUpperIndices(int i, int j);

	//--------------------------------------------------------------------------
	/// Apply a linear filter on the raw data
	/// and produce the smoother preprocessed data
	/// Proceed between two values to enable parallel processing
	/**
	* @param linearFilter: the linear filter to apply
	* @param leftIndex: proceed from this index
	* @param rightIndex: to this index
	* @throws
	*/
	//--------------------------------------------------------------------------
	void applyLinearFilter(image_matrix const& linearFilter,
						   unsigned int leftIndex, unsigned int rightIndex);

	//--------------------------------------------------------------------------
	/// Calculate the gradient and its norm for each pixel
	/// Proceed between two values to enable parallel processing
	/**
	* @param leftIndex: proceed from this index
	* @param rightIndex: to this index
	*/
	//--------------------------------------------------------------------------
	void applyGradientNorm(unsigned int leftIndex, unsigned int rightIndex);

	//--------------------------------------------------------------------------
	/// Applay Canny algorithm
	/// Proceed between two values to enable parallel processing
	/**
	* @param leftIndex: proceed from this index
	* @param rightIndex: to this index
	*/
	//--------------------------------------------------------------------------
	void applyCannyAlgorithm(unsigned int leftIndex, unsigned int rightIndex);

	image_matrix m_rawData, //Data before processing
		m_smoothedData, //Data after the first step of the processing: the linear filtering
		m_gradientData, //Data after gradient processing
		m_cannyData; //Data after edge detection using Canny algorithm

	image_matrix m_gradientsAngles; //Save all the gradients to apply Canny Algorithm

	unsigned int m_m, //number of columns
		m_n; //number of rows
};

#endif // IMAGEPROCESSOR_H
