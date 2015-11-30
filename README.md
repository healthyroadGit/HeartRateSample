# IncrediblEye - HeartRate Sample
IncrediblEye 2015




This is a sample of the HeartRate functionality of the IncrediblEye SDK.

Compiled executables can be found in the IncrediblEye Developer Zone [(click here)](http://www.incredibleye.com/samplehr.php).


## Prerequisites
You'll need to install:

- Qt 5.4.x
- CMake 2.8.11+
- IncrediblEye SDK


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


If CMake complains about Qt5Widgets, that means there is no environment variable set to the Qt directory, and you'll need to add this parameter to the cmake command:

`
-D CMAKE_PREFIX_PATH="<path/to/qt/>"
`

where the path leads to the directory containing Qt's /bin directory. 


## Running

To run the generated executable, you'll need to place the following directory in the same directory as the executable:

- config (from the IncrediblEye SDK)


## Using the application

The application has a simple interface:

- One main area that displays the image being captured from the camera
- A small area below the image, where error messages appear
- One button to start/stop the heart rate analysis
- Three buttons to change between camera devices (if applicable)
- One area below the three buttons, with the PPG signal ([wikipedia article](https://en.wikipedia.org/wiki/Photoplethysmogram)) and the value of the calculated Heart Rate

Note that in Linux, due to OpenCV, the cameras might only work the first time you select them. Restarting the application will make them work again.
