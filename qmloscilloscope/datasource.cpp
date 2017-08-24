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

#include "datasource.h"
#include "shared_mutex.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QtMath>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(QQuickView *appViewer, QObject *parent) :
    QObject(parent),
    m_appViewer(appViewer),
    m_index(-1)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    datafile = new QFile("scantube.dat");

    if (!datafile->open(QFile::WriteOnly)){
        qDebug() << "file cannot be opened";
    }
    //generateData(0, 5, 1024);
}

DataSource::~DataSource()
{
    if (datafile) {
        datafile->close();
    }
}

void DataSource::update(QAbstractSeries *series)
{
    mtx.lock_shared();
    if (series && m_data.count()>0) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);

        QVector<QPointF> points = m_data.at(0);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->replace(points);
    }
    mtx.unlock_shared();
}

void DataSource::generateData(QByteArray *buffer, int row)
{


    mtx.lock();

    unsigned short i = (((*buffer)[0] << 8) + (*buffer)[1] ) % 2;

    if (i) {
        datafile->write(buffer->toHex());
        datafile->write(QByteArray("\n"));

        auto N = (*buffer).length();

        QVector<QPointF> points;
        points.reserve(N / 2 - 1);

        m_data.clear();

        for(auto i=2; i<N-1; i+=2) {
            unsigned short p = ((*buffer)[i] << 8) + (*buffer)[i+1];
            auto x = i/2;
            points.append(QPointF(x, p));
        }

        if (row < m_data.length())
            m_data[row] = points;
        else
            m_data.append(points);
    }
    mtx.unlock();
}
