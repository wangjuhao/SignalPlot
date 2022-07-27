#include "LinkedStream.h"
#include <qDebug>
#include <QThread>

LinkedStream* LinkedStream::DataStream = nullptr;
StreamNode* LinkedStream::Head = new StreamNode();
StreamNode* LinkedStream::Tail = new StreamNode();
StreamNode* LinkedStream::plotSamplePtr = Tail;

LinkedStream::LinkedStream()
{
	qRegisterMetaType<StreamNode>("StreamNode");
	Head->pNext = Tail;
	Head->pPre = Tail;
	Tail->pNext = Head;
	Tail->pPre = Head;
	avilableLen = 0;
	showedLen = 0;
}


LinkedStream* LinkedStream::InstanceGet()
{
	if (DataStream == nullptr)
	{
		QMutexLocker loker(&LinkedStream_Constructor_Mutex);
		if (DataStream == nullptr)
		{
			DataStream = new LinkedStream();
		}
	}
	return DataStream;
}

void LinkedStream::initParam(const QVector<int>& openedChannel, bool recordable)
{
	m_record = recordable;
}

bool LinkedStream::isRecording() const
{
	return m_record;
}

void LinkedStream::setRecordStatus(bool recordable)
{
	m_record = recordable;
}

void LinkedStream::InsertNode(const array<int, 16>& d)
{
	StreamNode* temp = new StreamNode();
	temp->pPre = Head;
	temp->pNext = Head->pNext;
	copy(begin(d), end(d), begin(temp->arrayData));
	if (firstInsert)
	{
		firstInsert = false;
		plotSamplePtr = temp;
	}
	InsertLocker.lock();
	Head->pNext->pPre = temp;
	Head->pNext = temp;
	avilableLen++;
	InsertLocker.unlock();
}

Pack LinkedStream::extractData(int inlength)
{
	Pack res;
	if (inlength == -1)
	{
		PlotSampleLocker.lock();
		int i;
		while (plotSamplePtr->pPre!= Head)
		{
			for (i = 0; i < 16; i++)
			{
				res[i].push_back(plotSamplePtr->arrayData[i]*UNITRESOL);
			}
			plotSamplePtr = plotSamplePtr->pPre;
		}
		PlotSampleLocker.unlock();
		showedLen += avilableLen;
		avilableLen = 0;
	}
	else
	{
		int extractLen = 0;
		if (avilableLen > 100)
			inlength = inlength + 1;//���ݵ�ǰ�����ȵ����޸ķ��ص����ݳ��ȣ���������Խ��Խ��
		qDebug() << avilableLen;
		PlotSampleLocker.lock();
<<<<<<< HEAD
		while ((plotSamplePtr->pPre != Head) && (extractLen < inlength))
=======
		while ((plotSamplePtr != Head) && (extractLen <= length))
>>>>>>> 6f4a008 (链表阻塞)
		{
			for (int i = 0; i < 16; i++)
			{
				res[i].push_back(plotSamplePtr->arrayData[i]*UNITRESOL);
			}
			extractLen++;
			StreamNode* temp = plotSamplePtr;
			plotSamplePtr = plotSamplePtr->pPre;
			if (m_record)
			{
				//TODO:
				/*��temp�ڵ����ݴ��浽edf�ṹ�嵱��ȥ
				*/
			}
			delete temp;
		}
		PlotSampleLocker.unlock();
		showedLen += extractLen;
		avilableLen -= extractLen;
	}
	return res;
}

LinkedStream::~LinkedStream()
{
	StreamNode* tmp;
	StreamNode* beginNode = Head->pNext;//������Ҫ��Head���ж�,���ܴ�Head��ʼ����
	while (beginNode->pNext != Head)
	{
		tmp = beginNode;
		delete tmp;
		beginNode = beginNode->pNext;
	}
	delete Head;
}

void LinkedStream::onStopRecording(const QString& SavePath)
{
	//����QtConcurrent�첽��������
}