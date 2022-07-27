#include "PortReader.h"
#include "qcustomplot.h"
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QDateTime>
#include <QSerialPort>
#include <QMouseEvent>
#include <QtConcurrent>
#include <QSerialPortInfo>
#include <QCoreApplication>

QVector<double> PortReader::xAsix = QVector<double>();
QMap<int, QVector<double>> PortReader::yAxis = QMap<int, QVector<double>>();

int FindHeader(const QByteArray& arr, int beginPos)
{
	int parseIndex = 0;
	int index = beginPos;
	static QByteArray parseExp = "bbaa";
	while (parseIndex < parseExp.size() && index < arr.size())
	{
		if (arr[index++] == parseExp[parseIndex++])
			continue;
		else
			parseIndex = 0;
	}
	return index;
}

uint inline Byte2Hex(const char& byte)
{
	if (byte - '0' <= '9' - '0')
		return byte - '0';
	if (byte - 'a' <= 'f' - 'a')
		return byte - 'a' + 10;
	return -1;
}

int inline intConverterFromByte3(uint8_t inByte[3])
{
	int ChannelDataInt = 0x00000000;
	for (char j = 0; j < 3; j++)
	{
		ChannelDataInt = (ChannelDataInt << 8) | inByte[j];
	}
	if ((ChannelDataInt & 0x00800000) == 0x00800000)
	{
		ChannelDataInt |= 0XFF000000;
	}
	else {
		ChannelDataInt &= 0X00FFFFFF;
	}
	return ChannelDataInt;
}

bool VerifyFrame(const QByteArray& arr, int beginPos)
{
	uint checksum = 0;
	int endPos = beginPos + 3 * CHANNELNUM * 2;
	for (int index = beginPos; index < endPos; index += 2)
	{
		bool ok = true;
		checksum += (Byte2Hex(arr[index]) << 4);
		checksum += (Byte2Hex(arr[index + 1]));
		uint Converted = (Byte2Hex(arr[index]) << 4) + (Byte2Hex(arr[index + 1]));
	}
	QString s;
	s += arr[endPos];
	s += arr[endPos + 1];
	return (QString::number(~checksum & 0x000000FF, CHANNELNUM) == s);
}

