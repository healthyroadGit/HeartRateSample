cmake_minimum_required(VERSION 2.8.11)

project(HeartRateSample)

set(CMAKE_BUILD_TYPE Release)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
QT5_WRAP_UI(UIS_HDRS
    src/heartratesample.ui
    src/loadingscreen.ui
)
QT5_ADD_RESOURCES(resources src/heartratesample.qrc)

message("Binary directory ${HeartRateSample_BINARY_DIR}")

set( EXECUTABLE_OUTPUT_PATH 
     ${HeartRateSample_BINARY_DIR}/bin CACHE PATH
     "Output directory for the HeartRateSample executable" )


set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -O3" )


set(sample_sources
	src/main.cpp 
	src/heartratesample.h src/heartratesample.cpp
	src/CameraUtilities.h src/CameraUtilities.cpp

)

add_executable(HeartRateSample WIN32 ${sample_sources} ${UIS_HDRS} ${resources})
qt5_use_modules(HeartRateSample Widgets)
include_directories(${HRDSDK_DIR}/include)

if(WIN32)
	target_link_libraries(HeartRateSample  ${HRDSDK_DIR}/lib/HrDLib.lib)
endif(WIN32)
if(UNIX)
	target_link_libraries(HeartRateSample  ${HRDSDK_DIR}/lib/libHrDLib.so)
endif(UNIX)
