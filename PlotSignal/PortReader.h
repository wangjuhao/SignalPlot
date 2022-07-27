#pragma once
#include "globalDef.h"
#include "LinkedStream.h"
#include "qcustomplot.h"
#include <QGroupBox>
#include <QThread>
#include <QPixmap>

class QSerialPort;
class QCustomPlot;
class QTimer;

class PortReader : public QObject
{
<<<<<<< HEAD
	friend class SignalPlotter;
	Q_OBJECT
public:
	PortReader(const QVector<int>& inChannels);
	~PortReader();
	void sendPict();
private:
	static void updateYAxisData(QVector<double>& newData, int channelIndex);

public slots:
	void readFrame();
	void updatePict();
	void onWindowSizeChanged(const QSize& size);
	void onOpenChannelChanged(const QVector<int>& newChannel);
signals:
	void repaintPict(const QPixmap& pict);
private:
	QTimer* timer;
	QSerialPort* m_serialPort;
	QSize m_windowSize;
=======
	friend class PlotSignal;
	Q_OBJECT
public:
	PortReader(const QVector<int> &inChannels);
	~PortReader();

private:
	static void updateYAxisData(const QVector<double> &newData, int channelIndex);

public slots:
	void readFrame();
	void UpdatePict();
	void changeWindowSize_SLOT(const QSize &size);
signals:
	void sendRepaintPict(QPixmap pict);

private:
	QTimer* timer;
	QSerialPort* m_serialPort;
	QSize windowSize;
	int channelCount = 0;

	QVector<int> openChannel;
	
	QCustomPlot* pictSaver;
	static QVector<double> xAsix;
	static QMap<int,QVector<double>> yAxis;
};
>>>>>>> 6f4a008 (链表阻塞)

	QVector<int> m_openChannel;

	QCustomPlot* pictSaver;
	static QVector<double> xAsix;
	static QMap<int, QVector<double>> yAxis;
	
	int m_chnlCount = 0;
};