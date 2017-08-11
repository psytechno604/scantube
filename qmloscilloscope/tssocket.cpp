#include "tssocket.h"
#include <QDebug>

TSSocket::TSSocket(int index, QHostAddress addr, int listen_port, QObject *parent):
    QObject(parent), listen_port(listen_port), addr(addr), index(index)
{
    _receiver = new QUdpSocket(this);
    _receiver->bind(addr, listen_port);
    connect(_receiver, SIGNAL(readyRead()), this, SLOT(readyRead()));


}
void TSSocket::readyRead() {
    qDebug() << "diagram received";
    QByteArray buffer;
    buffer.resize(_receiver->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    _receiver->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << "Socket index: " << QString::number(index);
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer;

    //if (buffer)
}
