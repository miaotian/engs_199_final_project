
// Prevent this header file from being included multiple times
#pragma once

// VTK header files
#include <vtkPNGReader.h>
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkActor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageData.h>

// ITK header files
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkScalarImageKmeansImageFilter.h>

// Qt header files
#include <QFileDialog.h>
#include <QMainWindow.h>
#include <QLayout.h>
#include <QPushButton.h>
#include <QVTKWidget.h>

// OpenCV header files
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Class that represents the main window for our application
class ui : public QMainWindow
{
	Q_OBJECT
private:
	
public:
	// CLASS VARIABLES HERE

	// Constructor (happens when created)
	ui()
	{
		// Resize the window
		this->resize(800, 700); // CHANGE THIS

		// Create the "central" (primary) widget for the window
		QWidget *widget = new QWidget();
		this->setCentralWidget(widget);

		// Create your widgets
		// YOUR CODE HERE
		
		// Layout the widgets
		// YOUR CODE HERE

		// Connected widget signals to slots
		// YOUR CODE HERE

		// Display the window
		this->show();
	}

public slots: // This tells Qt we are defining slots here

	// YOUR CODE HERE

};