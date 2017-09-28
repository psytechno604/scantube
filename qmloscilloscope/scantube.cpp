#include "scantube.h"

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
double sum(QVector<double> a)
{
    double s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += a[i];
    }
    return s;
}

double mean(QVector<double> a)
{
    return sum(a) / a.size();
}

double sqsum(QVector<double> a)
{
    double s = 0;
    for (int i = 0; i < a.size(); i++)
    {
        s += pow(a[i], 2);
    }
    return s;
}

double stdev(QVector<double> nums)
{
    double N = nums.size();
    return pow(sqsum(nums) / N - pow(sum(nums) / N, 2), 0.5);
}



double pearsoncoeff(QVector<double> X, QVector<double> Y)
{
    return sum((X - mean(X))*(Y - mean(Y))) / (X.size()*stdev(X)* stdev(Y));
}

double shiftedcorr(QVector<double> X, QVector<double> Y, int shift)
{
    if (shift >= 0)
        return pearsoncoeff(X.mid(shift, X.length() - shift), Y.mid(0, Y.length() - shift));
    else
        return pearsoncoeff(X.mid(0, X.length() + shift), Y.mid(-shift, Y.length() + shift));
}
int _FindMaxValueInRangeOFArray(QVector <double>smp, int numsmp, int lowlim, int highlim)
{
  int k;
  double maxv = 0;
  double y1;
  int idx = -1;

  for(k=lowlim; k < highlim; k++)
  {
    y1 = fabs(smp[k]);
    if(y1 > maxv)
    {
      maxv = y1;
      idx = k;
    }
  }
  return idx;
}
