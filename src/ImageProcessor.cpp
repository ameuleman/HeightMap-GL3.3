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
//  Include
//******************************************************************************
#include <QVector2D>

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
vector<vector<float>> ImageProcessor::getPreprocessedData() const
//------------------------------------------------------------------------------
{
    return m_preprocessedData;
}

//------------------------------------------------------------------------------
vector<vector<float>> ImageProcessor::getProcessedData() const
//------------------------------------------------------------------------------
{
    return m_processedData;
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
template<class F> static void ImageProcessor::performInParallel(
        F const& functor, unsigned int leftIndex, unsigned int rightIndex,
        unsigned char maxParallelism, unsigned char parallelismLvl)
//------------------------------------------------------------------------------
{
    if(parallelismLvl <= maxParallelism)
    {
        parallelismLvl *= 2;
        unsigned int midIndex(unsigned int(rightIndex / 2));

        thread parallelProcessing(
            [&functor, midIndex, rightIndex, maxParallelism, parallelismLvl]()
            {
                performInParallel(functor,
                        midIndex + 1, rightIndex,
                        maxParallelism, parallelismLvl);
            });

        performInParallel(functor,
                    leftIndex, midIndex,
                    maxParallelism, parallelismLvl);

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
    m_preprocessedData.resize(m_n, vector<float>(m_m));
    m_processedData.resize(m_n, vector<float>(m_m));

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
void ImageProcessor::applyLinearFilter(vector<vector<float>> const& linearFilter,
        int leftIndex, int rightIndex)
//------------------------------------------------------------------------------
{
    int filterIRadius(int(linearFilter.size()/2));
    int filterJRadius(int(linearFilter[0].size()/2));

    int n(m_n);
    int m(m_m);

    for(int i(leftIndex); i < rightIndex; i++)
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

            m_preprocessedData[i][j] = pixelSum;
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

    performInParallel(
        [this, &linearFilter](unsigned int leftIndex, unsigned int rightIndex)
        {
            applyLinearFilter(linearFilter, leftIndex, rightIndex);
        },
        0, m_n);

    /*m_processedData.resize(m_n, vector<float>(m_m));
    QVector2D gradient;

    for(unsigned int i(0); i < m_n; i++)
    {
        for(unsigned int j(0); j < m_m; j++)
        {
            //gradient for the x axis
            if(i > 0)
                if(i < m_n - 1)
                    gradient.setX(m_preprocessedData[i + 1][j] - m_preprocessedData[i - 1][j]);
                else
                    gradient.setX(m_preprocessedData[i][j] - m_preprocessedData[i - 1][j]);
            else
                gradient.setX(m_preprocessedData[i + 1][j] - m_preprocessedData[i][j]);

            //gradient for the y axis
            if(j > 0)
                if(j < m_m - 1)
                    gradient.setY(m_preprocessedData[i][j + 1] - m_preprocessedData[i][j - 1]);
                else
                    gradient.setY(m_preprocessedData[i][j] - m_preprocessedData[i][j - 1]);
            else
                gradient.setY(m_preprocessedData[i][j + 1] - m_preprocessedData[i][j]);

            m_processedData[i][j] = gradient.length();
        }
    }*/
}
