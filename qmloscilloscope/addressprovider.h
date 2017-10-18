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
    static int getUnitIndex(QString unitName);
    static int getUnitIndex(int unitPseudoIndex);
    static QString getUnitName(int unitIndex);
    static QString getShiftCommand(int value);
    static int getReceiver(int unitIndex);
signals:

public slots:

private:
    static QVector<QHostAddress> address_table;
    static QVector<QString> shift_command_table;
    static QMap<int, int> pseudo_index_to_index_table;
    static QMap<int, int> index_to_receiver_table;
};

#endif // ADDRESS_PROVIDER_H
