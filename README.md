# Healthyroad - HeartRate Sample
HealthyRoad® 2015




This is a sample of the HeartRate functionality of the HealthyRoad SDK.

Compiled executables can be found in the HealthyRoad Developer Zone [(click here)](http://healthyroad.pt/devcenter/en/index.php).


## Prerequisites
You'll need to install:

- Qt 5.4.x
- CMake 2.8.11+
- HealthyRoad SDK


## Building
To build, you'll need to run these commands in the HeartRateSample directory:

**Linux:**

	$ mkdir build
	$ cd build
	$ cmake -D HRDSDK_DIR="<path/to/the/sdk>" ..
	$ make


**Windows (for MS Visual Studio):**

	$ mkdir build
	$ cd build
	$ cmake -G "Visual Studio 12 2013" -D HRDSDK_DIR="<path/to/the/sdk>" ..
	$ start HeartRateSample.sln


If CMake complains about Qt5Widgets, that means there is no environment variable set to the QT directory, and you'll need to add this parameter to the cmake command:

`
-D CMAKE_PREFIX_PATH="<path/to/qt/>"
`

where the path leads to the directory containing Qt's /bin directory. 


## Running

To run the generated executable, you'll need to place the following directory in the same directory as the executable:

- config (from the HealthyRoad SDK)


## Using the application

The application has a simple interface:

- One main area that displays the image being captured from the camera
- A small area below the image, where error messages appear
- One button to start/stop the heart rate analysis
- Three buttons to change between camera devices (if applicable)
- One area below the three buttons, with the PPG signal ([wikipedia article](https://en.wikipedia.org/wiki/Photoplethysmogram)) and the value of the calculated Heart Rate