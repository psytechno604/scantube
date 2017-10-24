#ifndef MEASUREMENT2_H
#define MEASUREMENT2_H

#include <QObject>
#include <QVector>
#include <QDateTime>
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
    QVector<QVector<float>> *buffer {nullptr};
    QString getText();
    void setTimestmp(QDateTime newtmstmp);
    void readFrom(QDataStream &ds);
    void writeTo(QDataStream &ds);
signals:

public slots:
private:
    int size {371101};
    QDateTime timestmp;
};

#endif // MEASUREMENT2_H
