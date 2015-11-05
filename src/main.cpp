#include "heartratesample.h"
#include <QtWidgets/QApplication>
#include "ui_loadingscreen.h"
#include <qdialog.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QDialog dialog;
	Ui::LoadingScreen ui;
	ui.setupUi(&dialog);
	dialog.setWindowIcon(QIcon(":HeartRateSample/Resources/appIcon.ico"));
	ui.hrIconLabel->setPixmap(QPixmap(":HeartRateSample/Resources/appIcon.png"));
	
	dialog.show();

	QApplication::processEvents();

	HeartRateSample w;
	dialog.close();
	if(!w.hasLicense())
	{
		return 0;
	}
	w.show();
	return a.exec();
}
