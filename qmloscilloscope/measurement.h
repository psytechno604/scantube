#ifndef MEASUREMENT2_H
#define MEASUREMENT2_H

#include <QObject>
#include <QVector>
#include <QDateTime>
#include "scantube.h"
#include "constants.h"

class Measurement : public QObject
{
    Q_OBJECT
public:
    explicit Measurement(double value=0, QObject *parent = nullptr);

    ~Measurement();
    double m_value;
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

    double getValue();
signals:

public slots:
private:
    int size {371101};
    QDateTime timestmp;
    QVector<QVector<float>> *buffer {nullptr};
    QVector<QVector<float>> *processed_buffer {nullptr};
};

#endif // MEASUREMENT2_H
