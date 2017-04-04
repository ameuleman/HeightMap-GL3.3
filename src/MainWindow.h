#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

#include "ImageProcessor.h"
#include "RenderWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_originalImageButton_clicked();

	void on_smoothedImageButton_clicked();

	void on_gradientNormButton_clicked();

	void on_cannyImageButton_clicked();

	void on_choseImageButton_clicked();

    void on_useIndexButton_clicked();

private:

	void launchRenderWindow(QString const& windowName, image_matrix const& imageData);

	void updateImageProcessor();

	Ui::MainWindow *ui;

	ImageProcessor m_imageProcessor;

	std::string m_imageFile = "../data/data.png";

    bool m_useIndex = true;
};

#endif // MAINWINDOW_H
