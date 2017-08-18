/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets/QApplication>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QtCore/QDir>
#include <QNetworkInterface>
#include <QComboBox>
#include <QObject>
#include "datasource.h"
#include "tssocket.h"
#include "address_provider.h"
#include "intercom.h"
#include "ipslistmodel.h"

#include <QtQml>

int main(int argc, char *argv[])
{
    qDebug() << QThread::currentThreadId();
    // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
    QApplication app(argc, argv);

    qmlRegisterType<IPsListModel>("IPsListModel", 1, 0, "IPsListModel");

    QQuickView viewer;

    // The following are needed to make examples run without having to install the module
    // in desktop environments.
#ifdef Q_OS_WIN
    QString extraImportPath(QStringLiteral("%1/../../../../%2"));
#else
    QString extraImportPath(QStringLiteral("%1/../../../%2"));
#endif
    viewer.engine()->addImportPath(extraImportPath.arg(QGuiApplication::applicationDirPath(),
                                      QString::fromLatin1("qml")));
    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);

    viewer.setTitle(QStringLiteral("QML Oscilloscope"));

    DataSource dataSource(&viewer);
    viewer.rootContext()->setContextProperty("dataSource", &dataSource);

    //QThread* thread = new QThread();
    //thread->start();

    intercom *_intercom = new intercom();

    _intercom->setDataSource(&dataSource);
    //_intercom->moveToThread(thread);
    //QObject::connect(thread, SIGNAL(started()), &_intercom, SLOT(run()));


    viewer.rootContext()->setContextProperty("_intercom", _intercom);

    IPsListModel myIPsListModel;

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost) && !address.isLoopback()){
            qDebug() << address.toString();
            myIPsListModel.add(address.toString());
        }
    }
    viewer.rootContext()->setContextProperty("myIPsListModel", &myIPsListModel);


    viewer.setSource(QUrl("qrc:/qml/qmloscilloscope/main.qml"));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.setColor(QColor("#404040"));
    viewer.show();





    auto ret = app.exec();
    _intercom->off();
    return ret;
}
//WOI1013046429B7AEA7211
