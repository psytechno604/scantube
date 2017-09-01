#include "measurement2.h"

QVector<double> operator-(QVector<double> a, double b)
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
}

/*Measurement2::Measurement2(QObject *parent) : QObject(parent)
{

}*/
Measurement2::Measurement2(QObject *parent) : QObject(parent)
{

}
/**/
QString Measurement2::getText()
{
    QString ret = "d=" + QString::number(distance) + ", ";
    for(auto c=0; c<_sqerr.length(); c++) {
        ret = ret + "#"+QString::number(c) + ": " + QString::number(_sqerr[c]) + ", "+QString::number(_corr[c])+";";
    }
    return ret;
}



Measurement2::Measurement2(double dst, int nch, int n)
{
    distance = dst;
    buffer.resize(nch);
    _sqerr.resize(nch);
    _corr.resize(nch);

    for(auto c=0; c<nch; c++)   {
        buffer[c].resize(n);
    }
}

Measurement2::~Measurement2()
{
    /*for(auto c=0; c<buffer.length(); c++)   {
       // delete buffer[c];
    }*/
}
