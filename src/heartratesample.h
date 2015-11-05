#ifndef HEARTRATESAMPLE_H
#define HEARTRATESAMPLE_H

#include <QtWidgets/QMainWindow>
#include "ui_heartratesample.h"
#include <HrDLib.h>
#include <vector>
#include <string>

#include <QPushButton>
#include <QImage>
#include <QTimer>


class HeartRateSample : public QMainWindow
{
	Q_OBJECT

public:
	HeartRateSample(QWidget *parent = 0);
	~HeartRateSample();


	/**
	* Checks if the license is valid for the sample
	*
	* @return whether the license is valid or not
	*/
	bool hasLicense() const;


private:
	Ui::HeartRateSampleClass ui;
	
	/* The camera being used */
	QTimer * timer = nullptr;
	
	/* The timer  */
	QTimer * timerIdleLoop = nullptr;

	/* The buttons that allow the user to choose the camera to use */
	std::vector<QPushButton*> cameraButtons;

	/* The camera being used */
	int currentDevice = 0;


	/* The HealthyRoad SDK */
	HrDLib sdk;
	
	/* Pointer to the image being captured by the camera */
	void * imgData;


	/* The number of frames to be analyzed per second */
	int fps = 20;

	/* The PPG signal */
	std::vector<double> signal;

	/* The time between analysis of each frame */
	double const maxTimeDiff;

	/* The time between analysis of each frame */
	long long prevTime = 0;
	
	/*  Boolean indicating if the loop is on the first frame */
	bool isFirstFrame = true;
	
	/*  Boolean indicating if the analysis has started */
	bool started = false;


	/* Boolean indicating if the analysis errored on the getHeartRate step */
	bool heartErrored = false;
	
	/* Time of the last error in the getHeartRate() step */
	long long lastHeartErrorTime = 0;


	static const int FRAME_WIDTH = 640;
	static const int FRAME_HEIGHT = 480;

	static const int SIGNAL_SIZE = 150;


	bool hasValidLicense = false;

	/**
	* Handles the value returned by the getHeartRate() function of the SDK,
	* printing error messages or the value
	*
	* @param bpm   the value returned by the getHeartRate() function
	*
	* @return  true if bpm is equivalent to an error (negative), false if not
	*/
	bool handleHeartRateValue(int bpm);


	/**
	* Resets the signal and bpm labels on the interface
	*
	* @return void, no return
	*/
	void reset();


	/**
	* Prints a message in a label
	*
	* @param label	   the label that will be written to
	* @param message   the message to print
	*
	* @return void, no return
	*/
	void printMessage(QLabel * label, std::string const & message);


	/**
	* Displays the image captured by the camera
	*
	* @return void, no return
	*/
	void showImage();

	/**
	* Displays a signal in the signal label
	*
	* @param signal   the signal to display
	*
	* @return void, no return
	*/
	void showSignal(std::vector<double> const & signal);
	

	/**
	* Creates a QImage of a signal
	*
	* @param signal   the signal
	*
	* @return  the QImage with the signal drawn
	*/
	QImage getSignalImage(std::vector<double> const & signal) const;


	/**
	* Changes the camera being used according to the button that was clicked
	*
	* @param index   the index of the clicked camera button
	*
	* @return void, no return
	*/
	void cameraBtnClicked(int index);



private slots:
	/**
	* The main loop called by the timer QTimer
	*
	* @return void, no return
	*/
	void loop();

	/**
	* The loop called when the main loop isn't running
	*
	* @return void, no return
	*/
	void idleLoop();


	/* Button Events */
	void on_startButton_clicked();
	void on_camera1Btn_clicked();
	void on_camera2Btn_clicked();
	void on_camera3Btn_clicked();

};

#endif // HEARTRATESAMPLE_H
