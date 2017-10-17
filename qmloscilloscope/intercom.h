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
    Q_INVOKABLE void setTimeout(int timeout);
    Q_INVOKABLE void setDataSource(DataSource *ds);
    Q_INVOKABLE void sendFix(QString distance);
    Q_INVOKABLE void sendScan();
    Q_INVOKABLE void endScan();
    Q_INVOKABLE void stopScanTimer();
    Q_INVOKABLE void sendTest();

    Q_INVOKABLE int getTimeout();

    Q_INVOKABLE void sendShift(int value);
    Q_INVOKABLE void sendShift(int value, int ipnum);
    Q_INVOKABLE void sendShift(int value1, int value2, int value3, int value4);
    Q_INVOKABLE void setContinueScan(bool continueScan);
private:
    int scan_counter {0};
    QVector<int> packets_received;
    bool m_running {true};

    bool continueScan {true};
    QTime dataReceiveTime;
    QTimer *timer {nullptr};
    int dataReceiveTimeElapsed {0};
    int timeout;
    bool connected {false};

    QQuickItem  *object {nullptr};
    QQuickView *m_appViewer {nullptr};
    QUdpSocket *_sender {nullptr};
    QHostAddress *myIP {nullptr};
    DataSource *_dataSource {nullptr};

    //number of ip's in system
    int ips_count {5};


    int src_port {1024};
    int dst_port {4096};
    int listen_port {1024};
    double distance {30.6};

    int packNum {1};

    void reCreateSender();    
signals:
    void finished();
public slots:
    void run();
    void setRunning(bool running);
    void processDatagram();
};

#endif // INTERCOM_H
