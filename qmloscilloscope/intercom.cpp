#include "intercom.h"
#include "addressprovider.h"
#include <QThread>
#include <QCoreApplication>
#include <QString>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>

intercom::intercom(QQuickView *appViewer, QObject *parent) : QObject(parent), m_appViewer(appViewer)
{
}

void intercom::on()
{
    qDebug() << "intercom::on" << QThread::currentThreadId();
    if (!_sender) {
        reCreateSender();
    }
    if (!_sender) {
        return;
    }
    for (auto i=1; i<ips_count; i++) {
        _sender->writeDatagram(QByteArray::fromHex("490008"), AddressProvider::getAddress(1), dst_port);
        QThread::msleep(100);
        _sender->writeDatagram(QByteArray::fromHex("4431"), AddressProvider::getAddress(1), dst_port);
    }
    QThread::msleep(100);
    _sender->writeDatagram(QByteArray::fromHex("4431"), AddressProvider::getAddress(0), dst_port);
    //QThread::msleep(100);

    //qDebug() << "sender: " << _sender->hasPendingDatagrams() << " receiver: " << _receiver->hasPendingDatagrams();
}
void intercom::off()
{
    qDebug() << "intercom::off" << QThread::currentThreadId();
    if (!_sender) {
        return;
    }

    _sender->writeDatagram(QByteArray::fromHex("4430"), AddressProvider::getAddress(0), dst_port);
    //QThread::msleep(100);
    //_sender->writeDatagram(QByteArray::fromHex("4430"), address_provider::get_address(1), dst_port);

    delete _sender;
    _sender = nullptr;
    //qDebug() << _sender->hasPendingDatagrams();
    //_data_sockets[1]->send_data(QByteArray::fromHex("00017f1f7f0e7f017f027f1c7f287f197ef87f137f137f137f1e7f007f1a7f057f217f1c7f147f1a7f157f137f147f127f227f0f7f147f0f7f107f197f0f7f1c7f107f097f267f0b7f2d7f1f7f157f067f127f197f157f027f127f147f167f017f0c7f187f277f0f7f1e7f117f027f167f0e7f0a7f257f167f147f157f1a7f127f1c7f097f147f1c7f147f187f277f057f167f117f1c7f0c7f0c7f0d7f167f0e7f0e7f1c7f207f1b7f207f257f087f2d7f1a7f0a7f137f107f197f167f1d7f197f277f0f7f0f7f0d7f277f047f1d7f217f0d7f0c7f087f137f267f097f187f167f1b7f097f377f1d7f217f197f1c7f037f117f267f127f107f0b7f157f117ef97f087f237f177f067f107f047f137f247f177f1d7f0e7f1b7f0c7f267f1a7f157f207f0b7f057f167f187f147f207f317f147f097f187f297f177f137f1d7f057f047f127f0b7f047f117f137f1f7f257f097f1c7f187f087f087f0a7f1f7f107f167f297f0a7f127f177f0e7f1a7f147f207f137f0c7f1a7f1e7f117f207f167f0a7f2a7f0f7f187f097f117f1f7f147f097f287f207f157efb7f177f087f157f1c7f037f257f147f187f0b7f217f077f247f217f067f087f1b7f187f1d7f057f167f047f177f147f0d7f187f127f147f127f207f127f127efe7f0c7f167f1e7f1a7f087f107f0a7f1e7f1b7f217f0d7f0c7f0b7f217f147f147f317f1e7f047f297f0f7f267f227f1d7f157f147f137f197f037f1e7f0c7f107f1a7f1e7f317f1d7f157f187f0b7f207f0b7f227f117f087f147f297f297f1e7f047f157f0f7f1a7f1e7f1b7f077f197f187f137f207f0a7f167f1d7ef77f067f247f1d7f187f187f117f1c7f077f297f1f7f2b7f267f1a7f1a7f257f0e7f267f187f207f127f0f7f1d7f217f037f167f267f367f0b7f147f247f057f117f0c7f137f207f1b7f117eff7f257f1f7f237f167f177f127f1b7f297f147f277f1b7f167f197f1f7f137f0b7f147f127f347f1a7f1a7f117f1f7f007f0b7f1f7f207f1c7f197f1a7f247f0e7f207f207f0f7f197f147f1c7efe7f177f247f037f187f157f0c7f1e7f137f237f107f1a7f1f7f0d7f147f0d7f147f1e7f107f1c7f0f7f1a7f107f057f287f187f0a7f247f117f217f1c7f337f077f247f107f207f177f077f0e7f1d7f1a7f217f227eff7f147f0e7f027f197f0d7f0f7f1a7f187f307f2f7f277f327f157f157f147f1b7f1e7f267f1a7f0e7f187f1c7f127f0b7f267f1a7f0d7f167f1d7f227f167f0c7f177f1e7f037f117f197f1f7f267f187f0a7f217f1c7f1b7efb7f077f1a7f2c7f117f147f0d7f1b7f2a7f1e7f0c7f127f297f0f7efe7f207f157f207f127f157f1f7f187f0f7f1b7f097f047f157f1f7f177f0e7efb7f1c7f237f1b7f1e7f157f117f267f277f257f147f1d7f0e7f1e7f097f087f1e7f127f1b7f187f327f087f177f107f207f1a7f167f237f177f137f147f217f1a7f207f1c7f117f147f1c7f247f007f157f0b7f207f157f1b7f1e7f107f187f0e7f157f217f1d7f137f227f1e7f2c7f187f1f7f1f7f0d7f137f2e7f187f2b7f287f217f147f1d7f147f337f087f2f7f1e7f157f1a7f0d7f257f0d7f0e7f137f347f137f057f1e7f297f187f277f257f077f297f167f2d7f1e7f1a7f177f217f397f0a7f197f237f227f107f387f237f2c7f207f247f2e7f1b7f237f2a7f237f237f207f0a7f237f2a7f217f137f317f1a7f157f2b7f247f267f0d7f247f257f2a7f217f1e7f197f117f177f1d7f1f7f187f1f7f317f137f107f067f247f267f3a7f1c7f2d7f0e7f287f337f277f2a7f3a7f2b7f267f217f2e7f127f2c7f1e7f2d7f2f7f197f2f7f1c7f367f227f237f267f247f0b7f187f2b7f267f267f2c7f0e7f257f297f2a7f2c7f357f357f0e7f227f0b7f2d7f297f327f247f217f3a7f247f1a7f297f1d7f1f7f257f247f1f7f237f207f1c7f237f187f217f217f227f167f2f7f197f3c7f2d7f0a7f1b7f1c7f237f237f2f7f1b7f267f1e7f157f247f257f2d7f1f7f247f217f1e7f2c7f2c7f217f1f"));

    //00027f0a7efe7f027f107f1d7f1d7efe7f1f7f147f127f0f7f127f167f127f227f107f187f077f0a7f207f357f1e7f117f177f157f1d7f157f0d7f117f107f257f1a7efa7f207f0f7f0b7f1f7f0d7f167f187f187f187f107f147f167f1f7f137f127f287f137f037f007f0f7f0b7f077f2d7f1f7f1f7f157f137f1c7f217f027f177f147f027f107f1c7f247f157f147f007f187f227f1d7f157f0e7f0f7f0b7f167f1b7f167f147f0b7f207f1f7f137f177f0b7f207f0e7f1e7f0d7f1b7f0e7f0d7f127f057f187f117f207f277f177eff7f197f217f147f1f7f107f267f207f1b7f217f1a7f107f047f267f2c7f117f207f0c7f117f037f197f1f7f037f0e7f0e7f0b7f107f107f147f097f187f0d7f0b7f1f7f147f0c7f117f167f147f1e7f247f1f7f317f227f207f0b7f157f107f197f107f0b7f1f7f097f0f7f1e7f177f277f1f7f057f157f147f1d7f077f1a7f137f127f097f1e7f0e7f207f0f7f127f1a7f1b7f207f107f067f1e7f067f077f177f0c7f037f167f137f1d7f257f297f0f7f217f077f207f147f107f177f217f1f7f127f127f197f117f007f267f277f0f7f027f207f077f1d7f267f117f167f197f197f147f217f157f1c7efe7f117f077f147f157f187f177f137f1d7f157f1d7f087f157f097f1d7f367f1c7f177f267f0c7f287eff7f157f167f117f1e7f267f107f137f097f1e7f1d7f1d7f037f117f247f127f0a7f157f1a7f0e7f067f107f157f0b7f197f0d7f1d7efd7f157f007f0a7f157f167f227f1d7f057f177f137f117f117f187f1c7f0d7f1d7f0d7f1c7f1b7efe7f207f127f137f1c7f237f167f067f157f177f1b7f127f357f0c7f177f197f1e7f117f1b7f107f097f0b7f0f7f127f1f7f147f187f0b7f147f227f1d7f027f1e7f1a7f177f187f1e7f187f1b7f247f177f187f257f1f7f177f107f267f187f177f1f7f137f0d7f287f157f1d7f057f267f0b7f167f097f1a7f057f137f157f177f0f7f0d7f0d7f0f7f237f297efa7f0b7f027f1a7f177f027f1b7f1f7f2c7f1b7f0a7f167f2d7f127f327f237f267f0f7f197f127f167f1c7f077f257f1b7f287f297f267f0c7f177f137f1a7f1d7f0f7f097f1c7f127f087f197f307f207f097f087f317f287f1b7f137f147f167f207f007f0d7f277f107f127f197f147f227f207f197f187f257f187f077f127f287f217f127f0b7f267f247f207f107f137f1a7f207f177f147f0c7f0b7f1b7f057f247f1c7f1b7f087f227f0e7f117f037f1f7f237f0b7f187f0f7f1e7f127f257f117f207f0c7f0d7f0c7f247f147f0c7f097f117f267f227f1d7f1f7f1e7f027f127f0c7f007f227f017f0d7f2d7f237f197f267f167f177f197f0d7f0d7f337f0a7f187f1f7f207f1d7f197f117f157f0e7f1b7f237f0e7f1d7f207f097f0e7f267f147f2d7f1e7f177f187f137f1f7f1c7f217f117f157f1f7f1f7f157f0c7f207f1f7f187f1c7f247f167f197f117f0b7f197f237f217f157f197f177f1d7f147f0a7f1c7f027f1e7f1a7f087f2d7f1c7f177f227f107f267f2e7f257f1a7f217f127f227f197f327f107f267f227f177f177f117f167f2c7f0c7f277f0f7f147f2a7f247f367f1b7f257f177f2b7f197f297f297f247f167f1d7f1d7f1e7f277f1f7f147f297f317f187f317f107f247f1c7f227f377f287f317f217f2d7f317f3a7f0f7f207f247f297f107f2a7f2b7f377f207f2d7f1d7f217f107f1d7f207f1d7f1a7f117f267f267f277f1b7f357f127f217f277f217f0c7f247f247f347f207f2d7f1d7f2b7f357f267f377f157f227f297f207f347f1a7f287f2e7f2b7f287f287f107f377f1d7f267f1b7f207f267f1f7f267f197f377f257f237f307f277f227f1f7f1c7f1c7f217f1e7f257f137f197f1c7f1f7f2b7f307f2b7f167f197f1b7f1c7f227f287f1b7f327f0a7f1e7f1e7f2b7f2c7f2d7f217f277f217f197f1b7f217f2b7f2d7f337f117f127f247f107f237f2b7f227f2b7f2c7f227f297f1d7f227f2c7f237f127f187f147f227f127f217f1d
    //00037f1b7f0f7f037f127ee07f1f7f1c7f117f267f0b7f167f1b7f217f127f0d7f0a7f227f157f267f0c7f127f0e7f107f317f257f0b7f237f1e7f137f1b7f117f107f0a7f1b7f0f7f1d7f357f117f1f7f057f067f057f197f1b7f0d7f167f127f187f0d7f1d7f107f1c7f1c7f147f147f217f107f007f2c7f1f7f247f137f217f127f1d7f1a7f1a7f187f047f147f217f127f197f107f217f077f1a7f037f0e7f127f097f127f1f7f0b7f227f0f7f0a7f0c7f187f187f197f197f067f147f2a7f137f247f1c7f177f037f157f187f127f1b7f0f7f047f237f157f147f037f147f1a7f137f0e7f147f077f167f167f1a7f157f1b7f017f1f7f1a7f0d7f227f0e7f197f1f7f147f227f127f117f117f207f267f157f097f167f167f1f7f2e7f157f247f0d7f1b7f147f0f7f2b7f0f7f067f197f267f0e7f237f0b7f217f087f107f2d7f227f1f7f087f1b7f1a7f247f107f147f167f007f127f177f1f7f177f1d7f197f1b7f1f7f0d7f137f1c7f237f127f0d7f097f0f7f117f0e7f217f117efd7f1e7f267f047f2a7f0f7f227f0a7f177f167f177f0b7f1e7f0f7f177f127f167f177f277f047f227f127f077efd7f1d7f107f0a7f077f177f1a7f0f7f167f0d7f167f087f147f1f7f057f177f197f117f097f187f117f217f1f7f177f257f267f137f0a7f1e7f0a7f1e7f207f127f1d7f087f117f217f0f7f2b7f027f0d7f257f037f0a7f1a7f047f237f0b7f067f177f167f107efa7f0f7f277efc7f257f1c7f1b7eff7f227f177f1a7f1d7f0a7f157f177f237f0a7f1b7f077f137f0f7f287f0c7f2b7f1c7f137f1a7f197f097f107f0f7f1b7f0f7f097f147f117f1d7f0b7f157f1f7f0f7f157f1b7f157f2b7f247f007f307f137f1a7f087f127f067f0f7f207f167f187f1a7f167f317f1e7f177f0b7f147f1f7f1d7f097f0e7f157f157f1b7f167f1f7f227f267f157f177f157f1d7f1b7f277f1e7f157f147f137f257f1f7f117f1f7f1c7f1c7f067f0c7f307f0e7f1f7f257f027f167f0f7f227f277f067f067f127f2b7f1d7f297f0e7f227f297f197f1f7f087f227f0f7f227f0f7f1d7ef97f1a7f137f0d7f1e7f0d7f197f237f197f177f107f0a7f157f2e7f1a7f287f1d7f1b7f0a7f077f297f0c7f0e7f1c7f177f1a7f1b7f077f267f257f0c7f197f1b7f087f177f247f157f237f267f267f2b7f267f237f167f0c7f157f167f1b7f137f1c7f237f087f147f237f1d7f137f1b7f137f047f187f297f167f137f037f237f257f2a7f207f177f157f057f2c7f137f147f027f0f7f037f127f127f077f177f087f257f127f097f137f147f1a7f147f1c7f1f7f107f247f2d7f0d7f187f147f1f7f1d7f2f7f0d7f1e7f217f0e7f2b7f0c7f297f137f1e7f117f1d7f0c7f1d7f147f117f187f187f1b7f177f277f157f1b7f1c7f1a7f117f187f2c7f267f197f187f167f197f2b7f137f217f1e7f1d7f237f037f267f107f257f057f197f297f1a7f217f0b7f147f1a7f377f077f177f1b7f1f7f1c7f297f267f257f0e7f2c7f127f2a7f167f267f297f117f227f157f1a7f167f2f7f1d7f197f1a7f237f147f2b7f097f107f187f1a7f307f1f7f1c7f297f197f257f0b7f157f257f297f147f167f1a7f187f0d7f217f197f2b7f167f2d7f257f1e7f2b7f357f2f7f207f297f2d7f137f247f247f207f197f267f347f2d7f187f0f7f187f207f127f2e7f1f7f197f2e7f0f7f1f7f267f217f237f097f1f7f177f2c7f227f347f1b7f207f1e7f247f227f297f287f2d7f337f1e7f287f187f2b7f297f2c7f287f327f337f227f257f157f1c7f287f1f7f2e7f187f217f277f207f337f3f7f1c7f327f1d7f357f257f2a7f1a7f327f387f417f247f1c7f307f2c7f337f1d7f2a7f2c7f237f1b7f237f257f257f287f157f2e7f267f2c7f167f207f337f2c7f257f367f337f197f257f327f2e7f217f177f1b7f1b7f1d7f1b7f1b7f1b7f2a7f1f7f2b7f1d7f297f1c7f1b7f207f2d7f2d7f297f297f2e7f067f227f307f3e7f227f137f107f257f057f287f207f2c7f217f1b
    //00047f097f0b7f187f0a7f227f287f237f167f0f7f2d7f087f117f157f0d7f1f7f157f177f1f7f187f157f117f137f0c7f157f0f7f1b7f0e7f1f7f0c7efb7f207f127f177f087f187f1c7f0d7f187f127f0e7f187f1f7f0f7f067f167f0b7f0f7f117f1e7f107f147f0d7f017f127f1b7f177f0b7f147f277f107f0d7f1d7f097f237f0b7f1a7f117f187f0f7f1a7f237f147f0c7f0f7f0b7f237efc7f0a7ef87f0f7f1b7f1e7f0c7f187f117f067f147f297f1f7f0d7f047f0e7f1d7f257f137f1a7f0e7f187f167f0b7f207eff7f237f0b7f0b7f147f147f017f187f257f147f1c7f177f047f0f7f047f057f0c7eff7f237f1e7f0e7f097f157f067f177f217f147f167f207f197f177f167f107f137f157f257f127f147f1c7f147f167f0f7f2e7f287f1a7f117f0c7f247f167f0b7f1c7f0d7f167f007f1a7f247f117f167f2c7f247f157f1a7f167f117f0b7f0c7f0c7f0c7f067f187f127f3d7f207f0e7f1e7f0a7f0f7f097f1b7f167f157f0e7f2c7f0e7f267f137f0c7f0b7f197f137f257f017f0f7f1a7ef17f057f197f227f187f1a7f097f187eff7eff7f037f087f0a7f187f0d7f157f167f027f167f097f0b7f1b7f227f1d7f0c7f257f137f237f177f247f147f1f7f167f197f0a7efe7f187f117f047f0e7f267f0e7f077f197f187f177f157f167f1a7f2a7f2a7f007f137f197f2a7f1a7f117f0d7f167f167f1b7f187f117f1b7f077f0a7f137eff7f197f0e7f147f0b7f197f1a7f0a7f107f097f197f117f0a7f1e7f127f167f0b7f137f2a7f1d7f0b7f107f167f017f157f267f0c7eec7f187f0d7f1f7f2e7f087f017f327f0f7f0c7f087f037f1c7efd7f1a7f1a7f067f0d7f087f167f227f217f167f107f217f067f087f0a7f0f7f117f087f097f187efe7f017f157f1c7f0d7f217f197f167f2b7f147f097f207f257f0c7f117f097f1e7f0a7f2a7f107eff7f0a7f137f1c7f177f187f0b7f217f107f117f1e7f1c7f1f7f117f107f177f197f087f1e7f117f0a7f107f287f1c7f217f107f1b7f067f177f1e7f117f167f117f0d7f257f177f0c7f097f0d7f107f157f177f0e7f1c7f177f077f027f1e7f097f217f2c7f067f117f247f1c7f217f1c7f0c7f187f197f0b7f097f087f107f1f7f087f247f1a7f0c7f1b7f127f217f117f1b7f127f207f177f1a7f1a7f097f0a7f097f207f197f1b7f0b7f187f137f1a7f1f7f297f137f0d7f097f1f7f157f077f157f0a7f037f227f157f147f137f147f107f197f2a7f047f137f0b7f257f1a7f2b7f197f237f037f187f227f1b7f077f097f127f0f7f197f127f197f1c7f167f0c7f167f1c7f087f0a7f0f7f1c7f1d7f187f1a7f0f7f2d7f177f0c7f0f7f157f1c7f1c7f067f0f7f1c7efc7f0d7f137f207f187f207f1e7f267f187f157f0c7f0d7f197f127f157f1a7efe7f117efb7f1d7f1a7f107f1c7f137f157f1e7f0b7f217f0a7f207f0d7f087f267f317f207f187f287f257f0e7f1e7f157f147f2b7f2a7f067f1f7f1c7f097f1f7f067f187f1d7f247f167f087f157f277f1d7f137f197f277f1f7f187f217f287f0f7f147f1d7f1b7f277f1f7f1f7f1b7f127f187f267f2d7f217f077f2a7f367f1b7f217f1a7f0a7f157f287f227f1f7f227f167f237f297f377f217f1a7f2b7f137f2d7f267f187f207f207f157f1b7f237f2c7f0e7f1c7f137f207f297f217f2d7f0c7f267f1f7f2b7f197f2c7f1c7f2e7f267f2d7f217f217f217f237f267f0a7f257f1f7f207f2c7f1f7f277f0e7f2e7f197f207f1b7f1a7f207f207f1c7f0a7f227f237f387f337f227f2c7f207f247f197f2c7f227f2d7f417f257f147f297f1f7f2f7f297f237f2f7f297f237f2b7f117f177f227f1f7f317f1f7f237f2d7f1c7f297f237f237f147f1d7f327f257f187f297f207f137f247f2a7f207f287f247f2a7f217f227f327f217f257f4c7f177f1b7f187f1e7f187f267f1e7f1a7f277f287f1d7f1d7f397f037f307f237f177f217f237f207f207f0c7f1e7f307f267f207f207f1f7f137f237f157f1e7f21
    //00057efe7f0c7f287f227f0d7f1a7f067f1a7f057f017f0e7f217f117f107f207f107f057f077efd7f147f1e7f0a7f207f037f1b7f117f1a7f127f0d7f157f257f027f057f097f167f137f127f247f027f157f027f0a7f137f037f1e7efb7f1c7f327f167f187f227f167f1a7f037f157f137f197f0e7f267f1b7efb7f1f7f277f0f7f107f117f137f1f7f1f7f297f187f117f167f0c7f207f1d7f197f097f067f147f267f287f077f0a7efe7f1f7f257f117f017f057f077f2c7f0a7f267f137f137f167f077f047f187f187f1d7f1c7f247f0d7f117f107f197f0e7f1b7f107f227f237f0f7f127f117f0f7f1d7f177f0a7f1c7f227f097f107f1d7f037f137f117f147f117f1e7f0d7ef47f147f1e7f117f187f087f077f167f1d7f257f277f107f1a7f257f117f0f7efb7f167f0f7f0f7f0f7f017f0a7f137f197f127f087f167eff7f1c7ef97f1c7efc7f127f147f237f267f1b7f267f137f257f097f077f1e7f0e7f0a7f137ef77f107f167f097f167f117f117f147f1b7f1a7f267f0f7f087f1f7efd7f297f1d7f1e7f217f127f137f297f027f017f2e7f0f7f1f7f117f017f117f0a7f197ef27f157f077f007f0c7f167f147f097f137f1c7efc7f2c7f0b7f1c7f027f0e7f0a7f217f0f7f037f1a7f1a7f197f1b7efa7f067f067f077f1f7f117f177f197f1f7f107f137f107f0d7f0e7f167f157f0a7f1d7f1d7f2e7f097f147f1e7f107f127f127f1e7f0d7f047f057f017f0d7f0f7f087f0b7f127f1c7f147f0e7f1b7f0d7f1a7f1e7f0e7f0b7f1a7f097f267f0e7f257f137f187f1f7f207f167f0b7f277f027f147f077f1e7f0c7f0c7f127f097f0d7f287f107f067f127f047f217f1f7f1a7f1e7f1f7f127f207f207f127efd7f1a7f327f137f1c7f207f207f2e7f207f127ef77f1d7f1a7f1c7f177f1e7f1a7f157f207f157f117f247f0b7f147f157efa7f0c7f217f0e7f127eff7f157f177f2f7f257f157f0c7f107f187f197f1b7ef97f147f0d7f1a7f327f0d7f197f0b7f1c7f197f137f147f177f157f0a7f0e7f157f0e7f1d7eff7f267f287f0f7f1a7f127f147f237f057f147f207f1c7f177f067f0f7f287efe7f0a7f0e7f157f137f1a7f257f197f2e7f187f147f1c7f017f0f7efc7f197f1d7f117f0b7f137f117f137f107f177f047f017f0f7f1c7f0a7f057f107f217f207f1b7f1c7f157f287f067f1e7f387f217f197f267f0f7f137f117f157f057f1c7f1b7f187f287f167efc7f117f137f0f7f217f177f1f7f257f097f147f117f117f0f7f127f0c7f007f1e7f057f297f1a7f157f217f0c7f277f207f257f117f0e7f1b7f167f1d7f0d7f207f077f1b7f087f0a7f247f217f277f107f147f147f037f227f1a7f067f0a7f0a7f307f227f1d7f197f147f1a7f067f257f167f287f1b7f267f237f077f207f1c7f0b7f207f267f117f1d7f1b7f277f1d7f237f2b7f0f7f2b7f1f7f217f0c7f287f0c7f197f127f217f087f097f227f337f137efe7f257f177efb7f1c7f0b7f0b7f1a7f1e7f1e7f317f1e7f1a7f187f177f207f1f7f157f1f7f207f1c7f367f1b7f337f0c7f1b7f377f097f267f057f1b7f1c7f117f267f127f1d7f1b7f267f1f7f257f2a7f1e7f187f137f207f387f187f2a7f1e7f147f1c7f217f1f7f107f287f177f177f167f347f127f247f0c7f287f177f217f1e7f227f257f1e7f227f0e7f227f177f2d7f257f397f167f147f2a7f277f217f317f197f1f7f0f7f2a7f1a7f287f2f7f1e7f287f1e7f3e7f237f177f237f1b7f2e7f1b7f1d7f217f347f2a7f277f0a7f257f2c7f367f267f2f7f197f217f337f287f1b7f147f267f2d7f187f1d7f217f187f297f1c7f287f297f237f187f1f7f397f217f247f187f2c7f227f1e7f177f277f257f207f157f1b7f1b7f1d7f177f197f1d7f2c7f147f257f0b7f2b7f127f2f7f237f1c7f267f297f257f1f7f077f277f177f277f207f0f7f1c7f1f7f257f217f1a7f397f1d7f2d7f1c7f1a7f217f2c7f107f157f247f307f177f167f0b7f207f2e7f157f187f207f0b7f317f1b
}

