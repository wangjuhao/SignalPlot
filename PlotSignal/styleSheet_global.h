#pragma once
#include <QFile>
#include <QWidget>
#include <QString>
#include <QApplication>

static QString textColor_global;
static QString panelColor_global;
static QString borderColor_global;
static QString normalColorStart_global;
static QString normalColorEnd_global;
static QString colorDarkStart_global;

static QString darkColorEnd_global;
static QString highColor_global;

static inline void checkColor(const QString &str)
{
	QString flagTextColor = "TextColor:";
	int indexTextColor = str.indexOf(flagTextColor);

	if (indexTextColor >= 0) {
		QString strTextColor = str.mid(indexTextColor + flagTextColor.length(), 7);
		textColor_global = strTextColor;
	}

	QString flagPanelColor = "PanelColor:";
	int indexPanelColor = str.indexOf(flagPanelColor);

	if (indexPanelColor >= 0) {
		QString strPanelColor = str.mid(indexPanelColor + flagPanelColor.length(), 7);
		panelColor_global = strPanelColor;
	}

	QString flagBorderColor = "BorderColor:";
	int indexBorderColor = str.indexOf(flagBorderColor);

	if (indexBorderColor >= 0) {
		QString strBorderColor = str.mid(indexBorderColor + flagBorderColor.length(), 7);
		borderColor_global = strBorderColor;
	}

	QString flagNormalColorStart = "NormalColorStart:";
	int indexNormalColorStart = str.indexOf(flagNormalColorStart);

	if (indexNormalColorStart >= 0) {
		QString strNormalColorStart = str.mid(indexNormalColorStart + flagNormalColorStart.length(), 7);
		normalColorStart_global = strNormalColorStart;
	}

	QString flagNormalColorEnd = "NormalColorEnd:";
	int indexNormalColorEnd = str.indexOf(flagNormalColorEnd);

	if (indexNormalColorEnd >= 0) {
		QString strNormalColorEnd = str.mid(indexNormalColorEnd + flagNormalColorEnd.length(), 7);
		normalColorEnd_global = strNormalColorEnd;
	}

	QString flagDarkColorStart = "DarkColorStart:";
	int indexDarkColorStart = str.indexOf(flagDarkColorStart);

	if (indexDarkColorStart >= 0) {
		QString strDarkColorStart = str.mid(indexDarkColorStart + flagDarkColorStart.length(), 7);
		colorDarkStart_global = strDarkColorStart;
	}

	QString flagDarkColorEnd = "DarkColorEnd:";
	int indexDarkColorEnd = str.indexOf(flagDarkColorEnd);

	if (indexDarkColorEnd >= 0) {
		QString strDarkColorEnd = str.mid(indexDarkColorEnd + flagDarkColorEnd.length(), 7);
		darkColorEnd_global = strDarkColorEnd;
	}

	QString flagHighColor = "HighColor:";
	int indexHighColor = str.indexOf(flagHighColor);

	if (indexHighColor >= 0) {
		QString strHighColor = str.mid(indexHighColor + flagHighColor.length(), 7);
		highColor_global = strHighColor;
	}
}

static inline void setStyle(const QString &str)
{
	static QString qss;

	QString paletteColor = str.mid(20, 7);
	qApp->setPalette(QPalette(QColor(paletteColor)));
	qApp->setStyleSheet(str);
	qApp->setStyleSheet(QString("QFrame#boxPanel{border-width:0px;background:%1;}"
		"QFrame#gboxDevicePanel,QFrame#gboxDeviceTitle{padding:3px;}")
		.arg(paletteColor));
}

static inline void changeStyle(const QString &qssFile)
{
	QString fileName = qssFile;

	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QFile::ReadOnly)) {
			QString str = file.readAll();
			checkColor(str);
			setStyle(str);
		}
	}
}