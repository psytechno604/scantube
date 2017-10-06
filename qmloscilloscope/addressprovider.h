#ifndef ADDRESS_PROVIDER_H
#define ADDRESS_PROVIDER_H

#include <QObject>
#include <QUdpSocket>

class AddressProvider : public QObject
{
    Q_OBJECT
public:
    explicit AddressProvider(QObject *parent = nullptr);
    static QHostAddress getAddress(int index);
    static int getIndex(QHostAddress address);
    static int getReceiverCount();
    static void Init();
signals:

public slots:

private:
    static QVector<QHostAddress> address_table;
};

#endif // ADDRESS_PROVIDER_H
