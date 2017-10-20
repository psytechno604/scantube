#include "interfacehelper.h"
#include <QtCore/QDebug>

InterfaceHelper::InterfaceHelper(QObject *parent) : QObject(parent)
{

}



int InterfaceHelper::getIntValue(QString id)
{
    return jsonData[id].toInt();
}

QString InterfaceHelper::getQStringValue(QString id)
{
    return jsonData[id].toString();
}

void InterfaceHelper::setValue(QString id, int value)
{
    qDebug() << "int: " << value;
    jsonData[id] = value;
}

void InterfaceHelper::setValue(QString id, QString value)
{
    qDebug() << "QString: " << value;
    jsonData[id] = value;
}