<<<<<<< HEAD
PortReader::PortReader(const QVector<int>& inChannels) : m_openChannel(inChannels),m_chnlCount(inChannels.count())
{
	QString s;
	for (auto channel : m_openChannel)
		s += QString::number(channel);
	m_serialPort = new QSerialPort(this);
	pictSaver = new QCustomPlot(0);
	pictSaver->setFixedSize(960, 720);
	pictSaver->plotLayout()->clear();
	pictSaver->plotLayout()->setColumnSpacing(0);
	
=======
PortReader::PortReader(const QVector<int>& inChannels) : openChannel(inChannels)
{
	m_serialPort = new QSerialPort(this);
	pictSaver = new QCustomPlot(0);
	pictSaver->setFixedSize(QApplication::desktop()->width(), QApplication::desktop()->height()-30);
	pictSaver->plotLayout()->clear();
	pictSaver->plotLayout()->setColumnSpacing(0);
>>>>>>> 6f4a008 (閾捐〃闃诲)
	xAsix.resize(1000);
	for (int i = 0; i < 1000; i++)
	{
		xAsix[i] = i / 500.0;
	}

	for (int i = 0; i < 16; i++)
	{
		yAxis[i].resize(1000);
	}

<<<<<<< HEAD
	for (int i = 0; i < m_chnlCount; i++)
=======
	int channelCount = openChannel.count();
	this->channelCount = channelCount;

	for (int i = 0; i < channelCount; i++)
>>>>>>> 6f4a008 (閾捐〃闃诲)
	{
		QCPAxisRect* AxisRect = new QCPAxisRect(pictSaver);
		//设置x轴相关参数
		AxisRect->axis(QCPAxis::atBottom)->setRange(0, 2);
		AxisRect->axis(QCPAxis::atBottom)->setTickPen(QPen(QColor(50, 50, 50), 3, Qt::DotLine));
		//设置y轴相关参数
		AxisRect->axis(QCPAxis::atLeft)->setRange(-MAXRANGE, MAXRANGE);
		AxisRect->axis(QCPAxis::atLeft)->setVisible(false);
		pictSaver->plotLayout()->addElement(i, 0, AxisRect);
		pictSaver->addGraph(AxisRect->axis(QCPAxis::atBottom), AxisRect->axis(QCPAxis::atLeft));
	}

<<<<<<< HEAD
	for (int i = 0; i < m_chnlCount; i++)
=======
	for (int i = 0; i<channelCount; i++)
>>>>>>> 6f4a008 (閾捐〃闃诲)
	{
		pictSaver->graph(i)->setData(xAsix, yAxis[i]);
	}

	const auto infos = QSerialPortInfo::availablePorts();
	this->setObjectName("PortReader");
	timer = new QTimer(this);
<<<<<<< HEAD
	connect(timer, &QTimer::timeout, this, &PortReader::updatePict);
	timer->start(2);
=======
	connect(timer, &QTimer::timeout, this, &PortReader::UpdatePict);
	timer->start(10);
>>>>>>> 6f4a008 (閾捐〃闃诲)

	if (infos.size() > 0)
	{
		m_serialPort->setPortName("COM3");
		if (m_serialPort->open(QIODevice::ReadWrite))
		{
			m_serialPort->setBaudRate(460800);
			m_serialPort->setParity(QSerialPort::NoParity);
			m_serialPort->setStopBits(QSerialPort::OneStop);
			m_serialPort->setDataBits(QSerialPort::Data8);
			m_serialPort->setRequestToSend(QSerialPort::HardwareControl);
			connect(m_serialPort, &QSerialPort::readyRead, this, &PortReader::readFrame);
		}
	}
}

PortReader::~PortReader()
{
	m_serialPort->close();
	delete pictSaver;
	delete m_serialPort;
	delete timer;

}
<<<<<<< HEAD

void PortReader::sendPict()
{
	pictSaver->replot();
	emit repaintPict(pictSaver->grab());
}

void PortReader::updateYAxisData(QVector<double>& newData, int channelIndex)
{
	//加reverse才能保证y轴数据顺序的正确性,见VSCode-C++ memmove-test.cpp做的测试
	reverse(newData.begin(), newData.end());
	int updateLength = newData.size();
	auto data = yAxis[channelIndex].data();
	auto newdata = newData.data();

	memmove(data + updateLength, data, (1000 - updateLength + 1) * sizeof(double));
	memmove(data, newdata, updateLength * sizeof(double));
}
void PortReader::updatePict()
{
	auto newPack = LinkedStream::InstanceGet()->extractData(2);
	auto grabPict = [&]() {
		for (auto channel : m_openChannel)
		{
			updateYAxisData(newPack[channel], channel);
		}
		for (int i = 0; i < m_chnlCount; i++)
		{
			pictSaver->graph(i)->setData(xAsix, yAxis[m_openChannel[i]]);
		}
		pictSaver->replot();
		QPixmap pict = pictSaver->grab();
		return pict;
	};
	//另一个线程异步执行此代码
	QEventLoop loop;
	QFutureWatcher<QPixmap> watcher;
	connect(&watcher, &QFutureWatcher<QPixmap>::finished, &loop, &QEventLoop::quit);
	watcher.setFuture(QtConcurrent::run(grabPict));
	loop.exec();

	emit repaintPict(watcher.future().result());
}

void PortReader::onWindowSizeChanged(const QSize& size)
{
	this->m_windowSize = size;
}

void PortReader::onOpenChannelChanged(const QVector<int>& newChannel)
{
	m_openChannel = newChannel;
=======
void PortReader::updateYAxisData(const QVector<double>& newData,int channelIndex)
{
	int updateLength = newData.size();
	int i;
	for (i = 999; i >= updateLength; i--)
	{
		yAxis[channelIndex][i] = yAxis[channelIndex][i - updateLength];
	}
	for (i = 0; i < updateLength; i++)
	{
		qDebug() << newData[i];
		yAxis[channelIndex][i] = newData[i];
	}
}

void PortReader::UpdatePict()
{
	QMap<int, QVector<double>> res = LinkedStream::InstanceGet()->extractData(1);
	auto runFunc = [&]() {
		for (auto channel : openChannel)
		{
			updateYAxisData(res[channel], channel);
		}
		for (int i = 0; i < channelCount; i++)
		{
			pictSaver->graph(i)->setData(xAsix, yAxis[channelCount]);
		}
		pictSaver->replot();
		QPixmap pict = pictSaver->grab();
		return pict;
	};
	//另一个线程异步执行此代码
	QEventLoop loop;
	QFutureWatcher<QPixmap> watcher;
	connect(&watcher, &QFutureWatcher<QPixmap>::finished, &loop, &QEventLoop::quit);
	watcher.setFuture(QtConcurrent::run(runFunc));
	loop.exec();

	QPixmap pict = watcher.future().result();
	emit sendRepaintPict(pict);
}

void PortReader::changeWindowSize_SLOT(const QSize& size)
{
	this->windowSize = size;
>>>>>>> 6f4a008 (閾捐〃闃诲)
}

static QByteArray  restData;
void PortReader::readFrame()
{
<<<<<<< HEAD
	QByteArray requestData = m_serialPort->readAll().toHex();
=======
	QByteArray requestData = m_serialPort->readLine().toHex();
>>>>>>> 6f4a008 (閾捐〃闃诲)
	if (!requestData.isEmpty())
	{
		requestData = restData + requestData;
		int infosize = requestData.size();
		int searchIndex = 0;
		while ((searchIndex = FindHeader(requestData, searchIndex)) < infosize - 100)
		{
<<<<<<< HEAD
			if (VerifyFrame(requestData, searchIndex))
			{
				array<int, 16> data4EachChannel;
				uint8_t inByte3[3];
=======

			if (VerifyFrame(requestData, searchIndex))
			{
				uint8_t inByte3[3];
				array<int,16> data4EachChannel;
>>>>>>> 6f4a008 (閾捐〃闃诲)
				for (int i = 0; i < CHANNELNUM; i++)
				{
					int curIndex = searchIndex + i * 3 * 2;
					inByte3[0] = ((Byte2Hex(requestData[curIndex]) << 4) + Byte2Hex(requestData[curIndex + 1]));
					inByte3[1] = ((Byte2Hex(requestData[curIndex + 2]) << 4) + Byte2Hex(requestData[curIndex + 3]));
					inByte3[2] = ((Byte2Hex(requestData[curIndex + 4]) << 4) + Byte2Hex(requestData[curIndex + 5]));
					data4EachChannel[i] = (double)intConverterFromByte3(inByte3);
				}
				LinkedStream::InstanceGet()->InsertNode(data4EachChannel);
			}
			searchIndex += 100;
		}
		searchIndex = FindHeader(requestData, searchIndex - 100);
		restData = requestData.mid(searchIndex);
	}
}

