#pragma once
#include <QMainWindow>
#include <QSettings>
#include <QGroupBox>
#include <QHBoxLayout>
#include "SignalPlotter.h"

class MainWindow : QMainWindow
{
public:
	MainWindow();
	~MainWindow();

private:
	QSettings m_settings;
	QHBoxLayout* overallLayout;
};

