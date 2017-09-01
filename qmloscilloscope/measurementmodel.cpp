#include "measurementmodel.h"

MeasurementModel::MeasurementModel(QObject *parent)
{

}

int MeasurementModel::rowCount() const
{
    return _data.length();
}
int MeasurementModel::rowCount(const QModelIndex &parent) const
{
    return rowCount();
}
QVariant MeasurementModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case ColorRole:
        return QVariant(index.row() < 2 ? "orange" : "skyblue");
    case TextRole:
        return QString::number(index.row()) + ":" + _data.at(index.row())->getText();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MeasurementModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[ColorRole] = "color";
    roles[TextRole] = "text";

    return roles;
}

bool MeasurementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

Qt::ItemFlags MeasurementModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void MeasurementModel::add(QString text)
{

}

void MeasurementModel::add(Measurement2 *_M)
{
    if (_M) {
        beginInsertRows(QModelIndex(), _data.size(), _data.size());
        _data.append(_M);
        endInsertRows();
    }
}

void MeasurementModel::emitDataChanged()
{
    QModelIndex top = createIndex(0, 0);
    QModelIndex bottom = createIndex(_data.length(), 0);
    emit dataChanged(top, bottom);
}

Measurement2 * MeasurementModel::get(int i)
{
    if (i<0 || i>=_data.length())
        return nullptr;
    return _data[i];
}

double MeasurementModel::getDistance(int channel)
{
    auto max = std::max_element( _data.begin(), _data.end(),
                                 [channel]( Measurement2 *a, const Measurement2  *b )
                                 {
                                     return a->_corr[channel] < b->_corr[channel];
                                 } );
    return (*max)->distance;
}

void MeasurementModel::clear()
{
    beginRemoveRows(QModelIndex(), _data.size(), _data.size());
    for (auto i=0; i<_data.length(); i++)   {
        if (_data[i])
            delete _data[i];
    }
    _data.clear();
    endRemoveRows();
}

