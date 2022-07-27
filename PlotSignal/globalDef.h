#pragma once
#include <cmath>
#include <QMap>
#include <QVector>

const int SAMPLEFREQ = 500;
const int CHANNELNUM = 16;
const double UNITRESOL = 0.02235174;
//const double MAXRANGE = (pow(2,32) - 1)*UNITRESOL;
const double MAXRANGE = 150;
const double  FRAMEPERSEC = 30.0;

typedef QMap<int, QVector<double>> Pack;