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
    Q_INVOKABLE void setMyIP(QString ip);
private:
    QUdpSocket *_sender {nullptr};
    QHostAddress *myIP {nullptr};

    int src_port {1024};
    int dst_port {4096};
    int listen_port {1024};

    void reCreateSender();
signals:

public slots:
    void processDatagram();
};

#endif // INTERCOM_H
