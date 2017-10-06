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
void AddressProvider::Init()
{
    address_table.push_back(QHostAddress("192.168.0.127"));
    address_table.push_back(QHostAddress("192.168.0.121"));
}
