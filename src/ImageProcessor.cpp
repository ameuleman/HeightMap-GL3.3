/**
*******************************************************************************
*
*  @file       ImageProcessor.cpp
*
*  @brief      Class to load an image and process it
*
*  @version    1.0
*
*  @date       01/03/2017
*
*  @author     Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
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
void ImageProcessor::loadData(string const& fileName)
//------------------------------------------------------------------------------
{
    //Load the image
    QImage image(fileName.c_str());

    byte * pLine;

    m_n = image.height();
    m_m = image.width();

    m_rawData.resize(m_n, vector<float>(m_m));

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
void ImageProcessor::processImage()
//------------------------------------------------------------------------------
{
    m_processedData.resize(m_n, vector<float>(m_m));

}


