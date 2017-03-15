/**
*******************************************************************************
*
*  @file       ImageProcessor.cpp
*
*  @brief      Class to load an image and process it
*
*  @date       01/03/2017
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  constant variables
//******************************************************************************
//threshold for Canny algorithm
const float THRESHOLD_1 = 0.02f;
const float THRESHOLD_2 = 0.055f;

//******************************************************************************
//  Include
//******************************************************************************
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <iostream>
#include "ImageProcessor.h"

//------------------------------------------------------------------------------
ImageProcessor::ImageProcessor(string const& fileName)
//------------------------------------------------------------------------------
{
    loadData(fileName);

    processImage();
}

//------------------------------------------------------------------------------
vector<vector<float>> ImageProcessor::getRawData() const
//------------------------------------------------------------------------------
{
    return m_rawData;
}

//------------------------------------------------------------------------------
vector<vector<float>> ImageProcessor::getSmoothedData() const
//------------------------------------------------------------------------------
{
    return m_smoothedData;
}

//------------------------------------------------------------------------------
vector<vector<float>> ImageProcessor::getGradientData() const
//------------------------------------------------------------------------------
{
    return m_gradientData;
}

//------------------------------------------------------------------------------
vector<vector<float>> ImageProcessor::getCannyData() const
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
template<class F> void ImageProcessor::performInParallel(
        F const& functor, unsigned int leftIndex, unsigned int rightIndex,
        unsigned char parallelismLvl, unsigned char maxParallelism)
//------------------------------------------------------------------------------
{
    parallelismLvl *= 2;

    if(parallelismLvl <= maxParallelism)
    {
        unsigned int midIndex((unsigned int)(rightIndex / 2));

        thread parallelProcessing(
            [&functor, midIndex, rightIndex, parallelismLvl, maxParallelism]()
            {
                performInParallel(functor,
                        midIndex, rightIndex,
                        parallelismLvl, maxParallelism);
            });

        performInParallel(functor,
                    leftIndex, midIndex,
                    parallelismLvl, maxParallelism);

        parallelProcessing.join();
    }
    else
    {
        functor(leftIndex, rightIndex);
    }
}


//------------------------------------------------------------------------------
void ImageProcessor::loadData(string const& fileName)
//------------------------------------------------------------------------------
{
    //Load the image
    QImage image(fileName.c_str());

    unsigned char * pLine;

    m_n = image.height();
    m_m = image.width();

    m_rawData.resize(m_n, vector<float>(m_m));
    m_smoothedData.resize(m_n, vector<float>(m_m));
    m_gradients.resize(m_n, vector<QVector2D>(m_m));
    m_gradientData.resize(m_n, vector<float>(m_m));
    m_cannyData.resize(m_n, vector<float>(m_m));

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

//------------------------------------------------------------------------------
pair<int, int> ImageProcessor::obtainLowerIndices(int i, int j)
//------------------------------------------------------------------------------
{
    if(i > 0)
        i -= 1;

    if(j > 0)
        j -= 1;

    return pair<int, int>(i, j);
}

//------------------------------------------------------------------------------
pair<int, int> ImageProcessor::obtainUpperIndices(int i, int j)
//------------------------------------------------------------------------------
{
    if(i < int(m_n) - 1)
        i += 1;

    if(j < int(m_m) - 1)
        j += 1;

    return pair<int, int>(i, j);
}

//------------------------------------------------------------------------------
void ImageProcessor::applyLinearFilter(vector<vector<float>> const& linearFilter)
//------------------------------------------------------------------------------
{
    int filterIRadius(int(linearFilter.size()/2));
    int filterJRadius(int(linearFilter[0].size()/2));

    int n(m_n);
    int m(m_m);

    for(int i(0); i < n; i++)
    {
        for(int j(0); j < m; j++)
        {
            float pixelSum(0);

            for(int iFilter(-filterIRadius); iFilter < filterIRadius + 1; iFilter++)
            {
                int iReadIndex;


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

                    pixelSum += m_rawData[iReadIndex][jReadIndex] * linearFilter[iFilter + filterIRadius][jFilter + filterJRadius];
                }
            }

            m_smoothedData[i][j] = pixelSum;
        }
    }
}

//------------------------------------------------------------------------------
void ImageProcessor::applyGradientNorm(unsigned int leftIndex, unsigned int rightIndex)
//------------------------------------------------------------------------------
{
    for(unsigned int i(leftIndex); i < rightIndex; i++)
    {
        for(unsigned int j(0); j < m_m; j++)
        {
            pair<int, int> lowerIndices(obtainLowerIndices(i, j));
            pair<int, int> upperIndices(obtainUpperIndices(i, j));

            //gradient for the x axis
            m_gradients[i][j].setX(m_smoothedData[upperIndices.first][j] -
                    m_smoothedData[lowerIndices.first][j]);

            //gradient for the y axis
            m_gradients[i][j].setY(m_smoothedData[i][upperIndices.second] -
                    m_smoothedData[i][lowerIndices.second]);

            m_gradientData[i][j] = m_gradients[i][j].length();
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
            if(m_gradientData[i][j] < THRESHOLD_1)
            {
                m_cannyData[i][j] = 0;
            }
            else
            {
                float theta = atan((m_gradients[i][j].x() / m_gradients[i][j].y()) * 4 / M_PI);

                pair<int, int> lowerIndices(obtainLowerIndices(i, j));
                pair<int, int> upperIndices(obtainUpperIndices(i, j));

                float maxChecker1, maxChecker2;

                if(theta < - 1)
                {

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


                if(m_gradientData[i][j] < max(maxChecker1, maxChecker2))
                {
                    m_cannyData[i][j] = 0;
                }
                else if(m_gradientData[i][j] > THRESHOLD_2)
                {
                    m_cannyData[i][j] = 1;
                }
                else if(m_gradientData[i][j] > THRESHOLD_1)
                {
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

                    if(max(hysteresisChecker1, hysteresisChecker2) > THRESHOLD_1)
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
    //Create the linear filter
    vector<vector<float>> linearFilter({vector<float>({2, 4, 5, 4, 2}),
                                        vector<float>({4, 9, 12, 9, 4}),
                                        vector<float>({5, 12, 15, 12, 5}),
                                        vector<float>({4, 9, 12, 9, 4}),
                                        vector<float>({2, 4, 5, 4, 2})});

    for_each(linearFilter.begin(), linearFilter.end(),
             [](vector<float> &l){for_each(l.begin(), l.end(),
             [](float &n){n /= 159.f;});});

    //Don't apply linear filter in parallel
    //since the computation is bound by the memory bandwidth
    applyLinearFilter(linearFilter);

    performInParallel(
        [this](unsigned int leftIndex, unsigned int rightIndex)
        {
            applyGradientNorm(leftIndex, rightIndex);
        },
        0, m_n);

    performInParallel(
        [this](unsigned int leftIndex, unsigned int rightIndex)
        {
            applyCannyAlgorithm(leftIndex, rightIndex);
        },
        0, m_n);
}
