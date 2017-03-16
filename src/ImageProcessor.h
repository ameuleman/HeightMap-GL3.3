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
#include <QVector2D>
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

    /*template<class F> static void performInParallel(
            F const& functor, unsigned int leftIndex, unsigned int rightIndex,
            unsigned char maxParallelism = thread::hardware_concurrency(),
            unsigned char parallilismLvl = 1);*/

    // getters
    vector<vector<float>> getRawData() const;
    vector<vector<float>> getSmoothedData() const;
    vector<vector<float>> getGradientData() const;
    vector<vector<float>> getCannyData() const;

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

    pair<int, int> obtainLowerIndices(int i, int j);

    pair<int, int> obtainUpperIndices(int i, int j);

    //--------------------------------------------------------------------------
    /// Apply a linear filter on the raw data
    /// and produce the smoother preprocessed data
    /**
    *  @param linearFilter: the linear filter to apply
    */
    //--------------------------------------------------------------------------
    void applyLinearFilter(vector<vector<float>> const& linearFilter,
                           unsigned int leftIndex, unsigned int rightIndex);

    void applyGradientNorm(unsigned int leftIndex, unsigned int rightIndex);

    void applyCannyAlgorithm(unsigned int leftIndex, unsigned int rightIndex);

    //--------------------------------------------------------------------------
    /// Calculate the norm of the gradient of each pixel to extract the outline
    //--------------------------------------------------------------------------
    void processImage();

    vector<vector<float>> m_rawData, //Data before processing
        m_smoothedData, //Data after the first step of the processing
        m_gradientData, //Data after gradient processing
        m_cannyData; //Data after edge detection using Canny algorithm

    vector<vector<float>> m_gradientsAngles; //Save all the gradients to apply Canny Algorithm

    unsigned int m_m, //number of columns
        m_n; //number of rows
};

#endif // IMAGEPROCESSOR_H
