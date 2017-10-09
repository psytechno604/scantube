#include "addressprovider.h"

AddressProvider::AddressProvider(QObject *parent) : QObject(parent)
{

}
QHostAddress AddressProvider::getAddress(int index)
{
    return address_table[index];
}

int AddressProvider::getIndex(QHostAddress address)
{

    try {
        return address_table.indexOf(address);
    }
    catch (...) {
        return -1;
    }
}

int AddressProvider::getReceiverCount()
{
    return address_table.length()-1;
}

QVector<QHostAddress> AddressProvider::address_table;
QMap<int, int> AddressProvider::pseudo_index_table;
void AddressProvider::Init()
{
    address_table.push_back(QHostAddress("192.168.0.127")); //synchronizer
    address_table.push_back(QHostAddress("192.168.0.121"));
    address_table.push_back(QHostAddress("192.168.0.122"));
    address_table.push_back(QHostAddress("192.168.0.123"));
    address_table.push_back(QHostAddress("192.168.0.124"));

    pseudo_index_table.insert(100, 0);
    pseudo_index_table.insert(171, 1);
    pseudo_index_table.insert(200, 2);
    pseudo_index_table.insert(271, 3);
    pseudo_index_table.insert(300, 4);
    pseudo_index_table.insert(371, 5);
    pseudo_index_table.insert(400, 6);
    pseudo_index_table.insert(471, 7);

    pseudo_index_table.insert(101, 8);
    pseudo_index_table.insert(110, 9);
    pseudo_index_table.insert(201, 10);
    pseudo_index_table.insert(210, 11);
    pseudo_index_table.insert(301, 12);
    pseudo_index_table.insert(310, 13);
    pseudo_index_table.insert(401, 14);
    pseudo_index_table.insert(410, 15);

    pseudo_index_table.insert(120, 16);
    pseudo_index_table.insert(111, 17);
    pseudo_index_table.insert(220, 18);
    pseudo_index_table.insert(211, 19);
    pseudo_index_table.insert(320, 20);
    pseudo_index_table.insert(311, 21);
    pseudo_index_table.insert(420, 22);
    pseudo_index_table.insert(411, 23);

    pseudo_index_table.insert(121, 24);
    pseudo_index_table.insert(130, 25);
    pseudo_index_table.insert(221, 26);
    pseudo_index_table.insert(230, 27);
    pseudo_index_table.insert(321, 28);
    pseudo_index_table.insert(330, 29);
    pseudo_index_table.insert(421, 30);
    pseudo_index_table.insert(430, 31);

    pseudo_index_table.insert(140, 32);
    pseudo_index_table.insert(131, 33);
    pseudo_index_table.insert(240, 34);
    pseudo_index_table.insert(231, 35);
    pseudo_index_table.insert(340, 36);
    pseudo_index_table.insert(331, 37);
    pseudo_index_table.insert(440, 38);
    pseudo_index_table.insert(431, 39);

    pseudo_index_table.insert(141, 40);
    pseudo_index_table.insert(150, 41);
    pseudo_index_table.insert(241, 42);
    pseudo_index_table.insert(250, 43);
    pseudo_index_table.insert(341, 44);
    pseudo_index_table.insert(350, 45);
    pseudo_index_table.insert(441, 46);
    pseudo_index_table.insert(450, 47);

    pseudo_index_table.insert(160, 48);
    pseudo_index_table.insert(151, 49);
    pseudo_index_table.insert(260, 50);
    pseudo_index_table.insert(251, 51);
    pseudo_index_table.insert(360, 52);
    pseudo_index_table.insert(351, 53);
    pseudo_index_table.insert(460, 54);
    pseudo_index_table.insert(451, 55);

    pseudo_index_table.insert(161, 56);
    pseudo_index_table.insert(170, 57);
    pseudo_index_table.insert(261, 58);
    pseudo_index_table.insert(270, 59);
    pseudo_index_table.insert(361, 60);
    pseudo_index_table.insert(370, 61);
    pseudo_index_table.insert(461, 62);
    pseudo_index_table.insert(470, 63);

}

int AddressProvider::getUnitIndex(QString unitName)
{
    return getUnitIndex(unitName.toInt());
}

int AddressProvider::getUnitIndex(int unitPseudoIndex)
{
    try {
        return pseudo_index_table.value(unitPseudoIndex);
    }
    catch (...) {
        return -1;
    }
}
