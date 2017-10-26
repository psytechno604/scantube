#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

#include "measurement.h"
#include "measurement.h"


class MeasurementModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
            ColorRole = Qt::UserRole + 1,
            TextRole
        };
    explicit MeasurementModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int rowCount() const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    Q_INVOKABLE void add(QString text);
    Q_INVOKABLE void add(Measurement *_M);
    void emitDataChanged();
    Measurement * get(int i);
    double getDistance(int channel);

    float getValue(int i, int e, int d);
    void setValue(int i, int e, int d, float value);
signals:

public slots:
    void clear();
    void clear(int newSize);
private:
    QStringList m_data;
    QVector<Measurement *> _data;
};

#endif // MEASUREMENT_H
