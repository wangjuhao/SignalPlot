#pragma once

#include "LinkedStream.h"
#include "PortReader.h"
#include <QThread>
#include <QtWidgets/QWidget>

class PlotSignal : public QWidget
{
    Q_OBJECT

public:
    PlotSignal(const QVector<int> &openChannels,QWidget *parent = nullptr);
    ~PlotSignal();
protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void changeWindowSize_SIG(const QSize& size);
public slots:
    void recievePict(const QPixmap pict);
private:
    PortReader* reader;
    QThread* readerThread;

    QPixmap curPict;
};
