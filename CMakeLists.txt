
# Specify the minimum version of CMake that can be used
cmake_minimum_required(VERSION 2.8)

# Name the project
PROJECT(Final_Project)

# Say where the libraries are located
set(VTK_DIR "D:/vtk/vtk_build/")
set(ITK_DIR "D:/itk/itk_build/")
set(CMAKE_PREFIX_PATH "D:/Qt/Qt5.10.1/5.10.1/msvc2017_64/")
set(OpenCV_DIR "D:/opencv/build/")

# Find and include VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Find and include ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# Find and include Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED QUIET)

# Find and include OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Collect and include all of our source files (.h and .cxx files)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)

# We are creating an executable project, and including the source files into it
add_executable(Final_Project ${CXX_FILES} ${QT_WRAP})

# Tell Qt which modules to use (there are others, like Test and Sql for example)
qt5_use_modules(Final_Project Core Gui Widgets winextras)

# Set the startup project in Visual Studio to be our project (rather than the extra stuff CMake builds)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Final_Project)

# Tell the project which libraries to link (Qt does this automatically)
target_link_libraries(Final_Project ${VTK_LIBRARIES})
target_link_libraries(Final_Project ${ITK_LIBRARIES})
target_link_libraries(Final_Project ${OpenCV_LIBS})
