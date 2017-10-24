#include "measurement.h"
#include <QDataStream>


/*QVector<double> operator-(QVector<double> a, double b)
{
    QVector<double> retvect;
    for (int i = 0; i < a.size(); i++)
    {
        retvect.push_back(a[i] - b);
    }
    return retvect;
}

QVector<double> operator*(QVector<double> a, QVector<double> b)
{
    QVector<double> retvect;
    for (int i = 0; i < a.size() ; i++)
    {
        retvect.push_back(a[i] * b[i]);
    }
    return retvect;
}*/

/*Measurement2::Measurement2(QObject *parent) : QObject(parent)
{

}*/
Measurement::Measurement(QObject *parent) : QObject(parent)
{
    timestmp = QDateTime::currentDateTime();
}
/**/
QString Measurement::getText()
{
    QString ret = ":" + QString::number(distance) + ":" + timestmp.toString("yyyyMMdd hh:mm:ss");
    /*for(auto c=0; c<_sqerr.length(); c++) {
        ret = ret + QString::number(_corr[c])+":";
    }*/
    return ret;
}

void Measurement::setTimestmp(QDateTime newtmstmp)
{
    timestmp = newtmstmp;
}

void Measurement::readFrom(QDataStream &ds)
{
    if (!buffer)    {
        return;
    }

    ds >> size;

    ds >> timestmp;

    ds >> distance;

    int nchannels, buffer_size;

    ds >> nchannels;

    buffer->resize(nchannels);

    short int rdr;
    for (int i=0; i<nchannels; i++) {
        ds >> buffer_size;
        (*buffer)[i].resize(buffer_size);
        for(int j=0; j<buffer_size; j++) {
            ds >> rdr;
            (*buffer)[i][j] = rdr;
        }
    }

}

void Measurement::writeTo(QDataStream &ds)
{
    ds << size;

    ds << timestmp;

    ds << distance;

    ds << buffer->length();

    if (buffer) {
        for (int e=0; e<buffer->length(); e++) {
            ds << int((*buffer)[e].length());
            for (int i=0; i<(*buffer)[e].length(); i++) {
                ds << short int((*buffer)[e][i]);
            }
        }
    }
}



Measurement::Measurement(double dst, int nch, int n)
{
    timestmp = QDateTime::currentDateTime();

    distance = dst;
    //buffer.resize(nch);
    _sqerr.resize(nch);
    _corr.resize(nch);

    for(auto c=0; c<nch; c++)   {
        buffer[c].resize(n);
    }
}

Measurement::~Measurement()
{
    /*for(auto c=0; c<buffer.length(); c++)   {
       // delete buffer[c];
    }*/
}