void intercom::setMyIP(QString ip)
{
    //qDebug() << QThread::currentThreadId();
    myIP = new QHostAddress(ip);
    reCreateSender();
}

void intercom::setAccumulation(QString acc)
{
    if (!_sender) {
        return;
    }
    QString command = "";
    if (acc=="1") command = "570100";
    if (acc=="2") command = "570101";
    if (acc=="4") command = "570102";
    if (acc=="8") command = "570103";
    if (acc=="16") command = "570104";
    if (acc=="32") command = "570105";
    if (acc=="64") command = "570106";
    if (acc=="128") command = "570107";
    if (acc=="256") command = "570108";
    if (acc=="512") command = "570109";
    if (acc=="1024") command = "57010A";

    if (command != "")
        _sender->writeDatagram(QByteArray::fromHex(command.toUtf8()), AddressProvider::getAddress(1), dst_port);
}

void intercom::setSpeed(QString spd)
{
    if (!_sender) {
        return;
    }
    QString accHexValue = "570D8" + spd;

    _sender->writeDatagram(QByteArray::fromHex(accHexValue.toUtf8()), AddressProvider::getAddress(1), dst_port);
}

void intercom::setDataSource(DataSource *ds)
{
    _dataSource = ds;
}

void intercom::sendFix(QString distance)
{
    if (!myIP || !_sender)
        return;
    //_sender->writeDatagram(QString::number(distance).toUtf8(), *myIP, listen_port);
    _sender->writeDatagram(distance.toUtf8(), *myIP, listen_port);

}

