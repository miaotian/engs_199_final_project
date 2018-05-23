
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
#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkImageFlip.h>

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
	vtkNew<vtkVolume> vol;
	vtkNew<vtkRenderer> ren1;
	vtkNew<vtkMarchingContourFilter> contour;
	vtkNew<vtkPolyDataMapper> mapper;
	vtkNew<vtkActor> actor;
	vtkNew<vtkCamera> cam1;
	vtkNew<vtkCoordinate> coord;
	vtkNew<vtkImageFlip> flipYFilter;
	vtkNew<vtkImageFlip> flipZFilter;
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
		but1 = new QPushButton("Load Data");
		but1->setFixedSize(100, 30);
		// Layout the widgets
		// YOUR CODE HERE
		layout_vertical = new QVBoxLayout();
		widget->setLayout(layout_vertical);
		layout_vertical->addWidget(viewport);
		layout_vertical->addWidget(but1);
		cam1->SetPosition(-1247.36, -1048.52, -654.272);
		cam1->SetFocalPoint(-12.5727, 11.141, 5.87349);
		cam1->SetViewUp(0.529516, -0.797376, 0.289491);
		cam1->SetViewAngle(11.7327);
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
		reader->SetDirectoryName("C:/Users/DoseOptics/Desktop/engs_199_final_project/data/d14068-39-anon");
		reader->Update();
		// Image viewer
		flipYFilter->SetFilteredAxis(1);
		flipYFilter->SetInputConnection(reader->GetOutputPort());
		flipYFilter->Update();

		flipZFilter->SetFilteredAxis(2);
		flipZFilter->SetInputConnection(flipYFilter->GetOutputPort());
		flipZFilter->Update();

		contour->SetInputConnection(flipZFilter->GetOutputPort());
		contour->SetValue(0, -400);
		contour->Update();
		mapper->SetInputConnection(contour->GetOutputPort());
		actor->SetMapper(mapper);
		ren1->AddActor(actor);
		ren1->SetActiveCamera(cam1);
		ren1->ResetCamera();
		viewport->GetRenderWindow()->AddRenderer(ren1);
		viewport->GetRenderWindow()->Render();
		coord->SetCoordinateSystemToViewport();
	}
};