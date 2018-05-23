
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
#include <vtkPolyData.h>
#include <vtkPNGReader.h>
#include <vtkUnsignedShortArray.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
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

#include <fstream>
// Class that represents the main window for our application
class ui : public QMainWindow
{
	Q_OBJECT
private:
	// Declaration of  CLASS VARIABLES HERE
	QPushButton * but1;
	QPushButton * but2;
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
	vtkPolyData * surf;
	vtkNew<vtkPNGReader> reader2;
	vtkRenderWindow * rendw;
	
	int num_p;
public:
	
	// Constructor (happens when created)
	ui()
	{
		// Resize the window
		this->resize(1800, 800); // CHANGE THIS

		// Create the "central" (primary) widget for the window
		QWidget *widget = new QWidget();
		this->setCentralWidget(widget);

		// Create your widgets
		// Configuration of vtkopegl window
		vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
		QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
		viewport = new QVTKOpenGLWidget();
		viewport->SetRenderWindow(window1.Get());
		viewport->GetRenderWindow()->SetSize(1920, 1200);
		// configuration of button etc. 
		but1 = new QPushButton("Load CT");
		but1->setFixedSize(100, 30);
		but2 = new QPushButton("Load PNG");
		but2->setFixedSize(100, 30);
		// Layout the widgets
		// YOUR CODE HERE
		layout_vertical = new QVBoxLayout();
		layout_horizontal = new QHBoxLayout();
		widget->setLayout(layout_vertical);
		layout_vertical->addWidget(viewport);
		layout_vertical->addLayout(layout_horizontal);
		layout_horizontal->addWidget(but1);
		layout_horizontal->addWidget(but2);
		// Configuration of the camera
		cam1->SetPosition(-1247.36, -1048.52, -654.272);
		cam1->SetFocalPoint(-12.5727, 11.141, 5.87349);
		cam1->SetViewUp(0.529516, -0.797376, 0.289491);
		cam1->SetViewAngle(2);

		// Configure coordinate
		coord->SetCoordinateSystemToWorld();
		rendw = viewport->GetRenderWindow();
		// Connected widget signals to slots
		
		connect(but1, SIGNAL(released()), this, SLOT(load_data()));
		connect(but2, SIGNAL(released()), this, SLOT(load_png()));
		// Display the window
		this->show();
	
	}

public slots: // This tells Qt we are defining slots here

	// YOUR CODE HERE
	// the load data code loads the dicom ct files
	void load_data()
	{
		// read all the dicom files in the specific directory
		reader->SetDirectoryName("C:/Users/DoseOptics/Desktop/engs_199_final_project/data/d14068-39-anon");
		reader->Update();
		//Flip the dicome ct structures
		flipYFilter->SetFilteredAxis(1);
		flipYFilter->SetInputConnection(reader->GetOutputPort());
		flipYFilter->Update();

		flipZFilter->SetFilteredAxis(2);
		flipZFilter->SetInputConnection(flipYFilter->GetOutputPort());
		flipZFilter->Update();
		// setting the marching contour filter 
		contour->SetInputConnection(flipZFilter->GetOutputPort());
		contour->SetValue(0, -400);
		contour->Update();
		// get the polydata structure
		surf = contour->GetOutput();
		num_p = surf->GetNumberOfPoints();
		// feed into the polydata mapper
		mapper->SetInputConnection(contour->GetOutputPort());
		mapper->Update();
		// feed signal into the polydata maper
		actor->SetMapper(mapper);
		actor->GetProperty()->SetColor(0.5, 0.5, 0.5);
		// connecting to the renderer and render windows.
		ren1->SetActiveCamera(cam1);
		rendw->AddRenderer(ren1);
		ren1->AddActor(actor);
		rendw->Render();
	
	}

	void load_png()
	{
		// first read the image data
		// foot print of position data for debug
		ofstream myfile;
		myfile.open("test.txt");
		// reading rthe png file of cherenkov images 
		reader2->SetFileName("C:/Users/DoseOptics/Desktop/engs_199_final_project/data/185.PNG");
		reader2->Update();
		// save the image data into 1-D vector
		unsigned short * image_array = static_cast<unsigned short *> (reader2->GetOutput()->GetScalarPointer());
		vtkNew<vtkUnsignedShortArray> my_array;
		int width = 1920, height = 1200;
		double * pos;
		int * p;
		unsigned short val;
		for (int i = 0; i < num_p; i++)
		{
			// get the position of point from the contour in world coodinate
			pos = surf->GetPoint(i);
			coord->SetValue(pos);
			// read the position in the viewport
			p = coord->GetComputedViewportValue(ren1);
			if (p[0] >= 0 && p[0] <= width && p[1] >= 0 && p[1] <= height)
				val = image_array[p[0] + p[1] * width];
			else
				val = 0;
			// assign the pixel value into array. 
			my_array->InsertNextValue(val);
			myfile << p[0] << " " << p[1] << std::endl;
		}
		myfile.close();
		// overlay the 2d pixel intensity back into the 3D contour. 
		surf->GetPointData()->SetScalars(my_array);
		rendw->Render();
	}
};