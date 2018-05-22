
// Prevent this header file from being included multiple times
#pragma once

// VTK header files
#include <vtkPNGReader.h>
#include <vtkActor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageData.h>
#include <vtkDICOMImageReader.h>
#include <QVTKOpenGLWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkNew.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkMarchingContourFilter.h>
#include <vtkPolyDataMapper.h>

// ITK header files
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkScalarImageKmeansImageFilter.h>

// Qt header files
#include <QPointer>
#include <QFileDialog.h>
#include <QMainWindow.h>
#include <QLayout.h>
#include <QPushButton.h>
#include <QVTKWidget.h>
#include <QSlider>
#include <QLabel>

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
	// Declaration of  CLASS VARIABLES HERE
	QPushButton * but1;
	QSlider *sl1;
	QLabel *label1;
	QVBoxLayout *layout_vertical;
	QHBoxLayout *layout_horizontal;
	QPointer<QVTKOpenGLWidget> viewport;
	vtkNew<vtkDICOMImageReader> reader;
	vtkNew<vtkGenericOpenGLRenderWindow> window1;
	QWidget *widget;
	vtkNew<vtkSmartVolumeMapper> volumeMapper;
	vtkNew<vtkVolumeProperty> volumeProperty;
	vtkNew<vtkVolume> vol;
	vtkNew<vtkRenderer> ren1;
	vtkNew<vtkColorTransferFunction> color;
	vtkNew<vtkPiecewiseFunction> compositeOpacity;
	vtkNew<vtkMarchingContourFilter> contour;
	vtkNew<vtkPolyDataMapper> mapper;
	vtkNew<vtkActor> actor;
public:
	
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
		vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
		QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
		viewport = new QVTKOpenGLWidget();
		viewport->SetRenderWindow(window1.Get());
		volumeProperty->ShadeOff();
		volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
		/*compositeOpacity->AddPoint(0.0, 0.0);
		compositeOpacity->AddPoint(80.0, 1.0);
		compositeOpacity->AddPoint(80.1, 0.0);
		compositeOpacity->AddPoint(255.0, 0.0);
		volumeProperty->SetScalarOpacity(compositeOpacity);
		color->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
		color->AddRGBPoint(40.0, 1.0, 0.0, 0.0);
		color->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
		volumeProperty->SetColor(color);*/
		but1 = new QPushButton("Load Data");
		but1->setFixedSize(100, 30);
		// Layout the widgets
		// YOUR CODE HERE
		layout_vertical = new QVBoxLayout();
		widget->setLayout(layout_vertical);
		layout_vertical->addWidget(viewport);
		layout_vertical->addWidget(but1);
		// Connected widget signals to slots
		// YOUR CODE HERE
		connect(but1, SIGNAL(released()), this, SLOT(load_data()));
		// Display the window
		this->show();
	
	}

public slots: // This tells Qt we are defining slots here

	// YOUR CODE HERE
	void load_data()
	{
		// read all the dicom files in the specific directory

		reader->SetDirectoryName("C:/Users/Tianshun/Desktop/engs199_final/data/d14068");
		reader->Update();
		// Image viewer
		contour->SetInputConnection(reader->GetOutputPort());
		contour->SetValue(0, -400);
		contour->Update();
		//volumeMapper->SetInputConnection(contour->GetOutputPort());
		//volumeMapper->SetRequestedRenderModeToRayCast();
		/*vol->SetMapper(volumeMapper);
		vol->SetProperty(volumeProperty);*/
		mapper->SetInputConnection(contour->GetOutputPort());
		actor->SetMapper(mapper);
		ren1->AddActor(actor);
		ren1->ResetCamera();
		viewport->GetRenderWindow()->AddRenderer(ren1);
		viewport->GetRenderWindow()->Render();
		
		// set the slider minimum and maximum
	}
};