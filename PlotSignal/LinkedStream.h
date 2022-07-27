#pragma once
#include "globalDef.h"
#include <QObject>
#include <QMutex>
#include <array>

using namespace std;
static QMutex LinkedStream_Constructor_Mutex;
static QMutex PlotSampleLocker;
static QMutex InsertLocker;

struct StreamNode
{
public:
	array<int, 16> arrayData;
	StreamNode* pPre = nullptr, * pNext = nullptr;
};

Q_DECLARE_METATYPE(StreamNode);

class LinkedStream : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool Record READ isRecording WRITE setRecordStatus);
public:
	static LinkedStream* InstanceGet();
	void initParam(const QVector<int> &openedChannel,bool recordable);
	bool isRecording() const;
	void setRecordStatus(bool recordable);
	void InsertNode(const array<int, 16>& d);
	Pack extractData(int length);
	~LinkedStream();
public:
	QObject* CurHolder = nullptr;//当前拥有该类实例的QObject

public slots:
	void onStopRecording(const QString &SavePath);

protected:
	LinkedStream();
private:
	bool firstInsert = true;
	static LinkedStream* DataStream;
	//TODO:定义edf格式数据结构,点击记录时往里面存数据
	static StreamNode* Head;//定义为静态,保证多线程访问过程中变量不被复制
	static StreamNode* Tail;
	static StreamNode* plotSamplePtr;//绘图采集指针
	unsigned int avilableLen;//链表总长度,不包含头、尾节点
	unsigned int showedLen;//已经送至绘图的长度
	bool m_record;
};
