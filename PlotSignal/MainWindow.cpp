#include "MainWindow.h"
#include "styleSheet_global.h"

MainWindow::MainWindow()
{
	changeStyle("cssSytles/darkgray.css");
	overallLayout = new QHBoxLayout(this);
}

MainWindow::~MainWindow()
{

}
