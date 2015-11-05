#include "heartratesample.h"
#include <QPainter>
#include <QPen>
#include <QMessageBox>

#include <iostream>
#include <chrono>
#include "CameraUtilities.h"

namespace 
{
	/**
	* Gets the current time, as time since the epoch
	*
	* @return the current time in milliseconds
	*
	*/
	long long getCurrentTime()
	{
		return std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count();
	}
}


HeartRateSample::HeartRateSample(QWidget *parent)
	: QMainWindow(parent), sdk("./config"), signal(SIGNAL_SIZE, 0), maxTimeDiff(1000.0 / fps)
{
	int licenseType = sdk.getLicenseType();
	if(licenseType != 1 && licenseType != 3)
	{
		hasValidLicense = false;
		QMessageBox::critical(0, "Error!", 
			"Invalid License! \nPlease purchase a license to use this sample.", QMessageBox::Ok);
		return;
	}
	hasValidLicense = true;

	ui.setupUi(this);
	ui.heartImageLabel->setPixmap(QPixmap(":HeartRateSample/Resources/Heart4040.png"));
	ui.heartImageLabel->setScaledContents(true); 
	setWindowIcon(QIcon(":HeartRateSample/Resources/appIcon.ico"));

	ui.trackMessageLabel->setVisible(false);
	ui.heartMessageLabel->setVisible(false);

	/* Show the zeroed signal */
	showSignal(signal);


	cameraButtons.insert(cameraButtons.end(), { ui.camera1Btn, ui.camera2Btn, ui.camera3Btn });
	
	
	auto devices = cameraUtils::getCameras();
	for (int i = devices.size(); i < cameraButtons.size(); ++i)
	{
		cameraButtons[i]->setEnabled(false);
	}


	sdk.easySetImgFromDevice(0, imgData);
	cameraButtons[0]->setEnabled(false);


	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(loop()));

	timerIdleLoop = new QTimer(this);
	connect(timerIdleLoop, SIGNAL(timeout()), this, SLOT(idleLoop()));
	timerIdleLoop->start(50);
}


HeartRateSample::~HeartRateSample()
{
	if(timer != nullptr)
	{
		killTimer(timer->timerId());
		delete timer;
	}

	if(timerIdleLoop != nullptr)
	{
		killTimer(timerIdleLoop->timerId());
		delete timerIdleLoop;
	}

}


bool HeartRateSample::hasLicense() const
{
	return hasValidLicense;
}


void HeartRateSample::loop()
{

	/* Check if there has been enough time between now and the last analysis */
	auto const timeDiff = getCurrentTime() - prevTime;
	if (!isFirstFrame && timeDiff < maxTimeDiff)
	{
		return;
	}


	isFirstFrame = false;
	prevTime = getCurrentTime();
	

	int const trackResult = sdk.track();
	showImage();

	if (trackResult == -1)
	{
		printMessage(ui.trackMessageLabel, "Face not found!");
		return;
	}
	ui.trackMessageLabel->setText("");
	ui.trackMessageLabel->setVisible(false);


	std::vector<double> values;
	int const bpm = sdk.getHeartRate(values);
	

	bool const isError = handleHeartRateValue(bpm);
	if (isError)
	{
		return;
	}
	
	if (!values.empty())
	{
		signal.insert(signal.end(), values.begin(), values.end());
	}

	/* Remove the oldest value of the signal */
	if (signal.size() > SIGNAL_SIZE + 32)
	{
		signal.erase(signal.begin());
	}

	showSignal({ signal.cbegin(), signal.cbegin() + SIGNAL_SIZE });
}


bool HeartRateSample::handleHeartRateValue(int const bpm)
{
	if (bpm == -1)
	{
		printMessage(ui.heartMessageLabel, "Error in the analysis!");
		return true;
	}
	else if (bpm == -2)
	{
		printMessage(ui.heartMessageLabel, "Framerate is lower than required!");
		heartErrored = true;
		lastHeartErrorTime = getCurrentTime();
		return true;
	}
	else if (bpm == -3)
	{
		reset();
		printMessage(ui.heartMessageLabel, "Resetting analysis!");
		heartErrored = true;
		lastHeartErrorTime = getCurrentTime();
		return true;
	}

	if (heartErrored)
	{
		/* Keep the error message showing for 1.6 seconds */
		auto const diff = getCurrentTime() - lastHeartErrorTime;
		int const maxDiff = 1600; // 1.6 seconds
		if (diff > maxDiff)
		{
			ui.heartMessageLabel->setText("");
			ui.heartMessageLabel->setVisible(false);
			heartErrored = false;
		}
	}

	ui.bpmLabel->setText(QString::fromStdString(std::to_string(bpm)));
	return false;
}


