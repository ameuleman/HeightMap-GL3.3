#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
*******************************************************************************
*
*  @file	MainWindow.h
*
*  @brief	Class that describes a control panel for the application
*
*  @author	Andréas Meuleman
*******************************************************************************
*/

//******************************************************************************
//  Include
//******************************************************************************
#include <QMainWindow>
#include <string>

#include "imageProcessing/ImageProcessor.h"
#include "rendering/RenderWindow.h"

namespace Ui {
	class MainWindow;
}

//==============================================================================
/**
*  @class  MainWindow
*  @brief  MainWindow is Class that describes a control panel for the application
*/
//==============================================================================
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

//******************************************************************************
//  slots that define the action of buttons
//******************************************************************************
private slots:
	void on_originalImageButton_clicked();

	void on_smoothedImageButton_clicked();

	void on_gradientNormButton_clicked();

	void on_cannyImageButton_clicked();

	void on_choseImageButton_clicked();

	void on_useIndexButton_clicked();

//******************************************************************************
private:
	/**
	 * @brief launchRenderWindow launch a new render window to display a height map
	 * @param windowName the name of the window to be created
	 * @param imageData data corresponding to the height map to be displayed
	 */
	void launchRenderWindow(QString const& windowName, Types::float_matrix const& imageData);

	/**
	 * @brief updateImageProcessor Update the image processor
	 * by changing the original image and process it
	 */
	void updateImageProcessor();

	Ui::MainWindow *ui;

	//image processor that store the height maps data and process them
	ImageProcessor m_imageProcessor;

	/**
	 * @brief m_imageFile name of the image file.
	 * @default "../data/data.png"
	 */
	std::string m_imageFile = (":/data/data.png");

	//To know if the index needs to be set
	bool m_useIndex = true;
};

#endif // MAINWINDOW_H
