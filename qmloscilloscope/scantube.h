#ifndef SCANTUBE_H
#define SCANTUBE_H
#include <QVector>

QVector<double> operator-(QVector<double> a, double b);

QVector<double> operator*(QVector<double> a, QVector<double> b);
double sum(QVector<double> a);

double mean(QVector<double> a);

double sqsum(QVector<double> a);

double stdev(QVector<double> nums);

double pearsoncoeff(QVector<double> X, QVector<double> Y);

double shiftedcorr(QVector<double> X, QVector<double> Y, int shift);

int _FindMaxValueInRangeOFArray(QVector <double> smp, int numsmp, int lowlim, int highlim);

#endif // SCANTUBE_H