void HeartRateSample::on_startButton_clicked()
{
	if (!started)
	{
		ui.trackMessageLabel->setText("");
		ui.trackMessageLabel->setVisible(false);
		ui.heartMessageLabel->setText("");
		ui.heartMessageLabel->setVisible(false);

		ui.startButton->setText("Stop");
		timer->start(0);
		timerIdleLoop->stop();
	}
	else
	{
		ui.startButton->setText("Start");
		reset();
		sdk.resetHeartRate();
		timer->stop();
		timerIdleLoop->start(50);
	}

	started = !started;
}


void HeartRateSample::reset()
{
	isFirstFrame = true;

	signal.clear();
	signal.insert(signal.end(), SIGNAL_SIZE, 0);
	
	showSignal(signal);

	ui.bpmLabel->setText(QString::fromStdString("0"));
}


void HeartRateSample::idleLoop()
{
	sdk.track();
	showImage();
}


void HeartRateSample::on_camera1Btn_clicked()
{
	int const index = 0;
	cameraBtnClicked(index);
}


void HeartRateSample::on_camera2Btn_clicked()
{
	int const index = 1;
	cameraBtnClicked(index);
}


void HeartRateSample::on_camera3Btn_clicked()
{
	int const index = 2;
	cameraBtnClicked(index);
}


void HeartRateSample::cameraBtnClicked(int const index)
{
	if (index != currentDevice)
	{
		cameraButtons[currentDevice]->setEnabled(true);
		currentDevice = index;
		sdk.easySetImgFromDevice(index, imgData);

		if (started)
		{
			ui.startButton->click();
		}
	
		cameraButtons[index]->setEnabled(false);
	}
}


void HeartRateSample::printMessage(QLabel * label, std::string const & message)
{
	label->setText(QString::fromStdString(message));
	label->setVisible(true);
	label->adjustSize();
}


void HeartRateSample::showImage()
{
	QImage imgIn((uchar*)imgData, FRAME_WIDTH, FRAME_HEIGHT, QImage::Format_RGB888);
	imgIn = imgIn.rgbSwapped();
	imgIn = imgIn.mirrored(true, false);
	
	QPixmap pixmap;
	pixmap.convertFromImage(imgIn);
	ui.cameraLabel->setPixmap(pixmap);
	ui.cameraLabel->setScaledContents(true);
}


void HeartRateSample::showSignal(std::vector<double> const & signal)
{
	QPixmap signalPixmap;
	signalPixmap.convertFromImage(getSignalImage(signal));
	ui.signalLabel->setPixmap(signalPixmap);
}


QImage HeartRateSample::getSignalImage(std::vector<double> const & signal) const
{
	// Used to normalize the values between 0 and 1
	double max = *std::max_element(signal.cbegin(), signal.cend());
	double min = *std::min_element(signal.cbegin(), signal.cend());

	if (max == 0 && min == 0)
	{
		max = 1;
		min = -1;
	}

	double const range = max - min;

	int const width = ui.signalLabel->width();
	int const height = ui.signalLabel->height();
	int const halfHeight = height / 2.0;
	int const size = signal.size();
	int const step = width / size;


	QImage image(width, height, QImage::Format_RGB888);
	image.fill(palette().color(QPalette::Background).rgb());

	QPainter painter(&image);
	painter.setPen(QPen(Qt::red, 2));

	
	QPoint p0(0, halfHeight);
	QPoint p1(0, 0);
	for (int i = 0; i < size; ++i)
	{
		p1.setX(i * step + 1);

		double const newVal = (signal[i] - min) / range;
		p1.setY(halfHeight / 2.0 + (halfHeight - (newVal * halfHeight)));

		painter.drawLine(p0, p1);
		p0 = p1;
	}

	return image;
}
