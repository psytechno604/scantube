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

void MeasurementModel::add(Measurement *_M)
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

Measurement * MeasurementModel::get(int i)
{
    if (i<0 || i>=_data.length())
        return nullptr;
    return _data[i];
}

double MeasurementModel::getDistance(int channel)
{
    auto max = std::max_element( _data.begin(), _data.end(),
                                 [channel]( Measurement *a, const Measurement  *b )
                                 {
                                     return a->_corr[channel] < b->_corr[channel];
                                 } );
    return (*max)?(*max)->distance:0;
}

float MeasurementModel::getValue(int i, int e, int d)
{
    auto m = get(i);
    if (m) {
        auto b = m->getBuffer();
        if (b && b->length() > e && (*b)[e].length() > d)  {
            return (*b)[e][d];
        }
    }
    throw 1;
}

void MeasurementModel::setValue(int i, int e, int d, float value)
{
    auto m = get(i);
    if (m) {
        auto b = m->getBuffer();
        if (b && b->length() > e && (*b)[e].length() > d)  {
            (*b)[e][d] = value;
        }
    }
}

void MeasurementModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, _data.size());
    for (auto i=0; i<_data.length(); i++)   {
        if (_data[i])
            delete _data[i];
    }
    _data.clear();
    //emitDataChanged();
    endRemoveRows();

}

void MeasurementModel::clear(int newSize)
{
    if (newSize >= _data.length())
        return;

    beginRemoveRows(QModelIndex(), newSize, _data.size() - newSize + 1);
    for (auto i=newSize; i<_data.length(); i++)   {
        if (_data[i])
            delete _data[i];
    }
    _data.remove(newSize, _data.size() - newSize);

    endRemoveRows();
    emitDataChanged();
}

