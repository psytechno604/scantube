#ifndef TSSOCKET_H
#define TSSOCKET_H

#include <QObject>
#include <QUdpSocket>


class TSSocket : public QObject
{
    Q_OBJECT
public:
    TSSocket(int index, QHostAddress addr, int listen_port, QObject *parent = 0);
    int index;
signals:
public slots:
    void readyRead();
private:

    QUdpSocket *_receiver;

    int src_port, dest_port, listen_port;
    QHostAddress addr;
};

#endif // TSSOCKET_H
