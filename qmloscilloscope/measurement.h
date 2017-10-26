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

    QString getText();
    void setTimestmp(QDateTime newtmstmp);
    void readFrom(QDataStream &ds);
    void writeTo(QDataStream &ds);
    QVector<QVector<float> > *getBuffer();
    QVector<QVector<float> > *getPBuffer();
    void setBuffer(QVector<QVector<float>> *b);

    int getSize();

signals:

public slots:
private:
    int size {371101};
    QDateTime timestmp;
    QVector<QVector<float>> *buffer {nullptr};
    QVector<QVector<float>> *processed_buffer {nullptr};
};

#endif // MEASUREMENT2_H