void intercom::sendScan()
{
    if (!_sender)
        return;
    _sender->writeDatagram(QByteArray::fromHex("4453"), AddressProvider::getAddress(0), dst_port);
}

void intercom::reCreateSender()
{
    if (!myIP) {
        qDebug() << "myIP not set";
        return;
    }
    if (_sender) {
        delete _sender;
    }
    //_sender = new QUdpSocket(this);
    _sender = new QUdpSocket();
    //_sender->bind(*myIP, src_port);
    _sender->bind(QHostAddress::AnyIPv4, src_port);
    connect(_sender, SIGNAL(readyRead()), this, SLOT(processDatagram()));
}

void intercom::processDatagram() {
    if (!_sender) {
        return;
    }
    //qDebug() << "intercom::processDatagram";
    //return;
    QByteArray buffer;
    buffer.resize(_sender->pendingDatagramSize());

   // _sender->

    QHostAddress sender;
    quint16 senderPort;
    _sender->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    //qDebug() << "Message from: " << sender.toString();
    //qDebug() << "Message port: " << senderPort;
    //qDebug() << "Message: " << buffer;

    int s = buffer.size();

    if (s>3 && s < MIN_DATA_PACKET_SIZE) {
        //_dataSource->save_point(buffer.toDouble(), 10, 0);
        _dataSource->savePoint(distance, 10, 0);
        distance = distance - 0.1;

        if (!object)
            object = m_appViewer->rootObject();

        QMetaObject::invokeMethod((QObject*)object, "setDistance", Q_ARG(QVariant, QString::number(distance)));
    }

    if (_dataSource && s >= MIN_DATA_PACKET_SIZE) {        
        _dataSource->readData(0, &buffer, sender);
        _dataSource->readData(1, &buffer, sender);
    }

}
