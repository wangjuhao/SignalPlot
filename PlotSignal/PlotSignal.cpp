#include "PlotSignal.h"
#include <QPainter>

PlotSignal::PlotSignal(const QVector<int>& openChannels,QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(QApplication::desktop()->width(), QApplication::desktop()->height()-30);
    this->move(0, 0);
    reader = new PortReader(openChannels);
    this->curPict = reader->pictSaver->grab();
    update();
    connect(reader, &PortReader::sendRepaintPict, this, &PlotSignal::recievePict);
    readerThread = new QThread(this);
    connect(readerThread, &QThread::finished, reader, &PortReader::deleteLater);
    reader->moveToThread(readerThread);
    readerThread->start(QThread::Priority::HighPriority);

}

PlotSignal::~PlotSignal()
{
    readerThread->wait();
    delete reader;
    delete readerThread;
    LinkedStream::InstanceGet()->~LinkedStream();
}

void PlotSignal::paintEvent(QPaintEvent* event)
{
    static QPainter painter;
    painter.begin(this);
    painter.drawPixmap(QPoint(0,0),this->curPict);
    painter.end();
}

void PlotSignal::recievePict(const QPixmap pict)
{
    this->curPict = pict;
    update();
}

