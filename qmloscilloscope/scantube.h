#ifndef SCANTUBE_H
#define SCANTUBE_H
#include <QVector>

/*
QVector<double> operator-(QVector<double> a, double b);

QVector<double> operator*(QVector<double> a, QVector<double> b);
double sum(QVector<double> a);

double mean(QVector<double> a);

double sqsum(QVector<double> a);

double stdev(QVector<double> nums);

double pearsoncoeff(QVector<double> X, QVector<double> Y);

double shiftedcorr(QVector<double> X, QVector<double> Y, int shift);*/

template <typename T>QVector<T> operator-(QVector<T> a, T b);

template <typename T>QVector<T> operator*(QVector<T> a, QVector<T> b);

template <typename T>T sum(QVector<T> a);

template <typename T>T mean(QVector<T> a);

template <typename T>T sqsum(QVector<T> a);

template <typename T>T stdev(QVector<T> nums);

template <typename T>T pearsoncoeff(QVector<T> X, QVector<T> Y);

template <typename T>QVector<T> operator-(QVector<T> a, T b)
{
    QVector<T> retvect;
    for (int i = 0; i < a.size(); i++)
    {
        retvect.push_back(a[i] - b);
    }
    return retvect;
}
template <typename T>QVector<T> operator*(QVector<T> a, QVector<T> b)
{
    QVector<T> retvect;
    for (int i = 0; i < a.size() ; i++)
    {
        retvect.push_back(a[i] * b[i]);
    }
    return retvect;
}
template <typename T>T sum(QVector<T> a)
{
    T s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += a[i];
    }
    return s;
}

template <typename T>T mean(QVector<T> a)
{
    return sum<T>(a) / a.size();
}

template <typename T>T sqsum(QVector<T> a)
{
    T s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += pow(a[i], 2);
    }
    return s;
}

template <typename T>T stdev(QVector<T> nums)
{
    T N = nums.size();
    return pow(sqsum<T>(nums) / N - pow(sum<T>(nums) / N, 2), 0.5);
}



template <typename T> inline T pearsoncoeff(QVector<T> X, QVector<T> Y)
{
    return sum<T>((X - mean<T>(X))*(Y - mean<T>(Y))) / (X.size()*stdev<T>(X)* stdev<T>(Y));
}



//template <typename T>T shiftedcorr(QVector<T> X, QVector<T> Y, int shift);

int _FindMaxValueInRangeOFArray(QVector <double> smp, int numsmp, int lowlim, int highlim);

#endif // SCANTUBE_H
