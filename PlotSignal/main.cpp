#include "SignalPlotter.h"
#include <QVBoxLayout>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
<<<<<<< HEAD
    QApplication a(argc, argv);
    SignalPlotter w({1,2,3,4,5});
    w.show();
    return a.exec();
=======
	qRegisterMetaType<StreamNode>("StreamNode");
	QApplication a(argc, argv);
	QVector<int> channels = { 0,1,2,3,5 };
	PlotSignal w(channels);
	w.show();
	return a.exec();
>>>>>>> 6f4a008 (链表阻塞)
}
