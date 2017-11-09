#ifndef INTERCOM_H
#define INTERCOM_H

#include <QObject>
#include "tssocket.h"
#include "datasource.h"
#include "constants.h"

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
    Q_INVOKABLE void setTimeout(int m_timeout);
    Q_INVOKABLE void setDataSource(DataSource *ds);
    Q_INVOKABLE void sendFix(QString m_Value);
    Q_INVOKABLE void sendScan();
    Q_INVOKABLE void endScan(int flag=0);
    Q_INVOKABLE void endScan0();
    Q_INVOKABLE void stopScanTimer();
    Q_INVOKABLE void sendTest();
    Q_INVOKABLE void scanRange(int m_fullscanCountdownStart=1);

    Q_INVOKABLE int getTimeout();

    Q_INVOKABLE void sendShift(int value);
    Q_INVOKABLE void sendShift16(int value);

    Q_INVOKABLE void setContinueScan(bool m_continueScan);

    Q_INVOKABLE void scanRangeOnce();

    Q_INVOKABLE void setFullscanCountdownStart(int value);
private:
    //bool waiting_for_shift
    int m_fullscanCountdown {0};
    int m_fullscanCountdownStart {10};
    bool m_fullscanModeOn {false};
    bool m_fullscanModeComplete {false};
    int m_currentShift{0};

    int m_ADCNumSteps {_ADCNumSteps};
    int m_ADCStep {0};
    QVector<int> m_packetsReceived;
    bool m_running {true};

    bool m_continueScan {false};
    QTime m_dataReceiveTime;
    QTime m_betweenScansTime;
    QTime m_invocationShitTime;

    QTime m_readDataTime;
    int m_readDataTimeElapsed;

    QTimer *m_timer {nullptr};
    int m_dataReceiveTimeElapsed {0};
    int m_timeout;
    bool m_connected {false};

    QQuickItem  *m_object {nullptr};
    QQuickView *m_appViewer {nullptr};
    QUdpSocket *m_sender {nullptr};
    QHostAddress *m_myIP {nullptr};
    DataSource *m_dataSource {nullptr};

    //number of ip's in system
    int m_IPsCount {_numOfDevices+1};


    int src_port {_srcPort};
    int dst_port {_dstPort};
    int listen_port {_listenPort};

    double m_Value {30.6};


    void m_reCreateSender();
signals:
    void finished();
public slots:
    void run();
    void setRunning(bool running);
    void processDatagram();

    void beforeScanRange();
    void readCommand(QByteArray &buffer);
};

#endif // INTERCOM_H
