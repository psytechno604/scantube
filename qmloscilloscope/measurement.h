#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QObject>
#include <QVector>

class Measurement : public QObject
{
    Q_OBJECT
public:
    explicit Measurement(QObject *parent = nullptr);
    Measurement(int dst, int nch, int n);
    ~Measurement();
    double distance;
    QVector<double> _sqerr;
    QVector<double> _corr;
    QVector<double *> buffer;
signals:

public slots:
};

#endif // MEASUREMENT_H
