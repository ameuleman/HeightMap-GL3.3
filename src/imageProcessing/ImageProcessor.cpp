/**
*******************************************************************************
*
*  @file	ImageProcessor.cpp
*
*  @brief	Class to load an image and process it
*
*  @author	Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  constant variables
//******************************************************************************
//threshold for Canny algorithm
const float THRESHOLD_1 = 0.029f;
const float THRESHOLD_2 = 0.065f;

//******************************************************************************
//  Include
//******************************************************************************
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <iostream>
#include "ImageProcessor.h"
#include "tools/ParallelTool.h"

//------------------------------------------------------------------------------
ImageProcessor::ImageProcessor(std::string const& fileName)
//------------------------------------------------------------------------------
{
	loadData(fileName);
	processImage();
}

//------------------------------------------------------------------------------
ImageProcessor::ImageProcessor()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void ImageProcessor::setRawData(Types::float_matrix const & imageData, unsigned int n, unsigned int m)
//------------------------------------------------------------------------------
{
	m_rawData = imageData;
	m_n = n;
	m_m = m;

	processImage();
}

//------------------------------------------------------------------------------
Types::float_matrix ImageProcessor::getRawData() const
//------------------------------------------------------------------------------
{
	return m_rawData;
}

//------------------------------------------------------------------------------
Types::float_matrix ImageProcessor::getSmoothedData() const
//------------------------------------------------------------------------------
{
	return m_smoothedData;
}

//------------------------------------------------------------------------------
Types::float_matrix ImageProcessor::getGradientData() const
//------------------------------------------------------------------------------
{
	return m_gradientData;
}

//------------------------------------------------------------------------------
Types::float_matrix ImageProcessor::getCannyData() const
//------------------------------------------------------------------------------
{
	return m_cannyData;
}

//------------------------------------------------------------------------------
unsigned int ImageProcessor::getM() const
//------------------------------------------------------------------------------
{
	return m_m;
}

//------------------------------------------------------------------------------
unsigned int ImageProcessor::getN() const
//------------------------------------------------------------------------------
{
	return m_n;
}

//------------------------------------------------------------------------------
void ImageProcessor::loadData(std::string const& fileName)
//------------------------------------------------------------------------------
{
	//Load the image
	QImage image(fileName.c_str());

	m_n = image.height();
	m_m = image.width();

	//Make sure their is data to load
	if(m_n && m_m)
	{
		if(image.format() == QImage::Format_Grayscale8)
		{
			//Allocate memory
			m_rawData.resize(m_n, Types::float_line(m_m));

			unsigned char * pLine;

			for(unsigned int i(0); i < m_n; i++)
			{
				//retrieve a line of data
				pLine = image.scanLine(i);

				//read all the data of the line and store them as floats in the [0,1] range
				for(unsigned int j(0); j < m_m; j++)
				{
					m_rawData[i][j] = float(pLine[j])/255.f;
				}
			}
		}
		else
			throw std::runtime_error("Wrong file : unsusual format, requires grayscale");
	}
	else
		throw std::runtime_error("Wrong file name : cannot process " + fileName);
}

//------------------------------------------------------------------------------
std::pair<int, int> ImageProcessor::obtainLowerIndices(int i, int j)
//------------------------------------------------------------------------------
{
	if(i > 0)
		i -= 1;

	if(j > 0)
		j -= 1;

	return std::pair<int, int>(i, j);
}

//------------------------------------------------------------------------------
std::pair<int, int> ImageProcessor::obtainUpperIndices(int i, int j)
//------------------------------------------------------------------------------
{
	if(i < int(m_n) - 1)
		i += 1;

	if(j < int(m_m) - 1)
		j += 1;

	return std::pair<int, int>(i, j);
}

//------------------------------------------------------------------------------
void ImageProcessor::applyLinearFilter(const Types::float_matrix &linearFilter,
								unsigned int leftIndex, unsigned int rightIndex)
//------------------------------------------------------------------------------
{
	//Make sure the filter's dimentions are odd numbers
	if(linearFilter.size() % 2 && linearFilter[0].size() % 2)
	{
		//To know how many pixel need to be read before and after the current pixel
		int filterIRadius(int(linearFilter.size()/2));
		int filterJRadius(int(linearFilter[0].size()/2));

		int n(m_n);
		int m(m_m);

		for(int i((int)(leftIndex)); i < (int)(rightIndex); i++)
		{
			for(int j(0); j < m; j++)
			{
				//Result of the filter for this pixel
				float pixelSum(0);

				for(int iFilter(-filterIRadius); iFilter < filterIRadius + 1; iFilter++)
				{
					//Index of the pixel where the data will be read
					int iReadIndex;

					//Set iReadIndex and clamp it, preventing it from being out of range
					if(i - iFilter > 0)
						if(i - iFilter < n)
							iReadIndex = i - iFilter;
						else
							iReadIndex = n - 1;
					else
						iReadIndex = 0;


					for(int jFilter(-filterJRadius); jFilter < filterJRadius + 1; jFilter++)
					{
						int jReadIndex;

						if(j - jFilter > 0)
							if(j - jFilter < m)
								jReadIndex = j - jFilter;
							else
								jReadIndex = m - 1;
						else
							jReadIndex = 0;

						//Add the value of the read index multiplied by the corresponding
						//value in the filter to the result for the current pixel
						pixelSum += m_rawData[iReadIndex][jReadIndex] *
							linearFilter[iFilter + filterIRadius][jFilter + filterJRadius];
					}
				}

				//Set the data
				m_smoothedData[i][j] = pixelSum;
			}
		}
	}
	else
		throw std::length_error("Wrong filter's dimensions");
}

//------------------------------------------------------------------------------
void ImageProcessor::applyGradientNorm(unsigned int leftIndex, unsigned int rightIndex)
//------------------------------------------------------------------------------
{
	QVector2D gradient;

	for(unsigned int i(leftIndex); i < rightIndex; i++)
	{
		for(unsigned int j(0); j < m_m; j++)
		{
			std::pair<int, int> lowerIndices(obtainLowerIndices(i, j));
			std::pair<int, int> upperIndices(obtainUpperIndices(i, j));

			//gradient for the x axis
			gradient.setX(m_smoothedData[upperIndices.first][j] -
					m_smoothedData[lowerIndices.first][j]);

			//gradient for the y axis
			gradient.setY(m_smoothedData[i][upperIndices.second] -
					m_smoothedData[i][lowerIndices.second]);

			//Store angles to apply Canny algorithm Later
			m_gradientsAngles[i][j] = atan((gradient.x() /
											gradient.y()) * 4 / M_PI);

			//Store gradient norm
			m_gradientData[i][j] = gradient.length();
		}
	}
}

//------------------------------------------------------------------------------
void ImageProcessor::applyCannyAlgorithm(unsigned int leftIndex, unsigned int rightIndex)
//------------------------------------------------------------------------------
{
	for(unsigned int i(leftIndex); i < rightIndex; i++)
	{
		for(unsigned int j(0); j < m_m; j++)
		{
			//In case the value of the gradient is below the first threshold,
			//we ignore the corresponding pixel
			if(m_gradientData[i][j] < THRESHOLD_1)
			{
				m_cannyData[i][j] = 0;
			}
			else
			{
				//Calculate the value of gradient norm for the adjacent pixels in the gradient directions
				float theta = m_gradientsAngles[i][j];

				std::pair<int, int> lowerIndices(obtainLowerIndices(i, j));
				std::pair<int, int> upperIndices(obtainUpperIndices(i, j));

				//Values of gradient norm in the two directions
				float maxChecker1, maxChecker2;

				if(theta < - 1)
				{
					//Interpolate the value for both directions
					maxChecker1 = m_gradientData[lowerIndices.first][j] * ( -1 - theta) +
						m_gradientData[lowerIndices.first][upperIndices.second] * (2 + theta);

					maxChecker2 = m_gradientData[upperIndices.first][j] * ( -1 - theta) +
						m_gradientData[upperIndices.first][lowerIndices.second] * (2 + theta);
				}
				else if(theta < 0)
				{
					maxChecker1 = m_gradientData[lowerIndices.first][upperIndices.second] * (- theta) +
						m_gradientData[i][upperIndices.second] * (1 + theta);

					maxChecker2 = m_gradientData[upperIndices.first][lowerIndices.second] * (- theta) +
						m_gradientData[i][lowerIndices.second] * (1 + theta);
				}
				else if(theta < 1)
				{
					maxChecker1 = m_gradientData[i][upperIndices.second] * (1 - theta) +
							m_gradientData[upperIndices.first][upperIndices.second] * (theta);

					maxChecker2 = m_gradientData[i][lowerIndices.second] * (1 - theta) +
							m_gradientData[lowerIndices.first][lowerIndices.second] * (theta);
				}

				else
				{
					maxChecker1 = m_gradientData[upperIndices.first][j] * (theta - 1) +
						m_gradientData[upperIndices.first][upperIndices.second] * (2 - theta);

					maxChecker2 = m_gradientData[lowerIndices.first][j] * (theta - 1) +
						m_gradientData[lowerIndices.first][lowerIndices.second] * (2 - theta);
				}

				//If the value of the pixel is not bigger than the value of adjacent pixels
				//in the gradient directions, we ignore it to make edges thinner
				if(m_gradientData[i][j] < std::max(maxChecker1, maxChecker2))
				{
					m_cannyData[i][j] = 0;
				}
				//if the value is bigger than the second threshold, we keep it
				else if(m_gradientData[i][j] > THRESHOLD_2)
				{
					m_cannyData[i][j] = 1;
				}
				//If the value is between the two thresholds, we apply the last part of Canny
				//algorithm: hysteresis
				else if(m_gradientData[i][j] > THRESHOLD_1)
				{
					//Values of gradient norm in the two directions of the gradient's normal vector
					float hysteresisChecker1, hysteresisChecker2;

					if(theta < - 1)
					{
						hysteresisChecker1 = m_gradientData[i][upperIndices.second] * (-1 - theta) +
							m_gradientData[upperIndices.first][upperIndices.second] * (2 + theta);

						hysteresisChecker2 = m_gradientData[i][lowerIndices.second] * (-1 - theta) +
							m_gradientData[lowerIndices.first][lowerIndices.second] * (2 + theta);
					}
					else if(theta < 0)
					{
						hysteresisChecker1 = m_gradientData[upperIndices.first][upperIndices.second] *
								(- theta) +
							m_gradientData[upperIndices.first][j] * (1 + theta);

						hysteresisChecker2 = m_gradientData[lowerIndices.first][upperIndices.second] *
								(- theta) +
							m_gradientData[lowerIndices.first][j] * (1 + theta);
					}
					else if(theta < 1)
					{
						hysteresisChecker1 = m_gradientData[upperIndices.first][j] * (1 - theta) +
							m_gradientData[upperIndices.first][lowerIndices.second] * (theta);

						hysteresisChecker2 = m_gradientData[lowerIndices.first][j] * (1 - theta) +
							m_gradientData[lowerIndices.first][lowerIndices.second] * (theta);
					}
					else
					{
						hysteresisChecker1 = m_gradientData[upperIndices.first][lowerIndices.second] *
								(2 - theta) +
							m_gradientData[i][lowerIndices.second] * (theta - 1);

						hysteresisChecker2 = m_gradientData[lowerIndices.first][upperIndices.second] *
								(2 - theta) +
							m_gradientData[i][upperIndices.second] * (theta - 1);
					}

					//If the value of adjacent pixels in gradient's normal vector directions,
					//we keep it
					if(std::max(hysteresisChecker1, hysteresisChecker2) > THRESHOLD_1)
					{
						m_cannyData[i][j] = 1;
					}

				}
			}
		}
	}
}

//------------------------------------------------------------------------------
void ImageProcessor::processImage()
//------------------------------------------------------------------------------
{
	if(m_n != 0 && m_m != 0 && m_rawData.size() == m_n && m_rawData[0].size() == m_m)
	{
		//Alocate memory
		m_smoothedData.resize(m_n, Types::float_line(m_m));
		m_gradientsAngles.resize(m_n, Types::float_line(m_m));
		m_gradientData.resize(m_n, Types::float_line(m_m));
		m_cannyData.resize(m_n, Types::float_line(m_m));

		//Create the linear filter
		Types::float_matrix linearFilter({Types::float_line({2, 4, 5, 4, 2}),
											Types::float_line({4, 9, 12, 9, 4}),
											Types::float_line({5, 12, 15, 12, 5}),
											Types::float_line({4, 9, 12, 9, 4}),
											Types::float_line({2, 4, 5, 4, 2})});

		for_each(linearFilter.begin(), linearFilter.end(),
				 [](Types::float_line &l){for_each(l.begin(), l.end(),
				 [](float &n){n /= 159.f;});});

		//Perform image processing in parallel to reduce computation time
		ParallelTool::performInParallel(
			[this, linearFilter](unsigned int leftIndex, unsigned int rightIndex)
			{
				try
				{
					applyLinearFilter(linearFilter, leftIndex, rightIndex);
				}
				catch(std::exception const& e)
				{
					std::cerr << "ERROR : " << e.what() << std::endl;
				}
			},
			0, m_n);

		ParallelTool::performInParallel(
			[this](unsigned int leftIndex, unsigned int rightIndex)
			{
				applyGradientNorm(leftIndex, rightIndex);
			},
			0, m_n);

		ParallelTool::performInParallel(
			[this](unsigned int leftIndex, unsigned int rightIndex)
			{
				applyCannyAlgorithm(leftIndex, rightIndex);
			},
			0, m_n);
	}
	else
	{
		throw std::runtime_error("Wrong data sizes, cannot process image");
	}
}

