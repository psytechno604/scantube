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
    static void initAll();
    static void initAddressTable();
    static void initShiftCommandTable();
    static void initShiftCommandTable16();
    static void initPseudoIndexToIndexTable();
    static void initIndexToReceiverTable();
    static int getUnitIndex(QString unitName);
    static int getUnitIndex(int unitPseudoIndex);
    static QString getUnitName(int unitIndex);
    static QString getShiftCommand(int value);
    static QString getShiftCommand16(int value);
    static int getReceiver(int unitIndex);
    static int getPseudoIndex(int unitTndex);
signals:

public slots:

private:
    static QVector<QHostAddress> s_addressTable;
    static QVector<QString> s_shiftCommandTable;
    static QVector<QString> s_shiftCommandTable16;
    static QMap<int, int> s_pseudoIndexToIndexTable;
    static QMap<int, int> s_indexToReceiverTable;
};

#endif // ADDRESS_PROVIDER_H
