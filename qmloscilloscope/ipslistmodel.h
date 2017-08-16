#ifndef IPSLISTMODEL_H
#define IPSLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QStringList>

class IPsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
            ColorRole = Qt::UserRole + 1,
            TextRole
        };
    IPsListModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    Q_INVOKABLE void add(QString text);

private:
    QStringList m_data;
};

#endif // IPSLISTMODEL_H
