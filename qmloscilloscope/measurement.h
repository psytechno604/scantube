#ifndef MEASUREMENT2_H
#define MEASUREMENT2_H

#include <QObject>
#include <QVector>
#include "scantube.h"

class Measurement : public QObject
{
    Q_OBJECT
public:
    explicit Measurement(QObject *parent = nullptr);
   /**/ Measurement(double dst, int nch, int n);
    ~Measurement();
    double distance;
    QVector<double> _sqerr;
    QVector<double> _corr;
    QVector<QVector<double>> buffer;
    QString getText();   
signals:

public slots:
};

#endif // MEASUREMENT2_H
