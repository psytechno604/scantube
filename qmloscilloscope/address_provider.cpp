#include "address_provider.h"

address_provider::address_provider(QObject *parent) : QObject(parent)
{

}
QHostAddress address_provider::get_address(int index)
{
    //return QHostAddress::LocalHost;

    if (index==0)
        return QHostAddress("192.168.30.127");
    return QHostAddress("192.168.30." + QString::number(index + 120));
}
