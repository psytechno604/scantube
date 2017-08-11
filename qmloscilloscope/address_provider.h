#ifndef ADDRESS_PROVIDER_H
#define ADDRESS_PROVIDER_H

#include <QObject>
#include <QUdpSocket>

class address_provider : public QObject
{
    Q_OBJECT
public:
    explicit address_provider(QObject *parent = nullptr);
    static QHostAddress get_address(int index);
signals:

public slots:
};

#endif // ADDRESS_PROVIDER_H
