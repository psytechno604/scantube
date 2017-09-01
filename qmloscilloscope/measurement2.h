#ifndef MEASUREMENT2_H
#define MEASUREMENT2_H

#include <QObject>
#include <QVector>
/*
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
}*/

class Measurement2 : public QObject
{
    Q_OBJECT
public:
    explicit Measurement2(QObject *parent = nullptr);
   /**/ Measurement2(double dst, int nch, int n);
    ~Measurement2();
    double distance;
    QVector<double> _sqerr;
    QVector<double> _corr;
    QVector<QVector<double>> buffer;
    QString getText();
    friend QVector<double> operator-(QVector<double> a, double b);
    friend QVector<double> operator*(QVector<double> a, QVector<double> b);
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
signals:

public slots:
};

#endif // MEASUREMENT2_H
