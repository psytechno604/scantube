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
Measurement::Measurement(double value, QObject *parent) : QObject(parent), m_value(value)
{
    timestmp = QDateTime::currentDateTime();

    QVector<float> tmp(_historyBufferSize);

    buffer = new QVector<QVector<float>>(_nChannels);
    buffer->fill(tmp);

    processed_buffer = new QVector<QVector<float>>(_nChannels);
    processed_buffer->fill(tmp);
}
/**/
QString Measurement::getText()
{
    QString ret = ":" + QString::number(m_value) + ":" + timestmp.toString("yyyyMMdd hh:mm:ss");
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

    ds >> m_value;

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
    ds << getSize();

    ds << timestmp;

    ds << m_value;

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

QVector<QVector<float> > *Measurement::getBuffer()
{
    return buffer;
}

QVector<QVector<float> > *Measurement::getPBuffer()
{
    return processed_buffer;
}

void Measurement::setBuffer(QVector<QVector<float> > *b)
{
    this->buffer = b;

    if (b && _corr.length() < b->length())  {
        _corr.resize(b->length());
        _corr.fill(0);
    }
}

int Measurement::getSize()
{
    int sz = 2*sizeof(int) + sizeof(double) + sizeof(QDateTime);
    if (buffer) {
        for (auto i=0; i<buffer->length(); i++) {
            sz += sizeof(int) + (*buffer)[i].length()*sizeof(short int);
        }
    }
    return sz;
}

double Measurement::getValue()
{
    return m_value;
}

Measurement::~Measurement()
{
    /*for(auto c=0; c<buffer.length(); c++)   {
       // delete buffer[c];
    }*/
}
