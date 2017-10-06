#ifndef INTERCOM_H
#define INTERCOM_H

#include <QObject>
#include "tssocket.h"
#include "datasource.h"

#define MIN_DATA_PACKET_SIZE 1456

class intercom : public QObject
{
    Q_OBJECT
public:
    explicit intercom(QQuickView *appViewer, QObject *parent = nullptr);
    Q_INVOKABLE void on();
    Q_INVOKABLE void off();
    Q_INVOKABLE void setMyIP(QString ip);
    Q_INVOKABLE void setAccumulation(QString acc);
    Q_INVOKABLE void setSpeed(QString spd);
    Q_INVOKABLE void setDataSource(DataSource *ds);
    Q_INVOKABLE void sendFix(QString distance);
    Q_INVOKABLE void sendScan();
private:
    QQuickItem  *object {nullptr};
    QQuickView *m_appViewer {nullptr};
    QUdpSocket *_sender {nullptr};
    QHostAddress *myIP {nullptr};
    DataSource *_dataSource {nullptr};

    //number of ip's in system
    int ips_count {2};


    int src_port {1024};
    int dst_port {4096};
    int listen_port {1024};
    double distance {30.6};

    void reCreateSender();    
signals:

public slots:
    void processDatagram();
};

#endif // INTERCOM_H
