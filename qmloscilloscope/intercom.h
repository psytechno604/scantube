#ifndef INTERCOM_H
#define INTERCOM_H

#include <QObject>
#include "tssocket.h"

class intercom : public QObject
{
    Q_OBJECT
public:
    explicit intercom(QObject *parent = nullptr);
    Q_INVOKABLE void on();
    Q_INVOKABLE void off();
private:
    QVector<TSSocket*> _command_sockets;
    QVector<TSSocket*> _data_sockets;
    QUdpSocket *_sender, *_receiver;

    int src_port {1024};
    int dst_port {4096};
    int listen_port {1024};
signals:

public slots:
    void readyRead();
};

#endif // INTERCOM_H
