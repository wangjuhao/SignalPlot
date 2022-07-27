#include "SignalPlotter.h"
#include <QPainter>
#include <QGroupBox>
#include <QVBoxLayout>

SignalPlotter::SignalPlotter(const QVector<int>& openChannels, QWidget* parent) : m_openChannels(openChannels)
{
    this->setGeometry(100,100,1060,820);

    debugInfoTimer = new QTimer(this);
    connect(debugInfoTimer, &QTimer::timeout, this, &SignalPlotter::printRefreshRate);
    debugInfoTimer->start(1000);
    reader = new PortReader(m_openChannels);
    connect(reader, &PortReader::repaintPict, this, &SignalPlotter::onPictUpdated);
    reader->sendPict();

    gBox = new QGroupBox("EEG Signal(100μmV/Div-2s/page)", this);
    gBox->setGeometry(20,20, 1060, 820);
    pictWrapper = new QLabel(gBox);
    pictWrapper->setPixmap(curPict);
    QVBoxLayout* outerLay = new QVBoxLayout(gBox);
    QVBoxLayout* innerLay = new QVBoxLayout(gBox);
    innerLay->addWidget(pictWrapper);
    innerLay->setContentsMargins(0, 0, 0, 0);

    gBox->setLayout(outerLay);
    outerLay->addLayout(innerLay);
    outerLay->setContentsMargins(50, 10, 20, 3);

    readerThread = new QThread(this);
    connect(readerThread, &QThread::finished, reader, &PortReader::deleteLater);
    reader->moveToThread(readerThread);
    readerThread->start();

    connect(this, &SignalPlotter::channelChanged, reader, &PortReader::onOpenChannelChanged);
    connect(this, &SignalPlotter::windowSizeChanged, reader, &PortReader::onWindowSizeChanged);
    
}

SignalPlotter::~SignalPlotter()
{
	readerThread->wait();
	delete reader;
	delete readerThread;
    LinkedStream::InstanceGet()->~LinkedStream();
}

QSize SignalPlotter::getWindowSize() const
{
    return m_winSize;
}

void SignalPlotter::setWindowSize(const QSize& newSize)
{
    m_winSize = newSize;
    emit windowSizeChanged(newSize);
}

QVector<int> SignalPlotter::getChannels() const
{
    return m_openChannels;
}

void SignalPlotter::setChannels(const QVector<int>& newChannel)
{
    m_openChannels = newChannel;
    emit channelChanged(newChannel);
}

static int refreshCount = 0;
static int preCount = 0;

void SignalPlotter::paintEvent(QPaintEvent* event)
{
    refreshCount++;
    pictWrapper->setPixmap(curPict);
}

void SignalPlotter::printRefreshRate()
{
	qDebug() << "current refreshRate:" << refreshCount - preCount;
	preCount = refreshCount;
}

void SignalPlotter::onPictUpdated(const QPixmap& pict)
{
    curPict = pict;
    update();
}
