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
	QObject* CurHolder = nullptr;//��ǰӵ�и���ʵ����QObject

public slots:
	void onStopRecording(const QString &SavePath);

protected:
	LinkedStream();
private:
	bool firstInsert = true;
	static LinkedStream* DataStream;
	//TODO:����edf��ʽ���ݽṹ,�����¼ʱ�����������
	static StreamNode* Head;//����Ϊ��̬,��֤���̷߳��ʹ����б�����������
	static StreamNode* Tail;
	static StreamNode* plotSamplePtr;//��ͼ�ɼ�ָ��
	unsigned int avilableLen;//�����ܳ���,������ͷ��β�ڵ�
	unsigned int showedLen;//�Ѿ�������ͼ�ĳ���
	bool m_record;
};
