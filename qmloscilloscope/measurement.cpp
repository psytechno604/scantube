#include "measurement.h"
#include <QVector>

Measurement::Measurement(QObject *parent) : QObject(parent)
{

}

Measurement::Measurement(int dst, int nch, int n)
{
    distance = dst;
    buffer.resize(nch);
    _sqerr.resize(nch);
    _corr.resize(nch);

    for(auto c=0; c<nch; c++)   {
        buffer[c] = new double[n];
    }
}

Measurement::~Measurement()
{
    for(auto c=0; c<buffer.length(); c++)   {
        delete buffer[c];
    }
}
