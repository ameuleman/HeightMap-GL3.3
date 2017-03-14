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
*  @date       01/03/2017
*
*  @author     Andréas Meuleman
*******************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#include <thread>
#include <QImage>


//******************************************************************************
//  namespace
//******************************************************************************
using namespace std;

//==============================================================================
/**
*  @class  ImageProcessor
*  @brief  ImageProcessor is Class to load an image and process it
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
    *  @param fileName: the name of the height map file
    */
    //--------------------------------------------------------------------------
    ImageProcessor(string const& fileName);

    template<class F> static void performInParallel(
            F const& functor, unsigned int leftIndex, unsigned int rightIndex,
            unsigned char parallilismLvl = 1,
            unsigned char maxParallelism = thread::hardware_concurrency());

    // getters
    vector<vector<float>> getRawData() const;
    vector<vector<float>> getPreprocessedData() const;
    vector<vector<float>> getProcessedData() const;

    unsigned int getM() const;
    unsigned int getN() const;

//******************************************************************************
private:
    //No default constructor
    ImageProcessor();

    //--------------------------------------------------------------------------
    /// Load the file and store it as vector<vector<float>>
    /**
    *  @param fileName: the name of the height map file
    */
    //--------------------------------------------------------------------------
    void loadData(string const& fileName);

    //--------------------------------------------------------------------------
    /// Apply a linear filter on the raw data
    /// and produce the smoother preprocessed data
    /**
    *  @param linearFilter: the linear filter to apply
    */
    //--------------------------------------------------------------------------
    void applyLinearFilter(vector<vector<float>> const& linearFilter,
            int leftIndex, int rightIndex);

    //--------------------------------------------------------------------------
    /// Calculate the norm of the gradient of each pixel to extract the outline
    //--------------------------------------------------------------------------
    void processImage();

    vector<vector<float>> m_rawData, //Data before processing
        m_preprocessedData, //Data after the first step of the processing
        m_processedData; //Data after processing

    unsigned int m_m, //number of columns
        m_n; //number of rows
};

#endif // IMAGEPROCESSOR_H
