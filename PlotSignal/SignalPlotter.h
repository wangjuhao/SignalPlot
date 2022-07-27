#pragma once

#include "PortReader.h"
#include <QThread>
#include <QtWidgets/QWidget>
#include <QLabel>
#include <QGroupBox>

class SignalPlotter : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QSize windowSize READ getWindowSize WRITE setWindowSize NOTIFY windowSizeChanged);
	Q_PROPERTY(QVector<int> openChannels READ getChannels WRITE setChannels NOTIFY channelChanged);
public:
	SignalPlotter(const QVector<int>& openChannels, QWidget* parent = nullptr);
	~SignalPlotter();
	QSize getWindowSize() const;
	void setWindowSize(const QSize& newSize);
	QVector<int> getChannels() const;
	void setChannels(const QVector<int>& newChannel);
protected:
	void paintEvent(QPaintEvent* event) override;

signals:
	void windowSizeChanged(const QSize &newSize);
	void channelChanged(const QVector<int> &newChannel);

public slots:
	void printRefreshRate();
	void onPictUpdated(const QPixmap& pict);

private:
	PortReader* reader;
	QThread* readerThread;
	QTimer* debugInfoTimer;
	QLabel* pictWrapper;
	QGroupBox* gBox;

	QVector<int> m_openChannels;
	QSize m_winSize;
	QPixmap curPict;
};