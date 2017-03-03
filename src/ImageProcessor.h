#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

/**
*******************************************************************************
*
*  @file       ImageProcessor.h
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
#include "RenderWindow.h"


//==============================================================================
/**
*  @class  ImageProcessor
*  @brief  ImageProcessor is Class to load an image and process it
*/
//==============================================================================
class ImageProcessor
{
public:
    //--------------------------------------------------------------------------
    /// Overloaded constructor with the name of the image file
    /// Load the file and perform the procesing
    /**
    *  @param fileName: the name of the height map file
    */
    //--------------------------------------------------------------------------
    ImageProcessor(string const& fileName);

    // getters
    vector<vector<float>> getRawData() const;
    vector<vector<float>> getProcessedData() const;

    unsigned int getM() const;
    unsigned int getN() const;

//******************************************************************************
private:
    //No default constructor
    ImageProcessor();

    //--------------------------------------------------------------------------
    /// Load the file and store it as vector<vector<float>>
    //--------------------------------------------------------------------------
    void loadData(string const& fileName);

    //--------------------------------------------------------------------------
    /// Calculate the norm of the gradient of each pixel to extract the outline
    //--------------------------------------------------------------------------
    void processImage();

    vector<vector<float>> m_rawData,
        m_processedData;

    unsigned int m_m,
        m_n;
};

#endif // IMAGEPROCESSOR_H
